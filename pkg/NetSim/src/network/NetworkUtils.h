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
#include <iostream>
#include <cstdio>

class NetworkUtils{

public:
	static void dumpNetwork(OneModeNetwork* network, int round = 2);

	static double getDensity(OneModeNetwork* network);

	static int getNumberOfTies(OneModeNetwork* network);

	static int getNumberOfReciprocalTies(OneModeNetwork* network);

	static int getHammingDistance(OneModeNetwork* network1, OneModeNetwork * network2);

private:
	static int countTies(OneModeNetwork* network, bool reciprocal);

};


#endif /* NETWORKUTILS_H_ */
