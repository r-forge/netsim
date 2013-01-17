/*
 * NetworkUtils.h
 *
 *  Created on: Nov 21, 2012
 *      Author: cws
 */

#ifndef NETWORKUTILS_H_
#define NETWORKUTILS_H_

#include "OneModeNetwork.h"
#include <iostream>
#include <cstdio>

class NetworkUtils{

public:
	static void dumpNetwork(OneModeNetwork* network, int round = 2);

	static double getDensity(OneModeNetwork* network);

};


#endif /* NETWORKUTILS_H_ */
