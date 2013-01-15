/*
 * SaomTest.h
 *
 *  Created on: Nov 26, 2012
 *      Author: cws
 */

#ifndef SAOMTEST_H_
#define SAOMTEST_H_

#include <vector>
#include "../../model/saom/NetworkEffect.h"
#include "../../model/ModelManager.h"
#include "../../model/MultinomialChoiceChangeModel.h"
#include "../../model/TimeModel.h"
#include "../../processstate/ProcessState.h"
#include "../../simulator/Simulator.h"




void poissonparameterTest(){

	// int nActors = 100;
	double rate = 1.0;
	double periodLength = 1000;
	int nSimulations = 100;
	double deviationOfAverage = 5; // What is the 99% confidence interval?

	double averageSteps = 0;

	for (int iSim = 0; iSim< nSimulations; iSim++){
		ModelManager modelmanager;
		PoissonTimeModel * poissonModel = new PoissonTimeModel(rate);
		modelmanager.addTimeModel(poissonModel);

		ProcessState processState;
		// processState.addNetwork(new MemoryOneModeNetwork(nActors));

		Simulator simulator(&processState, &modelmanager, periodLength);
		simulator.setVerbose(false);

		simulator.simulate();

		averageSteps += ( (double) simulator.getIterationSteps() ) / (double) nSimulations;
	}

	std::cout << "Average number of iteration steps: " << averageSteps << std::endl;

	ASSERT(fabs(averageSteps - (double) (rate * periodLength)) < deviationOfAverage);

}




/**
 * Tests whether a model with a density parameter
 * calculates the correct choice probabilities.
 * The effect itself it tested separately in the @EffectTest suite.
 */
void densityChoiceTest(){

	double densityParameter = -2.0;
	int actorIndex = 0;
	size_t networkIndex = 0;
	int nSimulations = 1000;
	double allowedDeviation = 0.05;

	// calculating expected values
	// network description below
	std::vector<double> expectedValues(5,0.0);
	std::vector<double> estimatedValues(5,0.0);
	double denominator = exp(densityParameter * 3)
			+ 3 * exp( densityParameter * 2)
			+ exp( densityParameter * 4);

	std::cout << "denominator: " << denominator << std::endl;

	// probability to... keep state
	expectedValues[0] = exp(densityParameter * 3) / denominator;
	// ... remove a tie
	expectedValues[1] = exp(densityParameter * 2) / denominator;
	expectedValues[2] = exp(densityParameter * 2) / denominator;
	expectedValues[3] = exp(densityParameter * 2) / denominator;
	// ... add a tie
	expectedValues[4] = exp(densityParameter * 4) / denominator;


	std::set<std::pair<NetworkEffect*, double> *> effects;
	effects.insert(new std::pair<NetworkEffect*, double>(
			new DensityEffect(0), densityParameter));

	MultinomialChoiceNetworkChangeModel saom(
			actorIndex,
			networkIndex,
			&effects,
			new std::vector<Updater*>(1,new TieSwapUpdater(networkIndex)));

	for (int iSim = 0; iSim < nSimulations; iSim++){

		MemoryOneModeNetwork network(5);
		network.addTie(0,1);
		network.addTie(0,2);
		network.addTie(0,3);

		ProcessState processState;
		processState.addNetwork(&network);

		// this is what is actually tested
		// if (iSim == 0) saom.setDebugMode(true); // test verbose
		ModelResult * result = saom.getChange(&processState);

		TieModelResult * actorResult = dynamic_cast<TieModelResult*>(result);

		estimatedValues[actorResult->getActorIndex2()] += 1 / ((double) nSimulations);
	}

	std::cout << "i: " << "expected" << " / " << "estimated" << std:: endl;
	for (int i = 0; i < 5; i++){
		std::cout << i << ": " << expectedValues[i] << " / " << estimatedValues[i] << std:: endl;
	}

	ASSERT(fabs(expectedValues[0] - estimatedValues[0]) < allowedDeviation);
	ASSERT(fabs(expectedValues[1] - estimatedValues[1]) < allowedDeviation);
	ASSERT(fabs(expectedValues[2] - estimatedValues[2]) < allowedDeviation);
	ASSERT(fabs(expectedValues[3] - estimatedValues[3]) < allowedDeviation);
	ASSERT(fabs(expectedValues[4] - estimatedValues[4]) < allowedDeviation);

}

