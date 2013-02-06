/*
 * MementoTest.h
 *
 *  Created on: Feb 6, 2013
 *      Author: cws
 */

#ifndef MEMENTOTEST_H_
#define MEMENTOTEST_H_

#include "MemoryOneModeNetwork.h"
#include "ProcessState.h"
#include "NetworkUtils.h"

void networkMementoTest(){
	MemoryOneModeNetwork * network = new MemoryOneModeNetwork(5);

	network->addTie(0,1);
	network->addTie(0,2);
	network->addTie(0,3);
	MemoryOneModeNetworkMemento * memento = network->saveToMemento();
	network->removeTie(0,1);
	network->addTie(0,4);
	network->restoreFromMemento(memento);
	ASSERT(network->hasTie(0,1));
	ASSERT(network->hasTie(0,2));
	ASSERT(network->hasTie(0,3));
	ASSERT(!network->hasTie(0,4));


	delete network;
}

void processStateMementoTestWithNetwork(){
	MemoryOneModeNetwork * network = new MemoryOneModeNetwork(5);

	ProcessState * processState = new ProcessState();

	size_t index = processState->addNetwork(network);

	network->addTie(0,1);
	network->addTie(0,2);
	network->addTie(0,3);
	ProcessStateMemento * memento = processState->saveToMemento();
	network->removeTie(0,1);
	network->addTie(0,4);
	processState->restoreFromMemento(memento);
	ASSERT(network->hasTie(0,1));
	ASSERT(network->hasTie(0,2));
	ASSERT(network->hasTie(0,3));
	ASSERT(!network->hasTie(0,4));
	ASSERT(processState->getNetwork(index)->hasTie(0,1));
	ASSERT(processState->getNetwork(index)->hasTie(0,2));
	ASSERT(processState->getNetwork(index)->hasTie(0,3));
	ASSERT(!processState->getNetwork(index)->hasTie(0,4));

	NetworkUtils utils;
	MemoryOneModeNetwork * net1 = dynamic_cast<MemoryOneModeNetwork*>(processState->getNetwork(index));
	utils.dumpNetwork(net1);

	delete network;
	delete processState;
}

cute::suite getMementoTests(){
	cute::suite s;

	s.push_back(CUTE(networkMementoTest));
	s.push_back(CUTE(processStateMementoTestWithNetwork));

	return s;
}

#endif /* MEMENTOTEST_H_ */
