/*
 * JacksonRogers2007ChangeModel.cpp
 *
 *  Created on: Feb 13, 2013
 *      Author: cws
 */

#include "JacksonRogers2007ChangeModel.h"

JacksonRogers2007ChangeModel::JacksonRogers2007ChangeModel(int networkIndex,
		double pLinkToParentNode, double pLinkToNeigborNode,
		int nParentNodes, int nNeighborNodes) {
	_networkIndex = networkIndex;
	_pLinkToParentNode = pLinkToParentNode;
	_pLinkToNeigborNode = pLinkToNeigborNode;
	_nParentNodes = nParentNodes;
	_nNeighborNodes = nNeighborNodes;
	_debug = false;
}

ModelResult * JacksonRogers2007ChangeModel::getChange(ProcessState * processState) {

	MemoryOneModeNetwork * network =
			dynamic_cast<MemoryOneModeNetwork*>(processState->getNetwork(_networkIndex));

	std::set<int> nodes;

	if(_debug){
		std::cout << "---- D E B U G --------------------" << std::endl;
	}

	// choose parent nodes and whether to connect to them
	NetworkUtils utils;
	std::set<int> parentNodes = utils.getNRandomNodes(network, _nParentNodes);

	std::set<int>::iterator itNodes = parentNodes.begin();
	for (; itNodes != parentNodes.end(); ++itNodes){
		double rand = Random::getInstance().getRandom();
		if(rand < _pLinkToParentNode)
			nodes.insert(*itNodes);
	}

	if(_debug){
		itNodes = nodes.begin();
		std::cout << "Parents: ";
		for (; itNodes != nodes.end(); ++itNodes)
			std::cout << *itNodes << ", ";
		std::cout << std::endl;
	}



	if (_pLinkToNeigborNode == 0
			|| _nNeighborNodes == 0)
		return new ActorSetModelResult(nodes);

	// choose network based nodes and whether to connect to them
	std::set<int> networkSearchableNodes;
	for (itNodes = parentNodes.begin(); itNodes != parentNodes.end(); ++itNodes){

		std::set<int> neighbors = network->getOutgoingNeighbors(*itNodes);

		// TODO remove
		if (_debug){
			std::cout << "Number of neighbors of " << *itNodes << " = " <<
					neighbors.size() << std::endl;
		}

		networkSearchableNodes.insert(
				neighbors.begin(),
				neighbors.end());

		// TODO remove
		if (_debug){
		std::cout << "New size of network searchable nodes = " <<
				networkSearchableNodes.size() << std::endl;
		}

	}

	// assuming there are exactly two, TODO remove
	if (_debug){
	std::cout << "Size of network searchable nodes inc. potential parents " <<
			networkSearchableNodes.size() << std:: endl;
	}

	// J&R, page 894: These nodes are
	// picked uniformly at random without replacement
	// from the union of the parents’ neighborhoods
	// (excluding all parents).
	// networkSearchableNodes.erase(parentNodes.begin(), parentNodes.end());
	for (itNodes = parentNodes.begin(); itNodes != parentNodes.end(); ++itNodes){
		// TODO remove, and: why did this not work with the one-liner
		if (_debug){
			std::cout << "Erasing node " << *itNodes << std::endl;
		}
		networkSearchableNodes.erase(*itNodes);
	}

	// TODO remove
	if (_debug){
		std::cout << "Size of network searchable nodes without potential parents " <<
				networkSearchableNodes.size() << " : ";
		for (itNodes = networkSearchableNodes.begin();
				itNodes != networkSearchableNodes.end(); ++itNodes)
			std::cout << *itNodes << ", ";
		std::cout << std:: endl;
	}


	// J&R, page 894, footnote 18
	// In a case where the union of the parents’ neighborhoods
	// (excluding the parents) consists of fewer than mn
	// nodes, then all of those and only those nodes are picked.
	// otherwise
	if(networkSearchableNodes.size() > _nNeighborNodes){

		int nNetworkSearchableNodes = networkSearchableNodes.size();
		while (networkSearchableNodes.size() > _nNeighborNodes){
			double rand = Random::getInstance().getRandom();

			// delete one node randomly
			double cumulativeProbability = 0;
			itNodes = networkSearchableNodes.begin();
			for (; itNodes != networkSearchableNodes.end(); ++itNodes){
				cumulativeProbability += 1 / (double) nNetworkSearchableNodes;
				if (cumulativeProbability > rand){
					networkSearchableNodes.erase(*itNodes);
					break; // for loop
				}
			}

		}

		// TODO remove
		if (_debug){
			std::cout << "PROBABILISTIC neighbor choice as there are " <<
					networkSearchableNodes.size() << " neighbor options" << std::endl <<
					"Nodes: ";
			for (itNodes = nodes.begin(); itNodes != nodes.end(); ++itNodes){
				std::cout << *itNodes << ", ";
			}
			std::cout << std::endl;
		}

	}


	// now remove neighbors set (having size <= m_n)
	// from the set with probability (1 - p_n)
	itNodes = networkSearchableNodes.begin();
	for (; itNodes != networkSearchableNodes.end(); ){
		double rand = Random::getInstance().getRandom();

		if (rand > _pLinkToNeigborNode)
			networkSearchableNodes.erase(itNodes++);
		else ++itNodes;

	}


	// add neighbors to set of nodes
	nodes.insert(networkSearchableNodes.begin(), networkSearchableNodes.end());

	if (_debug){
		std::cout << "Final nodes: ";
		for (itNodes = nodes.begin(); itNodes != nodes.end(); ++itNodes)
			std::cout << *itNodes << ", ";
		std::cout << std::endl;
	}

	return new ActorSetModelResult(nodes);

}

void JacksonRogers2007ChangeModel::setDebug(bool debug) {
	_debug = debug;
}
