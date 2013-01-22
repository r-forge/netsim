/*
 * main.cpp
 *
 *  Created on: Nov 16, 2012
 *      Author: cws
 */

#ifdef INSIDE


#include <Rcpp.h>
#include <RInside.h>	// for the embedded R via RInside
#include <iostream>
#include "rwrapper/rcpp_hello_world.h"
#include "rwrapper/wrapper_process_state.h"
#include "rwrapper/wrapper_network.h"
#include "network/MemoryOneModeNetwork.h"
#include "network/OneModeNetwork.h"
#include "network/NetworkUtils.h"
#include "unittest/TestSuite.h"
#include "model/ModelManager.h"
#include "model/TimeModel.h"
#include "model/Updater.h"
#include "model/MultinomialChoiceChangeModel.h"
#include "model/saom/NetworkEffect.h"
#include "processstate/ProcessState.h"
#include "simulator/Simulator.h"

using namespace Rcpp;

int main(int argc, char *argv[]){

	// define the process state
	ProcessState myProcessState;
	int nActors = 100;

	MemoryOneModeNetwork network(nActors, true, false);
	network.addTie(1,2);

	size_t indexNetwork = myProcessState.addNetwork(&network);

	// define the models and the ModelManager

	ModelManager myModelManager;

	std::set<std::pair<NetworkEffect*, double> *> effectParameterPairs;

	effectParameterPairs.insert(new std::pair<NetworkEffect*, double>(new DensityEffect(indexNetwork), -3.0));
	effectParameterPairs.insert(new std::pair<NetworkEffect*, double>(new ReciprocityEffect(indexNetwork), 1.0));

	// one model chain per actor
	for (int i = 0; i < nActors; i++){

		TieSwapUpdater * tieSwapUpdater = new TieSwapUpdater(indexNetwork);
		MultinomialChoiceNetworkChangeModel* saom =
				new MultinomialChoiceNetworkChangeModel(
						i,
						indexNetwork,
						&effectParameterPairs,
						new std::vector<Updater*>(1, tieSwapUpdater));
		PoissonTimeModel* poissonModel = new PoissonTimeModel(2.0);
		myModelManager.addTimeModel(poissonModel);
		myModelManager.addChangeModel(poissonModel, saom);
		myModelManager.addUpdater(saom, tieSwapUpdater);
	}

//	ExponentialDecayTimeUpdater* timeUpdater = new ExponentialDecayTimeUpdater(indexNetwork, halfLife, resetValue);
//	myModelManager.addTimeUpdater(timeUpdater);

	// define Simulator and simulate

	double periodLength = 1;
	Simulator mySimulator(&myProcessState, &myModelManager, periodLength);

	mySimulator.simulate();

	NetworkUtils utils;
	utils.dumpNetwork((OneModeNetwork*)myProcessState.getNetwork(indexNetwork),1);


	// Run unit tests
	runSuite();

	return 0;
}


#endif
