/*
 * NetworkEffect.cpp
 *
 *  Created on: Jan 17, 2013
 *      Author: cws
 */

#include "NetworkEffect.h"

OneModeNetworkEffect::OneModeNetworkEffect(size_t networkIndex) {
	_networkIndex = networkIndex;
}

DensityEffect::DensityEffect(size_t networkIndex) : OneModeNetworkEffect(networkIndex){
}

double DensityEffect::getEffect(ProcessState* processState, int actorIndex) {
	MemoryOneModeNetwork * net =
			dynamic_cast<MemoryOneModeNetwork *>(processState->getNetwork(_networkIndex));
	return net->getOutDegree(actorIndex);
}

std::string DensityEffect::getName() {
	return "Density";// +_networkIndex;
}

ReciprocityEffect::ReciprocityEffect(size_t networkIndex) : OneModeNetworkEffect(networkIndex){
}

double ReciprocityEffect::getEffect(ProcessState* processState,
		int actorIndex) {
	MemoryOneModeNetwork * net =
			dynamic_cast<MemoryOneModeNetwork *>(processState->getNetwork(_networkIndex));
	return net->getReciprocalNeighbors(actorIndex).size();
}


std::string ReciprocityEffect::getName() {
	return "Reciprocity";
}

TransitivityEffect::TransitivityEffect(size_t networkIndex)  : OneModeNetworkEffect(networkIndex){
}

double TransitivityEffect::getEffect(ProcessState* processState,
		int actorIndex) {
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

std::string TransitivityEffect::getName() {
	return "Transitivity";
}

ThreeCircleEffect::ThreeCircleEffect(size_t networkIndex) : OneModeNetworkEffect(networkIndex){
}

double ThreeCircleEffect::getEffect(ProcessState* processState,
		int actorIndex) {
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

std::string ThreeCircleEffect::getName() {
	return "Three-circle";
}

TwoPathEffect::TwoPathEffect(size_t networkIndex) : OneModeNetworkEffect(networkIndex){
}

double TwoPathEffect::getEffect(ProcessState* processState, int actorIndex) {
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

std::string TwoPathEffect::getName() {
	return "Two Path";
}

InPopularity::InPopularity(size_t networkIndex)  : OneModeNetworkEffect(networkIndex){
}

double InPopularity::getEffect(ProcessState* processState, int actorIndex) {
	double value = 0;
	MemoryOneModeNetwork * net =
			dynamic_cast<MemoryOneModeNetwork *>(processState->getNetwork(_networkIndex));

	return (double) net->getInDegree(actorIndex);
}

std::string InPopularity::getName() {
	return "In-popularity";
}

OutPopularity::OutPopularity(size_t networkIndex)  : OneModeNetworkEffect(networkIndex){
}

double OutPopularity::getEffect(ProcessState* processState, int actorIndex) {
	double value = 0;
	MemoryOneModeNetwork * net =
			dynamic_cast<MemoryOneModeNetwork *>(processState->getNetwork(_networkIndex));

	return (double) net->getOutDegree(actorIndex);
}

std::string OutPopularity::getName() {
	return "Out-popularity";
}
