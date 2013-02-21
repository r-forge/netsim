/*
 * NetworkUtils.h
 *
 *  Created on: Nov 21, 2012
 *      Author: cws
 */

#ifndef NETWORKUTILS_H_
#define NETWORKUTILS_H_

#include "OneModeNetwork.h"
#include "MemoryOneModeNetwork.h"
#include "Random.h"
#include <iostream>
#include <cstdio>
#include <set>

class NetworkUtils{

public:
	/**
	 * for debugging
	 */
	static void dumpInternalObjects(MemoryOneModeNetwork* network);

	static void dumpNetwork(MemoryOneModeNetwork* network, int round = 2);

	static double getDensity(MemoryOneModeNetwork* network);

	static int getNumberOfTies(MemoryOneModeNetwork* network);

	static int getNumberOfReciprocalTies(MemoryOneModeNetwork* network);

	static int getHammingDistance(MemoryOneModeNetwork* network1, MemoryOneModeNetwork * network2);

	static std::set<int> getNRandomNodes(Network * network, int n);

private:
	static int countTies(MemoryOneModeNetwork* network, bool reciprocal);

};


#endif /* NETWORKUTILS_H_ */
