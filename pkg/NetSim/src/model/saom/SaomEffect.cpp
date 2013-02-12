/*
 * NetworkEffect.cpp
 *
 *  Created on: Jan 17, 2013
 *      Author: cws
 */

#include "SaomEffect.h"

NetworkEffect::NetworkEffect(size_t networkIndex) {
	_networkIndex = networkIndex;
}

DensityEffect::DensityEffect(size_t networkIndex) : NetworkEffect(networkIndex){
}

double DensityEffect::getEffect(ProcessState* processState, int actorIndex) {
	MemoryOneModeNetwork * net =
			dynamic_cast<MemoryOneModeNetwork *>(processState->getNetwork(_networkIndex));
	return net->getOutDegree(actorIndex);
}

double DensityEffect::getEffectContribution(ProcessState* processState,
		int actorIndex1, int actorIndex2) {

	return 1.0;
}

std::string DensityEffect::getName() {
	return "Density";// +_networkIndex;
}

ReciprocityEffect::ReciprocityEffect(size_t networkIndex) : NetworkEffect(networkIndex){
}

double ReciprocityEffect::getEffect(ProcessState* processState,
		int actorIndex) {
	MemoryOneModeNetwork * net =
			dynamic_cast<MemoryOneModeNetwork *>(processState->getNetwork(_networkIndex));
	return net->getReciprocalNeighbors(actorIndex).size();
}

double ReciprocityEffect::getEffectContribution(ProcessState* processState,
		int actorIndex1, int actorIndex2) {
	MemoryOneModeNetwork * net =
			dynamic_cast<MemoryOneModeNetwork *>(processState->getNetwork(_networkIndex));
	if (net->hasTie(actorIndex2, actorIndex1))
		return 1.0;
	else return 0.0;
}

std::string ReciprocityEffect::getName() {
	return "Reciprocity";
}

