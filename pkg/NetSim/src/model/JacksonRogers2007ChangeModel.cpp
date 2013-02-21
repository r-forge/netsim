/*
 * JacksonRogers2007ChangeModel.cpp
 *
 *  Created on: Feb 13, 2013
 *      Author: cws
 */

#include "JacksonRogers2007ChangeModel.h"

JacksonRogers2007ChangeModel::JacksonRogers2007ChangeModel(int networkIndex,
		int actorIndex, double pLinkToParentNode, double pLinkToNeigborNode,
		int nParentNodes, int nNeighborNodes) {
	_networkIndex = networkIndex;
	_actorIndex = actorIndex;
	_pLinkToParentNode = pLinkToParentNode;
	_pLinkToNeigborNode = pLinkToNeigborNode;
	_nParentNodes = nParentNodes;
	_nNeighborNodes = nNeighborNodes;
}

ModelResult* JacksonRogers2007ChangeModel::getChange(ProcessState processState) {
	Network * net = processState.getNetwork(_networkIndex);

	int nActors = net->getSize();



	// TODO: include a new node with standard attributes
	// this is not according to the strict change->update logic.
	// However, this relaxation makes the implementation more straightforward
	// but also more restrictive: Inclusion of several nodes at the same time
	// is not safely possible
	int actorId = processState.addActor();

	std::set<int> nodes;

	return new ActorSetModelResult(nodes);

}
