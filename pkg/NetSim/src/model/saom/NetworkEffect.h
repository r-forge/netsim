/*
 * NetworkEffect.h
 *
 *  Created on: Dec 9, 2012
 *      Author: cws
 */

#ifndef NETWORKEFFECT_H_
#define NETWORKEFFECT_H_

#include "../../processstate/ProcessState.h"

/**
 * Abstract class of network effects to be used in
 * multinomial choice models
 */
class NetworkEffect {

public:
	virtual ~NetworkEffect() { }
	virtual double getEffect(ProcessState * processState, int actorIndex) = 0;
	virtual std::string getName() = 0;

};

class OneModeNetworkEffect : public NetworkEffect{

public:
	OneModeNetworkEffect(size_t networkIndex){
		_networkIndex = networkIndex;
	}

protected:
	size_t _networkIndex;

};


class DensityEffect : public OneModeNetworkEffect{

public:
	DensityEffect(size_t networkIndex) : OneModeNetworkEffect(networkIndex){
	}

	double getEffect(ProcessState * processState, int actorIndex){
		MemoryOneModeNetwork * net =
				dynamic_cast<MemoryOneModeNetwork *>(processState->getNetwork(_networkIndex));
		return net->getOutDegree(actorIndex);
	}

	std::string getName(){
		return "Density";// +_networkIndex;
	}

};

class ReciprocityEffect : public OneModeNetworkEffect{

public:
	ReciprocityEffect(size_t networkIndex) : OneModeNetworkEffect(networkIndex){
	}

	double getEffect(ProcessState * processState, int actorIndex){
		MemoryOneModeNetwork * net =
				dynamic_cast<MemoryOneModeNetwork *>(processState->getNetwork(_networkIndex));
		return net->getReciprocalNeighbors(actorIndex).size();
	}

	std::string getName(){
		return "Reciprocity";
	}


};


class TransitivityEffect : public OneModeNetworkEffect{

	TransitivityEffect(size_t networkIndex) : OneModeNetworkEffect(networkIndex){
	}

	double getEffect(ProcessState * processState, int actorIndex){
		double value = 0;
		MemoryOneModeNetwork * net =
				dynamic_cast<MemoryOneModeNetwork *>(processState->getNetwork(_networkIndex));

		// get all neighbors
		std::set<int> neighbors = net->getOutgoingNeighbors(actorIndex);
		// for each neighbor count the number of two-paths also leading there
		for (std::set<int>::iterator itNeighbors = neighbors.begin(); itNeighbors != neighbors.end(); ++itNeighbors){
			int nTransitiveTriads = net->getIntermediateNodesForward(actorIndex, (*itNeighbors)).size();
			value += (double) nTransitiveTriads;
		}

		return value;
	}

	std::string getName(){
		return "Transitivity";
	}

};

class ThreeCircleEffect : public OneModeNetworkEffect{

	ThreeCircleEffect(size_t networkIndex) : OneModeNetworkEffect(networkIndex){
	}

	double getEffect(ProcessState * processState, int actorIndex){
		double value = 0;
		MemoryOneModeNetwork * net =
				dynamic_cast<MemoryOneModeNetwork *>(processState->getNetwork(_networkIndex));

		// get all incoming neighbors
		std::set<int> neighbors = net->getIncomingNeighbors(actorIndex);
		// for each neighbor count the number of two-paths also leading there
		for (std::set<int>::iterator itNeighbors = neighbors.begin(); itNeighbors != neighbors.end(); ++itNeighbors){
			int nTransitiveTriads = net->getIntermediateNodesForward(actorIndex, (*itNeighbors)).size();
			value += (double) nTransitiveTriads;
		}

		return value;
	}

	std::string getName(){
		return "Three-circle";
	}

};


class TwoPathEffect : public OneModeNetworkEffect{

	TwoPathEffect(size_t networkIndex) : OneModeNetworkEffect(networkIndex){
	}

	double getEffect(ProcessState * processState, int actorIndex){
		double value = 0;
		MemoryOneModeNetwork * net =
				dynamic_cast<MemoryOneModeNetwork *>(processState->getNetwork(_networkIndex));

		std::set<int> distanceTwo = net->getNodesInDistanceTwo(actorIndex);
		// delete all neighbored nodes
		distanceTwo.erase(
				net->getOutgoingNeighbors(actorIndex).begin(),
				net->getOutgoingNeighbors(actorIndex).end());

		return (double) distanceTwo.size();
	}

	std::string getName(){
		return "Two Path";
	}

};

class InPopularity : public OneModeNetworkEffect{

	InPopularity(size_t networkIndex) : OneModeNetworkEffect(networkIndex){
	}

	double getEffect(ProcessState * processState, int actorIndex){
		double value = 0;
		MemoryOneModeNetwork * net =
				dynamic_cast<MemoryOneModeNetwork *>(processState->getNetwork(_networkIndex));

		return (double) net->getInDegree(actorIndex);
	}

	std::string getName(){
		return "In-popularity";
	}

};

class OutPopularity : public OneModeNetworkEffect{

	OutPopularity(size_t networkIndex) : OneModeNetworkEffect(networkIndex){
	}

	double getEffect(ProcessState * processState, int actorIndex){
		double value = 0;
		MemoryOneModeNetwork * net =
				dynamic_cast<MemoryOneModeNetwork *>(processState->getNetwork(_networkIndex));

		return (double) net->getOutDegree(actorIndex);
	}

	std::string getName(){
		return "Out-popularity";
	}

};

#endif /* NETWORKEFFECT_H_ */
