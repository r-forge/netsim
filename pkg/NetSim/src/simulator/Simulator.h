/*
 * Simulator.h
 *
 *  Created on: Nov 29, 2012
 *      Author: cws
 */

#ifndef SIMULATOR_H_
#define SIMULATOR_H_

#include "../processstate/ProcessState.h"
#include "../model/ModelManager.h"
#include "../model/Updater.h"
#include <utility>  // pair
#include <stdio.h>
#include <list>

/**
 * Logs all changes in the simulation process
 * as a list of node-related, relational or global events
 * and provides a human-readable form
 */
class SimulationResult{

public:
	SimulationResult(){ }

	void addLine(std::string line){
		std::string newLine = line.append("\n");
		_textualResult.append(newLine);
	}

private:
	std::string _textualResult;
};

/**
 * Manages a simulation run based on a
 * process state, a model manager and a
 * time period to be simulated. Provides a
 * SimulationResult object.
 */
class Simulator{

public:
	Simulator(ProcessState * processState, ModelManager * modelManager,
			double periodLength);

	void simulate();

	SimulationResult getSimulationResult();

	int getIterationSteps();

	void setVerbose(bool verbose = true);

private:
	double _time;
	double _periodLength;
	SimulationResult _result;
	ProcessState* _processState;
	ModelManager* _modelManager;

	void nextIterationStep();
	bool isTimeLeft();
	double getTimeUntilPeriodEnd();
	int _iterationSteps;
	bool _verbose;
};




#endif /* SIMULATOR_H_ */
