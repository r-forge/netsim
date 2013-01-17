/*
 * Simulator.cpp
 *
 *  Created on: Nov 29, 2012
 *      Author: cws
 */

#include <time.h>

#include "Simulator.h"
#include "../utils/Random.h"
#include "../processstate/ProcessState.h"
#include "../processstate/ProcessStateManager.h"
#include "../model/Updater.h"
#include "../model/ModelResult.h"

Simulator::Simulator(ProcessState * processState, ModelManager * modelManager,
		double periodLength) {

	// TODO:check that periodLength > 0

	_time = 0;
	_iterationSteps = 0;
	_periodLength = periodLength;
	_processState = processState;
	_modelManager = modelManager;
	_verbose = true;
}

void Simulator::simulate() {

	if(_verbose)
		std::cout << "Starting simulation. Length = " << _periodLength << ", Time = " << _time <<std::endl;
	clock_t timeStart = clock();

	double nextPercentageToPrint = 0;
	int nChanges = 0;

	while (_time < _periodLength){

		//simple progress bar
		if (nextPercentageToPrint <= (_time / _periodLength)){
			if (nextPercentageToPrint == 0 && _verbose) std::cout << "Progress (in 10%) [";
			if (_verbose) std::cout << "=" << std::flush;
			nextPercentageToPrint += 0.1;
			if (nextPercentageToPrint > 0.9 && _verbose) std::cout << "] done." << std::endl;
		}

		// maximum possible time span
		double timeSpan = _periodLength - _time;

		// -1 indicates that no TimeModel returns shorter than
		// the remaining time span
		int indexTimeModel = -1;
		std::vector<TimeModel*> timeModels = _modelManager->getTimeModels();
		std::set<int> indexesNextTimeModels;

		// choose TIME MODELS and determine timeSpan
		for (std::vector<TimeModel*>::iterator it = timeModels.begin();
				it != timeModels.end(); ++it){

			double tmp = (**it).getTimeSpan(_processState);

			// several changes at the same time
			if (tmp == timeSpan){
				indexTimeModel = it - timeModels.begin();
				// add index to set of indexes
				indexesNextTimeModels.insert(indexTimeModel);
			}

			// a new shorter time interval
			if (tmp < timeSpan){
				timeSpan = tmp;
				indexTimeModel = it - timeModels.begin();
				// reset set of indexed
				indexesNextTimeModels.clear();
				indexesNextTimeModels.insert(indexTimeModel);
			}

		}

		// TIME UPDATES based on timeSpan
		std::vector<TimeUpdater*> timeUpdaters = _modelManager->getTimeUpdater();

		TimeModelResult* timeModelResult = new TimeModelResult(timeSpan);

		for (std::vector<TimeUpdater*>::iterator it = timeUpdaters.begin();
				it != timeUpdaters.end(); ++it){

			(*it)->update(_processState,
					timeModelResult);
		}

		// at least one time model was chosen
		if (indexesNextTimeModels.size() > 0){

			_time += timeSpan;

			nChanges++;

			// run CHANGE MODELS of each TimeModel and collect results; linke these to updates
			std::list< std::pair<ModelResult*, Updater*> > resultUpdaterPairs;
			std::set<int>::iterator itTimeModelIndexes = indexesNextTimeModels.begin();
			for (; itTimeModelIndexes != indexesNextTimeModels.end(); ++itTimeModelIndexes){
				std::vector<ChangeModel*> changeModels = _modelManager->getChangeModels( timeModels[(*itTimeModelIndexes)] );

				std::vector<ChangeModel*>::iterator itChangeModels = changeModels.begin();
				for (; itChangeModels != changeModels.end(); ++itChangeModels){
					ModelResult * result =  (*itChangeModels)->getChange(_processState);
					std::vector<Updater*> updaters = _modelManager->getUpdaters(*itChangeModels);
					// link all Updaters to results
					std::vector<Updater*>::iterator itUpdaters = updaters.begin();
					for (; itUpdaters != updaters.end(); ++itUpdaters){
						resultUpdaterPairs.push_back(std::make_pair(result, *itUpdaters));
					} // updaters
				} // changeModels
			} // timeModels

			// apply all UPDATES using ModelResults
			// This part is separate as updates are subsequently applied to the previous process state
			std::list< std::pair<ModelResult*, Updater*> >::iterator itResultUpdaterPairs = resultUpdaterPairs.begin();
			for (; itResultUpdaterPairs != resultUpdaterPairs.end(); ++itResultUpdaterPairs){
				Updater * updater = (*itResultUpdaterPairs).second;
				ModelResult * result = (*itResultUpdaterPairs).first;
				updater->update(_processState, result);
			}
		}

		// No time model was chosen.
		// This means the remaining time is shorter than any TimeResult
		else {
			_time = _periodLength; // terminates loop
		}

	} // while loop

	_iterationSteps = nChanges;

	// report time and iteration steps
	if(_verbose){
		printf("Iteration steps: %d\n", nChanges);
		printf("Simulation time: %.2fs\n", (double)(clock() - timeStart)/CLOCKS_PER_SEC);
	}

}

SimulationResult Simulator::getSimulationResult() {
	return _result;
}

void Simulator::nextIterationStep() {
	//std::string messageIterationStep = "";


}

bool Simulator::isTimeLeft() {
	return (_time < _periodLength);
}

int Simulator::getIterationSteps() {
	return _iterationSteps;
}

void Simulator::setVerbose(bool verbose) {
	_verbose = verbose;
}

double Simulator::getTimeUntilPeriodEnd() {
	return (_periodLength - _time);
}
