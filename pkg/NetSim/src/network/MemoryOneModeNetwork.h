/*
 * MemoryOneModeNetwork.h
 *
 *  Created on: Nov 20, 2012
 *      Author: cws
 */

#ifndef MEMORYONEMODENETWORK_H_
#define MEMORYONEMODENETWORK_H_

#include "OneModeNetwork.h"

#include <map>
#include <set>
#include <boost/unordered_map.hpp>
#include <boost/bimap.hpp> // bi-directional linking of elements
#include <boost/bimap/set_of.hpp>
#include <boost/bimap/multiset_of.hpp>

/**
 * Extension of OneModeNetwork with a number of look ups.
 * Updates of a MemoryOneModeNetwork are slower than updates in a OneModeNetwork.
 * However, the lookup of certain values is much faster.
 * The graph is internally represented both as matrix and a list of pointers to edge vales
 */
class MemoryOneModeNetwork : public OneModeNetwork {

public:

	MemoryOneModeNetwork(int size, bool directed=true, bool reflexive = false);
	MemoryOneModeNetwork(std::vector<std::vector<double> > graph, bool directed=true,
			bool reflexive = false);
	~MemoryOneModeNetwork();

	/**
	 * Returns the (dichotomized) in-degree of a node i in O(1)
	 */
	int getInDegree(int i);

	/**
	 * Returns the (dichotomized) out-degree of a node i in O(1)
	 */
	int getOutDegree(int i);

	/**
	 * Returns a set of outgoing neighbors of node i,
	 * indicating all nodes that i has a tie to
	 */
	std::set<int> getOutgoingNeighbors(int i);

	/**
	 * Returns a set of incoming neighbors of node i,
	 * indicating all nodes that have a tie to node i
	 */
	std::set<int> getIncomingNeighbors(int i);

	/**
	 * Returns a set of neigbors who are reciprocally connected
	 */
	std::set<int> getReciprocalNeighbors(int i);

	/**
	 * returns nodes k that are connected with nodes i and j:
	 * i -> {k} -> j
	 */
	std::set<int> getIntermediateNodesForward(int i, int j);

	/**
	 * returns nodes k that are connected with nodes i and j:
	 * i <- {k} <- j
	 */
	std::set<int> getIntermediateNodesBackward(int i, int j);

	/**
	 * returns nodes k that are connected with nodes i and j:
	 * i -> {k} <- j
	 */
	std::set<int> getIntermediateNodesUp(int i, int j);

	/**
	 * returns nodes k that are connected with nodes i and j:
	 * i <- {k} -> j
	 */
	std::set<int> getIntermediateNodesDown(int i, int j);

	/**
	 * returns all nodes k that are connected with nodes i in distance 2:
	 * i -> ? -> k
	 */
	std::set<int> getNodesInDistanceTwo(int i);



private:

	// lookup maps
	std::map<int, int> _inDegreeMap;
	std::map<int, int> _outDegreeMap;

	std::map<int,std::set<int>* > _outgoingNeighborsMap;
	std::map<int,std::set<int>* > _incomingNeighborsMap;

protected:

	void init();
	void updateInternalRepresentation(int i, int j, double oldValue, double newValue);

	std::set<int> intersectSets(std::set<int>* set1, std::set<int>* set2);

};

class MockMemoryOneModeNetwork : public MemoryOneModeNetwork{

public:

	MockMemoryOneModeNetwork(int size, bool directed=true, bool reflexive = false) :
	MemoryOneModeNetwork(size, directed, reflexive){

	}

	MockMemoryOneModeNetwork(std::vector<std::vector<double> > graph, bool directed=true,
				bool reflexive = false) : MemoryOneModeNetwork(graph, directed, reflexive){

	}

protected:

	// do not make any internal updates after initialization
	void updateInternalRepresentation(int i, int j, double oldValue, double newValue){

	}
};


#endif /* MEMORYONEMODENETWORK_H_ */
