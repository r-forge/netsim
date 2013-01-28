/*
 * NetworkUtils.cpp
 *
 *  Created on: Nov 21, 2012
 *      Author: cws
 */

#include "NetworkUtils.h"


/**
 *
 */
void NetworkUtils::dumpNetwork(OneModeNetwork* network, int round) {
	std::vector<std::vector<double> > graph = network->getGraph();
	int size = network->getSize();

	// print
	std::cout << "OneModeNetwork, " << size << " nodes" << std::endl;
	for (int i = 0; i < size; i++){
		for (int j = 0; j < size; j++){
			printf("%.2lf", graph[i][j]);
			// double digit = round( graph[i][j] * (1 + round * 10) ) / (1 + round * 10);
			// std::cout << digit;
			if (j == (size - 1)) // last element in a row
				std::cout << std::endl;
			else std::cout << " ";
		}
	}
}

double NetworkUtils::getDensity(OneModeNetwork* network) {
	bool isDirected = network->isDirected();
	bool isReflexive = network->isReflexive();
	int size = network->getSize();
	double nTies = 0;
	int possibleTies = 0;

	// this first case is weird
	if (isDirected && isReflexive) possibleTies = size * size;
	if (isDirected && !isReflexive) possibleTies = size * (size -1);
	if (!isDirected && isReflexive) possibleTies = (size * (size -1) / 2) + size;
	if (!isDirected && !isReflexive) possibleTies = size * (size -1) / 2;

	for (int i = 0; i < size; i++){
		for (int j = 0; j < size; j++){

			if (isDirected){
				if (i != j)
					if (network->hasTie(i,j)) nTies += 1;
				if (isReflexive && i == j)
					if (network->hasTie(i,j)) nTies += 1;
			}

			if (!isDirected){
				if (i < j)
					if (network->hasTie(i,j)) nTies += 1;
				if (isReflexive && i == j)
					if (network->hasTie(i,j)) nTies += 1;
			}
		}
	}
	return nTies / (double) possibleTies;

}

int NetworkUtils::getNumberOfTies(OneModeNetwork* network) {
	return countTies(network, false);
}

int NetworkUtils::getNumberOfReciprocalTies(OneModeNetwork* network) {
	return countTies(network, true);
}

int NetworkUtils::countTies(OneModeNetwork * network,
		bool reciprocal) {

	int nActors = network->getSize();
	int nTies = 0;
	int nReciprocalTies = 0;

	for (int i = 0; i < nActors; i++){
		for (int j = 0; j< nActors; j++){
			if ( !network->isReflexive() && (i < j) ){
				if (network->hasTie(i,j))
					nTies++; // TODO: wrong count for reflexive networks
				if (network->hasTie(j,i))
					nTies++;
				if (network->hasTie(i,j) && network->hasTie(j,i))
					nReciprocalTies++;
			}
		}
	}

	if (reciprocal) return nReciprocalTies;
	else return nTies;
}

