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
	//_nextTimeModels = new std::vector<TimeModel>();
	//_nextChangeModels = new std::vector<ChangeModel>();
	//_nextChangeModelResults = new std::vector<ModelResult*>();
	//_nextUpdaters = new std::vector<Updater *>();
}


void Simulator::simulateOLD() {

	if(_verbose)
		std::cout << "Starting OLD simulation. Length = " << _periodLength << ", Time = " << _time <<std::endl;
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
		std::vector<TimeModel*> * timeModels = _modelManager->getTimeModels();
		std::pair<double, std::set<TimeModel *> > timeResult = chooseTimeModels(
				timeModels,
				nextTimeSpan);
		nextTimeSpan = timeResult.first;
		std::set<TimeModel * > nextTimeModels = timeResult.second;

		// TIME UPDATES based on timeSpan
		std::vector<TimeUpdater*> * timeUpdaters = _modelManager->getTimeUpdater();
		applyTimeUpdates(timeUpdaters, nextTimeSpan);

		if (!nextTimeModels.empty()){

			_time += nextTimeSpan;
			nChanges++;

			// apply CHANGE MODELS if at least one time model was chosen
			std::vector<ChangeModel * > * changeModels =
					_modelManager->getChangeModels(nextTimeModels);
			std::vector< std::pair<ModelResult*, Updater*> > resultUpdaterPairs =
					applyChangeModels(changeModels);
			free(changeModels);

			// apply CHANGE UPDATES
			applyChangeUpdates(resultUpdaterPairs);
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

void Simulator::simulate() {

	if(_verbose)
		std::cout << "Starting simulation. Length = " << _periodLength << ", Time = " << _time <<std::endl;
	clock_t timeStart = clock();

	double nextPercentageToPrint = 0;
	_iterationSteps = 0;

	while( _time < _periodLength){

		//simple progress bar
		if (nextPercentageToPrint <= (_time / _periodLength)){
			if (nextPercentageToPrint == 0 && _verbose) std::cout << "Progress (in 10%) [";
			if (_verbose) std::cout << "=" << std::flush;
			nextPercentageToPrint += 0.1;
		}


		// TIME MODELS
		std::vector<TimeModel*> * timeModels = _modelManager->getTimeModels();
		_nextTimeModels.clear();
		// maximum time span
		double timeSpan = (_periodLength - _time);
		for (std::vector<TimeModel*>::iterator it = timeModels->begin();
				it != timeModels->end(); ++it){
			double newTimeSpan = (*it)->getTimeSpan(_processState);
			if (newTimeSpan == timeSpan){
				_nextTimeModels.push_back(*it);
			}
			if (newTimeSpan < timeSpan){
				timeSpan = newTimeSpan;
				_nextTimeModels.clear();
				_nextTimeModels.push_back(*it);
			}
		}
		_time += timeSpan;

		// TIME UPDATES
		std::vector<TimeUpdater *> * timeUpdaters = _modelManager->getTimeUpdater();
		for (std::vector<TimeUpdater *>::iterator it = timeUpdaters->begin();
				it != timeUpdaters->end(); ++it){
			(*it)->update(_processState, timeSpan);
		}

		// CHANGE MODELS
		// get change models from time models
		_nextChangeModels.clear();
		for (std::vector<TimeModel*>::iterator it = _nextTimeModels.begin();
				it != _nextTimeModels.end(); ++it){
			_nextChangeModels.insert(_nextChangeModels.end(),
					_modelManager->getChangeModels(*it)->begin(),
					_modelManager->getChangeModels(*it)->end());
		}
		// get change results and create update vector
		_nextResults.clear();
		_nextUpdaters.clear();
		for (std::vector<ChangeModel * >::iterator it = _nextChangeModels.begin();
				it != _nextChangeModels.end(); ++it){
			ModelResult * result = (*it)->getChange(_processState);
			_nextResults.push_back(result);
			std::vector<Updater *> * updaters = _modelManager->getUpdaters(*it);
			_nextUpdaters.push_back(updaters);
		}

		// UPDATES
		if (_nextResults.size() != _nextUpdaters.size())
			return; // throw error
		for (size_t i = 0; i < _nextResults.size(); i++){
			for (std::vector<Updater *>::iterator it = (*_nextUpdaters[i]).begin();
					it != (*_nextUpdaters[i]).end(); ++it){
				(*it)->update(_processState,
						_nextResults[i]);
			}
		}

		++_iterationSteps;
	} // while loop

	if (_verbose) std::cout << "] done." << std::endl;


	// report time and iteration steps
	if(_verbose){
		double duration = (double)(clock() - timeStart)/CLOCKS_PER_SEC;
		printf("Iteration steps: %d\n", _iterationSteps);
		printf("Simulation time: %.2fs\n", duration);
		printf("Time per iteration: %.6fs\n", duration / ((double) _iterationSteps));
	}

	// reset simulator
	_time = 0;

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
		std::vector<TimeModel*> * timeModels,
		double remainingSimulationTime) {

	std::set<TimeModel *> nextTimeModels;


	for (std::vector<TimeModel*>::iterator itTimeModel = timeModels->begin();
			itTimeModel != timeModels->end();
			++itTimeModel){

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

void Simulator::applyTimeUpdates(std::vector<TimeUpdater*> * timeUpdaters,
		double timeSpan) {
	if (timeUpdaters->empty()) return;
	else {
		TimeModelResult* timeModelResult = new TimeModelResult(timeSpan);

		for (std::vector<TimeUpdater*>::iterator it = timeUpdaters->begin();
				it != timeUpdaters->end(); ++it){

			(*it)->update(_processState,
					timeModelResult);
		}

		delete timeModelResult;
	}

}

std::vector< std::pair<ModelResult*, Updater*> > Simulator::applyChangeModels(
		std::vector<ChangeModel *> * changeModels) {

	// run CHANGE MODELS of each TimeModel and collect results; link these to updates
	std::vector< std::pair<ModelResult*, Updater*> > resultUpdaterPairs;
	std::vector<ChangeModel*>::iterator itChangeModels = changeModels->begin();

	for (; itChangeModels != changeModels->end(); ++itChangeModels){
			ModelResult * result =  (*itChangeModels)->getChange(_processState);
			std::vector<Updater*> * updaters =
					_modelManager->getUpdaters(*itChangeModels);
			// link all Updaters to results
			std::vector<Updater*>::iterator itUpdaters = updaters->begin();
			for (; itUpdaters != updaters->end(); ++itUpdaters){
				resultUpdaterPairs.push_back(std::make_pair(result, *itUpdaters));
			} // updaters

	} // changeModels

	return resultUpdaterPairs;
}

void Simulator::applyChangeUpdates(
	std::vector<std::pair<ModelResult*, Updater*> > resultUpdaterPairs) {
	// This part is separate as updates are subsequently applied to the previous process state
	std::vector< std::pair<ModelResult*, Updater*> >::iterator itResultUpdaterPairs =
			resultUpdaterPairs.begin();
	for (; itResultUpdaterPairs != resultUpdaterPairs.end(); ++itResultUpdaterPairs){
		Updater * updater = (*itResultUpdaterPairs).second;
		ModelResult * result = (*itResultUpdaterPairs).first;
		updater->update(_processState, result);
	}

}
