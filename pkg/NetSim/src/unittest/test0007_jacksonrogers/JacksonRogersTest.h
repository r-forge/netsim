/*
 * JacksonRogersTest.h
 *
 *  Created on: Feb 21, 2013
 *      Author: cws
 */

#ifndef JACKSONROGERSTEST_H_
#define JACKSONROGERSTEST_H_

#include "cute.h"
#include "ModelResult.h"
#include "JacksonRogers2007ChangeModel.h"

void jacksonRogersParentSearchTest(){

	int nSimulations = 20000;
	double allowedDeviation1 = 0.02;
	double allowedDeviation2 = 0.02;
	int networkSize = 10;
	// jackson rogers parameters
	int potentialParents = 5;
	double pLink = 0.5;

	double avgNParents = 0;
	double expectedNParents = ((double) potentialParents) * pLink;
	std::vector<double> choiceRatios(networkSize, 0.0);
	double expectationChoiceRatios = potentialParents * pLink / networkSize;
	std::vector<double> expectedRatios(networkSize, expectationChoiceRatios);

	MemoryOneModeNetwork network(networkSize);
	ProcessState * processState = new ProcessState();
	processState->addNetwork(&network);

	JacksonRogers2007ChangeModel jrModel(0, pLink, 0, potentialParents, 0);

	for (int i = 0; i< nSimulations; i++){
		ModelResult * r = jrModel.getChange(processState);
		ActorSetModelResult * result = dynamic_cast<ActorSetModelResult* >(r);

		std::set<int> parents = result->getActorSet();

		avgNParents += ((double)parents.size()) / (double) nSimulations;

		std::set<int>::iterator itParents = parents.begin();
		for( ; itParents != parents.end(); ++itParents){
			choiceRatios[*itParents] += 1 / (double) nSimulations;
		}
	}

	double deviation = fabs(avgNParents - expectedNParents);
	std::cout << "average number of parents: " << avgNParents <<
			"(exp.: " << expectedNParents << ", dev.: "<<
			deviation << " / " << allowedDeviation1 << ")" << std::endl;
	ASSERT(deviation < allowedDeviation1);


	for (size_t i = 0; i < choiceRatios.size(); i++){
		std::cout << "Ratio choices of actor " << i << ": " <<
				choiceRatios[i] << std::endl;
		ASSERT(fabs(choiceRatios[i] - expectedRatios[i]) < allowedDeviation2);
	}

}

void jacksonRogersNeighborsSearchTest(){
	int nSimulations = 10000;
	double allowedDeviation1 = 0.01;
	double allowedDeviation2 = 0.01;

	// jackson&rogers model parameters
	// do not change, expectations are hard-coded below
	double pLinkParent = 1;
	double pLinkNeighbor = 0.4;
	int nParents = 2;
	int nNeighbors = 1;

	MemoryOneModeNetwork * network = new MemoryOneModeNetwork(4);
	network->addTie(0, 1);
	network->addTie(1, 0);
	network->addTie(1, 3);
	network->addTie(2, 1);

	// Six combinations of parents with probability 1/6 each
	// parents 0,1: choose 3 with p = 0.4
	// parents 0,2: choose 1 with p = 0.4
	// parents 0,3: choose 1 with p = 0.4
	// parents 1,2: choose 0 with p = 0.2, 3 with p = 0.2
	// parents 1,3: choose 0 with p = 0.4
	// parents 2,3: choose 1 with p = 0.4
	// 1/2 it the probability of being a parent
	// p(0 is in set) = 1/2 + 1/6 * 0.4 + 1/6 * 0.2 = 0.6
	// p(1 is in set) = 1/2 + 1/2 * 0.4 = 0.7
	// p(2 is in set) = 1/2 = 0.5
	// p(3 is in set) = 1/2 + 1/6 * 0.4 + 1/6 * 0.2 = 0.6
	std::vector<double> expectedRatios;
	expectedRatios.push_back(0.6);
	expectedRatios.push_back(0.7);
	expectedRatios.push_back(0.5);
	expectedRatios.push_back(0.6);
	std::vector<double> choiceRatios(4, 0.0);
	double expectedNNodes = 2 + 0.4;

	double avgNNodes = 0;


	ProcessState * processState = new ProcessState();
	processState->addNetwork(network);

	JacksonRogers2007ChangeModel jrModel(0, pLinkParent, pLinkNeighbor, nParents, nNeighbors);
	jrModel.setDebugMode(false);

	for (int i = 0; i < nSimulations; i++){

		ActorSetModelResult * result =
				dynamic_cast<ActorSetModelResult * >(jrModel.getChange(processState));
		std::set<int> neighbors = result->getActorSet();

		ASSERT(neighbors.size() <= nNeighbors + nParents);

		avgNNodes += (double) neighbors.size() / nSimulations;

		std::set<int>::iterator itNeighbors = neighbors.begin();
		for ( ; itNeighbors != neighbors.end(); ++itNeighbors){
			choiceRatios[*itNeighbors] += 1 / (double) nSimulations;
		}

	}

	ASSERT(fabs(avgNNodes - expectedNNodes) < allowedDeviation1);
	std::cout << "Average number of linked nodes: " << avgNNodes << std::endl;

	for (size_t i = 0; i < choiceRatios.size(); i++){
		std::cout << "Average number of selection of node " << i << ": " <<
				choiceRatios[i] << " (exp.: " << expectedRatios[i] << ")" << std::endl;
		ASSERT(fabs(choiceRatios[i] - expectedRatios[i]) < allowedDeviation2);
	}
}

void jacksonRogersBirthModelSimulationTest(){
	MemoryOneModeNetwork * network = new MemoryOneModeNetwork(20);
	NetworkUtils utils;
	utils.addRandomTiesToNetwork(network, 0.25);

	// process state
	ProcessState * processState = new ProcessState;
	int networkIndex = processState->addNetwork(network);
	// timer
	int timerIndex = processState->addGlobalAttribute(0);

	// model manager
	ModelManager * modelManager = new ModelManager;

	// TODO method stub
}

cute::suite getJacksonRogersTests(){
	cute::suite s;

	s.push_back(CUTE(jacksonRogersParentSearchTest));
	s.push_back(CUTE(jacksonRogersNeighborsSearchTest));


	return s;
}


#endif /* JACKSONROGERSTEST_H_ */
