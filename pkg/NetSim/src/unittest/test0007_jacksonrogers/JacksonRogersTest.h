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

	int nSimulations = 10000;
	double allowedDeviation = 0.01;
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
			deviation << " / " << allowedDeviation << ")" << std::endl;
	ASSERT(deviation < allowedDeviation);


	for (size_t i = 0; i < choiceRatios.size(); i++){
		std::cout << "Ratio choices of actor " << i << ": " <<
				choiceRatios[i] << std::endl;
		ASSERT(fabs(choiceRatios[i] - expectedRatios[i]) < allowedDeviation);
	}

}

void jacksonRogersNeighborsSearchTest(){
	int nSimulations = 10000;
	double allowedDeviatino = 0.01;

	//jacksonrogers parameters
	double pLinkParent = 1;
	double pLinkNeighbor = 0.5;
	int nParents = 2;
	int nNeighbors = 1;

	MemoryOneModeNetwork * network = new MemoryOneModeNetwork(4);
	network->addTie(0, 1);
	network->addTie(1, 0);
	network->addTie(1, 3);
	network->addTie(2, 1);

	// Six combinations of parents with probability 1/6 each
	// parents 0,1: choose 3 with p = 0.5
	// parents 0,2: choose 1 with p = 0.5
	// parents 0,3: choose 1 with p = 0.5
	// parents 1,2: choose 0 with p = 0.25, 3 with p = 0.25
	// parents 1,3: choose 0 with p = 0.5
	// parents 2,3: choose 1 with p = 0.5
	// p(0 is neighbor) = 1/6 * 0.25 + 1/6 * 0.5 = 0.125
	// p(1 is neighbor) = 3 * 1/6 * 0.5 + 1/6 * 0.25 = 0.2916667
	// p(2 is neighbor) = 0
	// p(3 is neighbor) = 1/6 * 0.5 = 0.08333333
	std::vector<double> expectedRatios;
	expectedRatios.push_back(0.125);
	expectedRatios.push_back(0.2916667);
	expectedRatios.push_back(0);
	expectedRatios.push_back(0.08333333);
	std::vector<double> choiceRatios(4, 0.0);


	ProcessState * processState = new ProcessState();
	processState->addNetwork(network);

	for (int i = 0; i < nSimulations; i++){

	}


}

cute::suite getJacksonRogersTests(){
	cute::suite s;

	s.push_back(CUTE(jacksonRogersParentSearchTest));

	return s;
}


#endif /* JACKSONROGERSTEST_H_ */
