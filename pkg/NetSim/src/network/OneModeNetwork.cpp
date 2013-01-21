/*
 * Network.cpp
 *
 *  Created on: Nov 20, 2012
 *      Author: cws
 */

#include "OneModeNetwork.h"

OneModeNetwork::OneModeNetwork(int size, bool directed, bool reflexive) {
	_size = size;
	_graph = std::vector<std::vector<double> >(_size, std::vector<double>(_size));
	_directed = directed;
	_reflexive = reflexive;
	init();

}

OneModeNetwork::OneModeNetwork(std::vector<std::vector<double> > graph,
		bool directed, bool reflexive) {
	_size = graph.size();
	_graph = graph;
	_directed = directed;
	_reflexive = reflexive;
	init();
}

OneModeNetwork::~OneModeNetwork() {
}

bool OneModeNetwork::addTie(int i, int j) {

	// check validity
	if (!isIndexesValid(i, j)) return false;

	// set tie value
	return setTie(i, j, DefaultTieValue);
}


bool OneModeNetwork::hasTie(int i, int j) {

	// check validity
	if (!isIndexesValid(i, j)) return false;

	return(_graph[i][j] > 0);

}

bool OneModeNetwork::removeTie(int i, int j) {

	// check validity
	if (!isIndexesValid(i, j)) return false;

	// set tie value
	return setTie(i, j, 0.0);

}

std::vector<std::vector<double> > OneModeNetwork::getGraph() const {
	return _graph;
}

int OneModeNetwork::getSize(){
	return _size;
}

bool OneModeNetwork::setTie(int i, int j, double value) {
	// check validity of index
	if (!isIndexesValid(i, j)) return false;

	// no change at all
	if (_graph[i][j] == value) return true;

	// trying to set a reflexive tie in a non-reflexive network
	if((!_reflexive) && (i == j)) return false;

	// Sets tie value and updates lookup maps
	double oldValue = _graph[i][j];
	_graph[i][j] = value;
	updateInternalRepresentation(i, j, oldValue, value);
	if (!_directed) {
		double oldValue = _graph[j][i];
		_graph[j][i] = value;
		this->updateInternalRepresentation(j, i, oldValue, value);
	}


	return true;
}

bool OneModeNetwork::isIndexValid(int i) {
	return((i >= 0) & (i < _size));
}

bool OneModeNetwork::isIndexesValid(int i, int j) {
	return (isIndexValid(i) & isIndexValid(j));
}

void OneModeNetwork::updateInternalRepresentation(int i, int j,
		double oldValue, double newValue) {
	// no updates if the value did not change
	if (oldValue == newValue) return;

}

bool OneModeNetwork::isDirected() const {
	return _directed;
}

bool OneModeNetwork::isReflexive() const {
	return _reflexive;
}

double OneModeNetwork::getTieValue(int i, int j) {
	return _graph[i][j];
}

void OneModeNetwork::init() {
}
