/*
 * MemoryOneModeNetwork.cpp
 *
 *  Created on: Nov 20, 2012
 *      Author: cws
 */

#include "MemoryOneModeNetwork.h"

MemoryOneModeNetwork::MemoryOneModeNetwork(int size, bool directed, bool reflexive)
	//	: OneModeNetwork(size, directed, reflexive){
{
	// Remark: init() has to be loaded again, as constructors cannot call
	// overridden virtual functions
	init(size, directed, reflexive);
}

MemoryOneModeNetwork::MemoryOneModeNetwork(
		std::vector<std::vector<double> > graph, bool directed, bool reflexive)
		// : OneModeNetwork(graph, directed, reflexive){
	{
	int size = graph.size();
	init(size, directed, reflexive);
	for (int i = 0; i < graph.size(); i++){
		for (int j = 0; j < graph[i].size(); j++){
			if (graph[i][j] != 0){
				setTie(i,j,graph[i][j]);
			}
		}
	}
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

void MemoryOneModeNetwork::init(int size, bool directed, bool reflexive){

	_size = size;
	_directed = directed;
	_reflexive = reflexive;

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


	// TODO remove. This is redundant, right?
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

MemoryOneModeNetworkMemento * MemoryOneModeNetwork::saveToMemento() {
	MemoryOneModeNetworkMemento * memento = new MemoryOneModeNetworkMemento;
	memento->saveInDegreeMap(_inDegreeMap);
	memento->saveOutDegreeMap(_outDegreeMap);
	memento->saveIncomingNeighborsMap(_incomingNeighborsMap);
	memento->saveOutgoingNeighborsMap(_outgoingNeighborsMap);
	return memento;
}

void MemoryOneModeNetwork::restoreFromMemento(NetworkMemento * memento) {

	MemoryOneModeNetworkMemento* myMemento =
			dynamic_cast<MemoryOneModeNetworkMemento*>(memento);
	_inDegreeMap = myMemento->getSavedInDegreeMap();
	_outDegreeMap = myMemento->getSavedOutDegreeMap();
	_incomingNeighborsMap = myMemento->getSavedIncomingNeighborsMap();
	_outgoingNeighborsMap = myMemento->getSavedOutgoingNeighborsMap();
}

bool MemoryOneModeNetwork::hasTie(int i, int j) {
	if (_outgoingNeighborsMap[i]->find(j) != _outgoingNeighborsMap[i]->end())
		return true;
	else return false;
}

double MemoryOneModeNetwork::getTieValue(int i, int j) {
	if (_outgoingNeighborsMap[i]->find(j) != _outgoingNeighborsMap[i]->end())
		return 1.0;
	else return 0.0;

}

std::vector<std::vector<double> > MemoryOneModeNetwork::getGraph() {
	std::vector<std::vector<double> > graph(_size, std::vector<double>(_size));
	for (int i = 0; i < _size; i++){
		for (int j = 0; j < _size; j++){
			graph[i][j] = getTieValue(i, j);
		}
	}
	return graph;

}

bool MemoryOneModeNetwork::addTie(int i, int j) {
	return setTie(i, j, 1.0);
}

bool MemoryOneModeNetwork::setTie(int i, int j, double value) {
	// check validity of index
	if (!isIndexValid(i)) return false;
	if (!isIndexValid(j)) return false;

	// no change at all
	if (getTieValue(i,j) == value) return true;

	// trying to set a reflexive tie in a non-reflexive network
	if((!_reflexive) && (i == j)) return false;

	// Sets tie value and updates lookup maps
	double oldValue = getTieValue(i, j);
	updateInternalRepresentation(i, j, oldValue, value);
	if (!_directed) {
		double oldValue = getTieValue(j,i);
		updateInternalRepresentation(j, i, oldValue, value);
	}


	return true;

}

bool MemoryOneModeNetwork::removeTie(int i, int j) {
	return setTie(i, j, 0.0);
}

int MemoryOneModeNetwork::getSize() {
	return _size;
}

bool MemoryOneModeNetwork::isDirected() const {
	return _directed;
}

bool MemoryOneModeNetwork::isReflexive() const {
	return _reflexive;
}

bool MemoryOneModeNetwork::isIndexValid(int i) {
	return((i >= 0) & (i < _size));
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
	// OneModeNetwork::updateInternalRepresentation(i, j, oldValue, newValue);

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

MemoryOneModeNetworkMemento::MemoryOneModeNetworkMemento() {
}

std::map<int, int> MemoryOneModeNetworkMemento::getSavedInDegreeMap() {
	return _inDegreeMap;
}

std::map<int, int> MemoryOneModeNetworkMemento::getSavedOutDegreeMap() {
	return _outDegreeMap;
}

std::map<int, std::set<int> *> MemoryOneModeNetworkMemento::getSavedIncomingNeighborsMap() {
	return _incomingNeighborsMap;
}

std::map<int, std::set<int> *> MemoryOneModeNetworkMemento::getSavedOutgoingNeighborsMap() {
	return _outgoingNeighborsMap;
}

void MemoryOneModeNetworkMemento::saveInDegreeMap(
		const std::map<int, int> inDegreeMap) {
	_inDegreeMap = inDegreeMap;
}

void MemoryOneModeNetworkMemento::saveOutDegreeMap(
		const std::map<int, int> outDegreeMap) {
	_outDegreeMap = outDegreeMap;
}

void MemoryOneModeNetworkMemento::saveIncomingNeighborsMap(
		const std::map<int, std::set<int> *> incomingNeighborsMap) {

	for (std::map<int, std::set<int> *>::const_iterator it = incomingNeighborsMap.begin();
			it != incomingNeighborsMap.end(); ++it){
		delete _incomingNeighborsMap[(*it).first];
		std::set<int> * nodes = new std::set<int>(*((*it).second));
		_incomingNeighborsMap[(*it).first] = nodes;
	}
}

void MemoryOneModeNetworkMemento::saveOutgoingNeighborsMap(
		const std::map<int, std::set<int> *> outgoingNeighborsMap) {
	for (std::map<int, std::set<int> *>::const_iterator it = outgoingNeighborsMap.begin();
			it != outgoingNeighborsMap.end(); ++it){
		delete _outgoingNeighborsMap[(*it).first];
		std::set<int> * nodes = new std::set<int>(*((*it).second));
		_outgoingNeighborsMap[(*it).first] = nodes;
	}
}
