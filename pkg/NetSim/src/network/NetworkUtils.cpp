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
void NetworkUtils::dumpNetwork(MemoryOneModeNetwork* network, int round) {
	std::vector<std::vector<double> > graph = network->getGraph();
	int size = network->getSize();

	// print
	std::cout << "MemoryOneModeNetwork, " << size << " nodes" << std::endl;
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

double NetworkUtils::getDensity(MemoryOneModeNetwork* network) {
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

int NetworkUtils::getNumberOfTies(MemoryOneModeNetwork* network) {
	return countTies(network, false);
}

int NetworkUtils::getNumberOfReciprocalTies(MemoryOneModeNetwork* network) {
	return countTies(network, true);
}

int NetworkUtils::getHammingDistance(MemoryOneModeNetwork* network1,
		MemoryOneModeNetwork* network2) {
	int nActors = network1->getSize();
	if (network2->getSize() != nActors) return -1;

	int distance = 0;

	for (int i = 0; i < nActors; i++){
		for (int j = 0; j< nActors; j++){
			if(network1->getTieValue(i, j) !=
					network2->getTieValue(i,j))
				distance++;
		}
	}

	return distance;

}

void NetworkUtils::dumpInternalObjects(MemoryOneModeNetwork* network) {

	std::cout << "-------------------" << std::endl;
	std::cout << "Dumping internal objects of a MemoryOneModeNetwork:" << std::endl;
	std::cout << "Size: " << network->_size << std::endl;
	std::cout << "Directed: " << network->_directed << std::endl;
	std::cout << "Reflexive: " << network->_reflexive << std::endl;

	std::cout << "Node : in-degree" << std::endl;
	std::map<int, int>::iterator itInDeg = network->_inDegreeMap.begin();
	for (; itInDeg != network->_inDegreeMap.end(); ++itInDeg){
		std::cout << (*itInDeg).first << " : " << (*itInDeg).second << std::endl;
	}
	std::cout << "Node : out-degree" << std::endl;
	std::map<int, int>::iterator itOutDeg = network->_outDegreeMap.begin();
	for (; itOutDeg != network->_outDegreeMap.end(); ++itOutDeg){
		std::cout << (*itOutDeg).first << " : " << (*itOutDeg).second << std::endl;
	}

	std::cout << "Node : incoming neighbors" << std::endl;
	std::map<int,std::set<int>* >::iterator itIncNeigh =
			network->_incomingNeighborsMap.begin();
	for (; itIncNeigh != network->_incomingNeighborsMap.end(); ++itIncNeigh){
		std::cout << (*itIncNeigh).first << " : ";
		std::set<int>::iterator itNeighbors =  (*itIncNeigh).second->begin();
		for(; itNeighbors !=  (*itIncNeigh).second->end(); ++itNeighbors){
			std::cout << (*itNeighbors) << ", ";
		}
		std::cout << std::endl;
	}

	std::cout << "Node : outgoing neighbors" << std::endl;
	std::map<int,std::set<int>* >::iterator itOutNeigh =
			network->_outgoingNeighborsMap.begin();
	for (; itOutNeigh != network->_outgoingNeighborsMap.end(); ++itOutNeigh){
		std::cout << (*itOutNeigh).first << " : ";
		std::set<int>::iterator itNeighbors =  (*itOutNeigh).second->begin();
		for(; itNeighbors !=  (*itOutNeigh).second->end(); ++itNeighbors){
			std::cout << (*itNeighbors) << ", ";
		}
		std::cout << std::endl;
	}


	std::cout << "-------------------" << std::endl;
}

int NetworkUtils::countTies(MemoryOneModeNetwork * network,
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
					nReciprocalTies += 2;
			}
		}
	}

	if (reciprocal) return nReciprocalTies;
	else return nTies;
}

