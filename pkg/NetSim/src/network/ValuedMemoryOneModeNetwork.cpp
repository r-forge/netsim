/*
 * ValuedMemoryOneModeNetwork.cpp
 *
 *  Created on: Jan 28, 2013
 *      Author: cws
 */

#include "ValuedMemoryOneModeNetwork.h"

ValuedMemoryOneModeNetwork::ValuedMemoryOneModeNetwork(
		int size, bool directed, bool reflexive) :
		MemoryOneModeNetwork(size, directed, reflexive){

	init();

}

ValuedMemoryOneModeNetwork::ValuedMemoryOneModeNetwork(
		std::vector<std::vector<double> > graph,
		bool directed, bool reflexive) :
			MemoryOneModeNetwork(graph, directed, reflexive){

	init();
}

// TODO: should be implemented in O(log(m)), m = number of ties
// assuming that the key set is ordered
double ValuedMemoryOneModeNetwork::getMaxTieValueBelow(double threshold) {
	std::map<double, std::set<std::pair<int, int> > >::iterator it;
	double maxValue = 0.0;
	for (it = _valueTiesMap.begin(); it != _valueTiesMap.end(); ++it){
		if ( (*it).first >= threshold) return maxValue;
		maxValue = (*it).first;
	}
	return threshold;
}

// TODO: should be implemented in O(log(m)), m = number of ties
// assuming that the key set is ordered
double ValuedMemoryOneModeNetwork::getMinTieValueAbove(double threshold) {
	std::map<double, std::set<std::pair<int, int> > >::iterator it;
	for (it = _valueTiesMap.begin(); it != _valueTiesMap.end(); ++it){
		if ( (*it).first > threshold) return (*it).first;
	}
	return threshold;
}

void ValuedMemoryOneModeNetwork::init() {

	// basically does what updateInternalRepresentation() does
	// but without calling base class
	for (int i = 0; i < _size; i++){
		for (int j = 0; j < _size; j++){
			_tieValueMap[std::make_pair(i,j)] = _graph[i][j];
			( _valueTiesMap[_graph[i][j]] ).insert( std::make_pair(i,j) );
		}
	}


}

void ValuedMemoryOneModeNetwork::updateInternalRepresentation(int i, int j,
		double oldValue, double newValue) {

	MemoryOneModeNetwork::updateInternalRepresentation(i, j, oldValue, newValue);

	// update edge list representation
	_tieValueMap[std::make_pair(i,j)] = newValue; //automatically replaces old value
	( _valueTiesMap[newValue] ).insert( std::make_pair(i,j) );
	( _valueTiesMap[oldValue] ).erase( std::make_pair(i,j) );



}
