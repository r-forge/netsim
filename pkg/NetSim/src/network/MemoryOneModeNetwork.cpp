/*
 * MemoryOneModeNetwork.cpp
 *
 *  Created on: Nov 20, 2012
 *      Author: cws
 */

#include "MemoryOneModeNetwork.h"

MemoryOneModeNetwork::MemoryOneModeNetwork(int size, bool directed, bool reflexive)
		: OneModeNetwork(size, directed, reflexive){
	// Remark: init() has to be loaded again, as constructors cannot call
	// overridden virtual functions
	init();
}

MemoryOneModeNetwork::MemoryOneModeNetwork(
		std::vector<std::vector<double> > graph, bool directed, bool reflexive)
		: OneModeNetwork(graph, directed, reflexive){
	init();
}

MemoryOneModeNetwork::~MemoryOneModeNetwork() {
	typedef std::map<int,std::set<int>* >::iterator it_type;
	for (it_type it = _outgoingNeighborsMap.begin();
			it != _outgoingNeighborsMap.end(); ++it){
		delete (*it).second;
	}
	for (it_type it = _incomingNeighborsMap.begin();
			it != _incomingNeighborsMap.end(); ++it){
		delete (*it).second;
	}

}

int MemoryOneModeNetwork::getOutDegree(int i) {
	return _outDegreeMap[i];
}

int MemoryOneModeNetwork::getInDegree(int i) {
	return _inDegreeMap[i];
}

void MemoryOneModeNetwork::init(){

	// initialize lookup maps
	for (int i = 0; i < _size; i++){
		// initialize degree counts with zero
		_inDegreeMap.insert(
				std::map<int,int>::value_type(i,0)
				);
		_outDegreeMap.insert(
				std::map<int,int>::value_type(i,0)
				);
		// initialize neighbor maps with empty sets
		_outgoingNeighborsMap.insert(
				std::map<int, std::set<int>* >::value_type(i,new std::set<int>())
				);
		_incomingNeighborsMap.insert(
				std::map<int, std::set<int>* >::value_type(i,new std::set<int>())
				);
	}

	// set initial values.
	for (int i = 0; i < _size; i++){
		for (int j = 0; j < _size; j++){
			updateInternalRepresentation(i, j, 0, getTieValue(i,j));
		}
	}


}

std::set<int> MemoryOneModeNetwork::getOutgoingNeighbors(int i) {
	return *_outgoingNeighborsMap[i];
}

std::set<int> MemoryOneModeNetwork::getIncomingNeighbors(int i) {
	return *_incomingNeighborsMap[i];
}

std::set<int> MemoryOneModeNetwork::getReciprocalNeighbors(int i) {
	return intersectSets(_incomingNeighborsMap[i], _outgoingNeighborsMap[i]);
}


std::set<int> MemoryOneModeNetwork::getNodesInDistanceTwo(int i) {
	std::set<int> distanceTwoNeighbors;

	for (std::set<int>::iterator it = _outgoingNeighborsMap[i]->begin(); it != _outgoingNeighborsMap[i]->end(); ++it){
		// insert neighbor sets of neighbors
		distanceTwoNeighbors.insert(
				_outgoingNeighborsMap[(*it)]->begin(), _outgoingNeighborsMap[(*it)]->end()
				);
	}

	distanceTwoNeighbors.erase(i);
	return distanceTwoNeighbors;

}

std::set<int> MemoryOneModeNetwork::intersectSets(std::set<int>* set1,
		std::set<int> * set2) {
	std::set<int> resultSet;

	std::set_intersection(set1->begin(), set1->end(), set2->begin(), set2->end(),
			std::inserter(resultSet, resultSet.end()));

	 return resultSet;
}

std::set<int> MemoryOneModeNetwork::getIntermediateNodesBackward(int i, int j) {
	return intersectSets(_incomingNeighborsMap[i], _outgoingNeighborsMap[j]);
}

std::set<int> MemoryOneModeNetwork::getIntermediateNodesUp(int i, int j) {
	return intersectSets(_outgoingNeighborsMap[i], _outgoingNeighborsMap[j]);
}

std::set<int> MemoryOneModeNetwork::getIntermediateNodesDown(int i, int j) {
	return intersectSets(_incomingNeighborsMap[i], _incomingNeighborsMap[j]);
}

std::set<int> MemoryOneModeNetwork::getIntermediateNodesForward(int i, int j) {
	return intersectSets(_outgoingNeighborsMap[i], _incomingNeighborsMap[j]);
}

void MemoryOneModeNetwork::updateInternalRepresentation(int i, int j,
		double oldValue, double newValue) {
	// load function in super class first
	OneModeNetwork::updateInternalRepresentation(i, j, oldValue, newValue);

	// ignore recursive ties
	if ( (!_reflexive) & (i == j) ) return;

	// Tie inclusion
	if (newValue > 0) {
		if (oldValue == 0 ){
			_inDegreeMap[j]++;
			_outDegreeMap[i]++;
			_outgoingNeighborsMap[i]->insert(j);
			_incomingNeighborsMap[j]->insert(i);
		}
	}

	// Tie removal
	if (newValue == 0) {
		if (oldValue > 0) {
			_inDegreeMap[j]--;
			_outDegreeMap[i]--;
			_outgoingNeighborsMap[i]->erase(j);
			_incomingNeighborsMap[j]->erase(i);
		}
	}


}