void densityParameterSaomTest(){
	ProcessState processState1;
	ProcessState processState2;
	ModelManager modelManager1;
	ModelManager modelManager2;
	int nActors = 3;
	double densityParameter1 = 0.0;
	double densityParameter2 = -1.0;

	double expectedDensityNull = 0.5;
	double expectedDensityMinusOne = 0.2266792; // from working paper on expected densities.

	double allowedDeviation = 0.02;

	// 100 expected changes per actor to reach stable state
	double rateParameter = 10; // was 100
	double timeSpan = 1;
	int nSimulations = 100; // was 1000

	double averageDensityNull = 0;
	double averageDensityMinusOne = 0;


	size_t networkIndex1 = processState1.addNetwork(new MemoryOneModeNetwork(nActors));
	size_t networkIndex2 = processState2.addNetwork(new MemoryOneModeNetwork(nActors));


	// TODO: if this is moved within the loop, the test crashes.
	std::set<std::pair<NetworkEffect*, double> *> effects1;
	effects1.insert(new std::pair<NetworkEffect*, double>(
			new DensityEffect(networkIndex1), densityParameter1));
	std::set<std::pair<NetworkEffect*, double> *> effects2;
	effects2.insert(new std::pair<NetworkEffect*, double>(
			new DensityEffect(networkIndex2), densityParameter2));

	for (int i = 0; i< nActors; i++){
		PoissonTimeModel * poissonModel = new PoissonTimeModel(rateParameter);

		TieSwapUpdater * tieSwapUpdater = new TieSwapUpdater(networkIndex1);

		MultinomialChoiceNetworkChangeModel * saom1 =
				new MultinomialChoiceNetworkChangeModel(
						i,
						networkIndex1,
						&effects1,
						new std::vector<Updater*>(1, tieSwapUpdater));
		MultinomialChoiceNetworkChangeModel * saom2 =
				new MultinomialChoiceNetworkChangeModel(
						i,
						networkIndex2,
						&effects2,
						new std::vector<Updater*>(1, tieSwapUpdater));

		modelManager1.addTimeModel(poissonModel);
		modelManager1.addChangeModel(poissonModel, saom1);
		modelManager1.addUpdater(saom1, tieSwapUpdater);

		modelManager2.addTimeModel(poissonModel);
		modelManager2.addChangeModel(poissonModel, saom2);
		modelManager2.addUpdater(saom2, tieSwapUpdater);

	}



	for (int iSim = 0; iSim < nSimulations; iSim++){

		Simulator simulator1(&processState1, &modelManager1, timeSpan);
		Simulator simulator2(&processState2, &modelManager2, timeSpan);

		simulator1.setVerbose(false);
		simulator2.setVerbose(false);

		simulator1.simulate();
		simulator2.simulate();

		MemoryOneModeNetwork * network1 = dynamic_cast<MemoryOneModeNetwork*>(processState1.getNetwork(networkIndex1));
		MemoryOneModeNetwork * network2 = dynamic_cast<MemoryOneModeNetwork*>(processState2.getNetwork(networkIndex2));
		NetworkUtils utils;
		double densityNull = utils.getDensity(network1);
		double densityMinusOne = utils.getDensity(network2);
		averageDensityNull += densityNull/nSimulations;
		averageDensityMinusOne += densityMinusOne/nSimulations;

	}

	std::cout << "Average density with parameter 0: " << averageDensityNull << std::endl;
	std::cout << "Average density with parameter -1: " << averageDensityMinusOne << std::endl;

	ASSERT(fabs(averageDensityNull - expectedDensityNull) < allowedDeviation);
	ASSERT(fabs(averageDensityMinusOne - expectedDensityMinusOne) < allowedDeviation);

}



cute::suite getTestSaomSuite(){
	cute::suite s;

	s.push_back(CUTE(poissonparameterTest));
	s.push_back(CUTE(densityChoiceTest));
	s.push_back(CUTE(densityParameterSaomTest));

	return s;
}



#endif /* SAOMTEST_H_ */
