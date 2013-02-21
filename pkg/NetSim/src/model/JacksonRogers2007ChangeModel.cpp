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
}

ModelResult * JacksonRogers2007ChangeModel::getChange(ProcessState * processState) {

	MemoryOneModeNetwork * network =
			dynamic_cast<MemoryOneModeNetwork*>(processState->getNetwork(_networkIndex));

	std::set<int> nodes;

	// choose parent nodes and whether to connect to them
	NetworkUtils utils;
	std::set<int> parentNodes = utils.getNRandomNodes(network, _nParentNodes);

	std::set<int>::iterator itNodes = parentNodes.begin();
	for (; itNodes != parentNodes.end(); ++itNodes){
		double rand = Random::getInstance().getRandom();
		if(rand < _pLinkToParentNode)
			nodes.insert(*itNodes);
	}


	if (_pLinkToNeigborNode == 0
			|| _nNeighborNodes == 0)
		return new ActorSetModelResult(nodes);

	// choose network based nodes and whether to connect to them
	std::set<int> networkSearchableNodes;
	for (itNodes = parentNodes.begin(); itNodes != parentNodes.end(); ++itNodes){

		networkSearchableNodes.insert(
				network->getOutgoingNeighbors(*itNodes).begin(),
				network->getOutgoingNeighbors(*itNodes).end());

	}

	// J&R, page 894: These nodes are
	// picked uniformly at random without replacement
	// from the union of the parents’ neighborhoods
	// (excluding all parents).
	networkSearchableNodes.erase(parentNodes.begin(), parentNodes.end());

	// J&R, page 894, footnote 18
	// In a case where the union of the parents’ neighborhoods
	// (excluding the parents) consists of fewer than mn
	// nodes, then all of those and only those nodes are picked.
	if (networkSearchableNodes.size() <= _nNeighborNodes){
		nodes.insert(networkSearchableNodes.begin(), networkSearchableNodes.end());
		return new ActorSetModelResult(nodes);
	}

	int nParentNodesChosen = nodes.size();
	int nNetworkSearchableNodes = networkSearchableNodes.size();
	while (nodes.size() < nParentNodesChosen + _nNeighborNodes){
		double rand = Random::getInstance().getRandom();

		itNodes = networkSearchableNodes.begin();
		double cumulativeProbability = 0;
		for (; itNodes != networkSearchableNodes.end(); ++itNodes){
			cumulativeProbability += 1 / (double) nNetworkSearchableNodes;
			if (cumulativeProbability > rand){
				nodes.insert(*itNodes);
				break;
			}
		}

	}


	return new ActorSetModelResult(nodes);

}