TransitivityEffect::TransitivityEffect(size_t networkIndex)  : NetworkEffect(networkIndex){
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

double TransitivityEffect::getEffectContribution(ProcessState* processState,
		int actorIndex1, int actorIndex2) {
	MemoryOneModeNetwork * net =
			dynamic_cast<MemoryOneModeNetwork *>(processState->getNetwork(_networkIndex));
	double triadsClosedByTie =
			net->getIntermediateNodesForward(actorIndex1, actorIndex2).size();
	double triadsStartedByTie =
			net->getIntermediateNodesUp(actorIndex2, actorIndex1).size();

	return triadsClosedByTie + triadsStartedByTie;
}

std::string TransitivityEffect::getName() {
	return "Transitivity";
}

ThreeCycleEffect::ThreeCycleEffect(size_t networkIndex) : NetworkEffect(networkIndex){
}

double ThreeCycleEffect::getEffect(ProcessState* processState,
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

double ThreeCycleEffect::getEffectContribution(ProcessState* processState,
		int actorIndex1, int actorIndex2) {
	MemoryOneModeNetwork * net =
			dynamic_cast<MemoryOneModeNetwork *>(processState->getNetwork(_networkIndex));
	return net->getIntermediateNodesBackward(actorIndex1, actorIndex2).size();

}

std::string ThreeCycleEffect::getName() {
	return "Three-circle";
}

TwoPathEffect::TwoPathEffect(size_t networkIndex) : NetworkEffect(networkIndex){
}

double TwoPathEffect::getEffect(ProcessState* processState, int actorIndex) {
	MemoryOneModeNetwork * net =
			dynamic_cast<MemoryOneModeNetwork *>(processState->getNetwork(_networkIndex));

	std::set<int> distanceTwo = net->getNodesInDistanceTwo(actorIndex);
	std::set<int> neighboredNodes = net->getOutgoingNeighbors(actorIndex);

	// delete all neighbored nodes
	// TODO: Is there a faster way?
	for (std::set<int>::iterator it = neighboredNodes.begin();
			it != neighboredNodes.end();
			++it){
		if (distanceTwo.find(*it) != distanceTwo.end())
			distanceTwo.erase(*it);
	}

	return (double) distanceTwo.size();
}

double TwoPathEffect::getEffectContribution(ProcessState* processState,
		int actorIndex1, int actorIndex2) {
	MemoryOneModeNetwork * net =
			dynamic_cast<MemoryOneModeNetwork *>(processState->getNetwork(_networkIndex));
	return - net->getIntermediateNodesForward(actorIndex1, actorIndex2).size();

}

std::string TwoPathEffect::getName() {
	return "Two Path";
}

InPopularityEffect::InPopularityEffect(size_t networkIndex)  : NetworkEffect(networkIndex){
}

double InPopularityEffect::getEffect(ProcessState* processState, int actorIndex) {
	double value = 0;
	MemoryOneModeNetwork * net =
			dynamic_cast<MemoryOneModeNetwork *>(processState->getNetwork(_networkIndex));

	std::set<int> neighbors = net->getOutgoingNeighbors(actorIndex);

	// sum the incoming ties of all neighbors
	for (std::set<int>::iterator it = neighbors.begin(); it != neighbors.end(); ++it){
		value += (double) net->getIncomingNeighbors(*it).size();
	}

	return value;

}

double InPopularityEffect::getEffectContribution(ProcessState* processState,
		int actorIndex1, int actorIndex2) {
	MemoryOneModeNetwork * net =
			dynamic_cast<MemoryOneModeNetwork *>(processState->getNetwork(_networkIndex));
	return net->getInDegree(actorIndex2);

}

std::string InPopularityEffect::getName() {
	return "In-popularity";
}

OutPopularityEffect::OutPopularityEffect(size_t networkIndex)  : NetworkEffect(networkIndex){
}

double OutPopularityEffect::getEffect(ProcessState* processState, int actorIndex) {
	double value = 0;
	MemoryOneModeNetwork * net =
			dynamic_cast<MemoryOneModeNetwork *>(processState->getNetwork(_networkIndex));

	std::set<int> neighbors = net->getOutgoingNeighbors(actorIndex);

	// sum the incoming ties of all neighbors
	for (std::set<int>::iterator it = neighbors.begin(); it != neighbors.end(); ++it){
		value += (double) net->getOutgoingNeighbors(*it).size();
	}

	return value;

}

double OutPopularityEffect::getEffectContribution(ProcessState* processState,
		int actorIndex1, int actorIndex2) {
	MemoryOneModeNetwork * net =
			dynamic_cast<MemoryOneModeNetwork *>(processState->getNetwork(_networkIndex));
	return net->getOutDegree(actorIndex2);

}

std::string OutPopularityEffect::getName() {
	return "Out-popularity";
}

AttributeEffect::AttributeEffect(size_t attributeIndex) {
	_attributeIndex = attributeIndex;
}

AttributeOneModeNetworkEffect::AttributeOneModeNetworkEffect(size_t attributeIndex, size_t networkIndex) :
		AttributeEffect(attributeIndex), NetworkEffect(networkIndex){
}

std::string NetworkEffect::getClassName() {
	return "OneModeNetworkEffect";
}

std::string AttributeEffect::getClassName() {
	return "AttributeEffect";
}

std::string AttributeOneModeNetworkEffect::getClassName() {
	return "AttributeOneModeNetworkEffect";
}

double EgoXEffect::getEffect(ProcessState* processState, int actorIndex) {
	AttributeContainer * attributeContainer =
			processState->getAttributeContainer(_attributeIndex);
	MemoryOneModeNetwork * network =
			dynamic_cast<MemoryOneModeNetwork *>(processState->getNetwork(_networkIndex));

	double value;
	value = attributeContainer->getValue(actorIndex);
	value *= network->getOutgoingNeighbors(actorIndex).size();

	return value;
}

EgoXEffect::EgoXEffect(size_t attributeIndex, size_t networkIndex) :
	AttributeOneModeNetworkEffect(attributeIndex, networkIndex){
}

double EgoXEffect::getEffectContribution(ProcessState* processState,
		int actorIndex1, int actorIndex2) {
	AttributeContainer * attributeContainer =
			processState->getAttributeContainer(_attributeIndex);
	return attributeContainer->getValue(actorIndex1);

}

std::string EgoXEffect::getName() {
	return "egoX";
}

double AltXEffect::getEffect(ProcessState* processState, int actorIndex) {
	AttributeContainer * attributeContainer =
			processState->getAttributeContainer(_attributeIndex);
	MemoryOneModeNetwork * network =
			dynamic_cast<MemoryOneModeNetwork *>(processState->getNetwork(_networkIndex));

	double value;
	std::set<int> neighbors = network->getOutgoingNeighbors(actorIndex);
	std::set<int>::iterator it = neighbors.begin();
	for (; it != neighbors.end(); ++it){
		value += attributeContainer->getValue(*it);
	}

	return value;
}

AltXEffect::AltXEffect(size_t attributeIndex, size_t networkIndex) :
			AttributeOneModeNetworkEffect(attributeIndex, networkIndex){
}

double AltXEffect::getEffectContribution(ProcessState* processState,
		int actorIndex1, int actorIndex2) {
	AttributeContainer * attributeContainer =
			processState->getAttributeContainer(_attributeIndex);
	return attributeContainer->getValue(actorIndex2);

}

std::string AltXEffect::getName() {
	return "altX";
}

SimilarityAttributeOneModeNetworkEffect::SimilarityAttributeOneModeNetworkEffect(
		size_t attributeIndex, size_t networkIndex,
		double meanSimilarityScores = 0.5) : AttributeOneModeNetworkEffect(attributeIndex, networkIndex){
	_meanSimilarityScores = meanSimilarityScores;
}

LinearShapeAttributeEffect::LinearShapeAttributeEffect(size_t attributeIndex) :
	AttributeEffect (attributeIndex){
}

double LinearShapeAttributeEffect::getEffect(ProcessState* processState,
		int actorIndex) {
	AttributeContainer * attributeContainer =
			processState->getAttributeContainer(_attributeIndex);
	return attributeContainer->getValue(actorIndex);
}


std::string LinearShapeAttributeEffect::getName() {
	return "linear shape effect";
}

QuadraticShapeAttributeEffect::QuadraticShapeAttributeEffect(size_t attributeIndex) :
		AttributeEffect (attributeIndex){
}

double QuadraticShapeAttributeEffect::getEffect(ProcessState* processState,
		int actorIndex) {
	AttributeContainer * attributeContainer =
			processState->getAttributeContainer(_attributeIndex);
	// return squared effect
	return pow(attributeContainer->getValue(actorIndex), 2);

}


std::string QuadraticShapeAttributeEffect::getName() {
	return "Quadratic shape effect";
}

TotalSimilarityEffect::TotalSimilarityEffect(size_t attributeIndex,
		size_t networkIndex, double meanSimilarityScores) :
				SimilarityAttributeOneModeNetworkEffect(attributeIndex, networkIndex, meanSimilarityScores){
}

double TotalSimilarityEffect::getEffect(ProcessState* processState,
		int actorIndex) {

	AttributeContainer * attributeContainer =
			processState->getAttributeContainer(_attributeIndex);
	MemoryOneModeNetwork * network =
			dynamic_cast<MemoryOneModeNetwork *>(processState->getNetwork(_networkIndex));

	double value = 0;
	double minAttributeValue = attributeContainer->getMin();
	double maxAttributeValue = attributeContainer->getMax();
	double egoValue = attributeContainer->getValue(actorIndex);
	double range = maxAttributeValue - minAttributeValue;

	std::set<int> neighbors = network->getOutgoingNeighbors(actorIndex);
	std::set<int>::iterator it = neighbors.begin();

	for (; it != neighbors.end(); ++it){
		double alterValue = attributeContainer->getValue(*it);
		double sim = (range - abs(alterValue - egoValue)) / range;
		value += sim - _meanSimilarityScores;
	}

	return value;
}

double TotalSimilarityEffect::getEffectContribution(ProcessState* processState,
		int actorIndex1, int actorIndex2) {
	AttributeContainer * attributeContainer =
			processState->getAttributeContainer(_attributeIndex);

	double minAttributeValue = attributeContainer->getMin();
	double maxAttributeValue = attributeContainer->getMax();
	double egoValue = attributeContainer->getValue(actorIndex1);
	double alterValue = attributeContainer->getValue(actorIndex2);
	double range = maxAttributeValue - minAttributeValue;

	// similarity_ij - mean similarity scores
	double sim = (range - abs(alterValue - egoValue)) / range;
	return sim = _meanSimilarityScores;
}

std::string TotalSimilarityEffect::getName() {
	return "Total similarity";
}
