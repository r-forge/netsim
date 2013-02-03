/*
 * Simulator.cpp
 *
 *  Created on: Nov 29, 2012
 *      Author: cws
 */

#include "Simulator.h"

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
		}

		// maximum possible time span
		double nextTimeSpan = _periodLength - _time;


		// choose TIME MODELS and determine timeSpan
		std::vector<TimeModel*> timeModels = _modelManager->getTimeModels();
		std::pair<double, std::set<TimeModel *> > timeResult = chooseTimeModels(timeModels, nextTimeSpan);
		nextTimeSpan = timeResult.first;
		std::set<TimeModel * > nextTimeModels = timeResult.second;

		// TIME UPDATES based on timeSpan
		std::vector<TimeUpdater*> timeUpdaters = _modelManager->getTimeUpdater();
		applyTimeUpdates(timeUpdaters, nextTimeSpan);

		if (!nextTimeModels.empty()){

			_time += nextTimeSpan;
			nChanges++;

			// apply CHANGE MODELS if at least one time model was chosen
			std::vector<ChangeModel * > changeModels = _modelManager->getChangeModel(nextTimeModels);
			std::list< std::pair<ModelResult*, Updater*> > resultUpdaterPairs = applyChangeModels(changeModels);

			// apply CHANGE UPDATES

			// apply all UPDATES using ModelResults
			// This part is separate as updates are subsequently applied to the previous process state
			std::list< std::pair<ModelResult*, Updater*> >::iterator itResultUpdaterPairs = resultUpdaterPairs.begin();
			for (; itResultUpdaterPairs != resultUpdaterPairs.end(); ++itResultUpdaterPairs){
				Updater * updater = (*itResultUpdaterPairs).second;
				ModelResult * result = (*itResultUpdaterPairs).first;
				updater->update(_processState, result);
			}
		} // if at least one time model was chosen

		// No time model was chosen.
		// This means the remaining time is shorter than any TimeResult
		else {
			_time = _periodLength; // terminates loop

		}

	} // while loop

	if (_verbose) std::cout << "] done." << std::endl;


	_iterationSteps = nChanges;

	// report time and iteration steps
	if(_verbose){
		double duration = (double)(clock() - timeStart)/CLOCKS_PER_SEC;
		printf("Iteration steps: %d\n", nChanges);
		printf("Simulation time: %.2fs\n", duration);
		printf("Time per iteration: %.6fs\n", duration / ((double) nChanges));
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

std::pair<double, std::set<TimeModel *> > Simulator::chooseTimeModels(
		std::vector<TimeModel*> timeModels,
		double remainingSimulationTime) {

	// -1 indicates that no TimeModel returns shorter than
	// the remaining time span

	std::set<TimeModel *> nextTimeModels;


	for (std::vector<TimeModel*>::iterator itTimeModel = timeModels.begin();
			itTimeModel != timeModels.end(); ++itTimeModel){

		double latestTimeSpan = (**itTimeModel).getTimeSpan(_processState);

		// several changes at the same time
		if (latestTimeSpan == remainingSimulationTime){
			nextTimeModels.insert(*itTimeModel);
		}

		// a new shorter time interval
		if (latestTimeSpan < remainingSimulationTime){
			remainingSimulationTime = latestTimeSpan;
			nextTimeModels.clear();
			nextTimeModels.insert(*itTimeModel);
		}

	}

	return std::make_pair(remainingSimulationTime, nextTimeModels);

}

void Simulator::applyTimeUpdates(std::vector<TimeUpdater*> timeUpdaters,
		double timeSpan) {
	if (timeUpdaters.empty()) return;
	else {
		TimeModelResult* timeModelResult = new TimeModelResult(timeSpan);

		for (std::vector<TimeUpdater*>::iterator it = timeUpdaters.begin();
				it != timeUpdaters.end(); ++it){

			(*it)->update(_processState,
					timeModelResult);
		}

		delete timeModelResult;
	}

}

std::list< std::pair<ModelResult*, Updater*> > Simulator::applyChangeModels(std::vector<ChangeModel *> changeModels) {


				// run CHANGE MODELS of each TimeModel and collect results; link these to updates
				std::list< std::pair<ModelResult*, Updater*> > resultUpdaterPairs;
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

				return resultUpdaterPairs;
}

void Simulator::applyChangeUpdates(
		std::list<std::pair<ModelResult*, Updater*> > pair) {
}
