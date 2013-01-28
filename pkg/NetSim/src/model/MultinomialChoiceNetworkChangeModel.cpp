/*
 * MultinomialChoiceNetworkChangeModel.cpp
 *
 *  Created on: Jan 17, 2013
 *      Author: cws
 */

#include "MultinomialChoiceNetworkChangeModel.h"

MultinomialChoiceNetworkChangeModel::MultinomialChoiceNetworkChangeModel(
		int actorIndex, size_t dependentNetworkIndex,
		std::set<std::pair<SaomEffect*, double> *>* effectParameterPairs,
		std::vector<Updater*>* updaters) {

	init(actorIndex, dependentNetworkIndex, effectParameterPairs, updaters);
}

MultinomialChoiceNetworkChangeModel::MultinomialChoiceNetworkChangeModel(
		int actorIndex, size_t dependentNetworkIndex,
		std::set<std::pair<SaomEffect*, double> *>* effectParameterPairs) {

	std::vector<Updater*>* updaters = new std::vector<Updater*>();
	init(actorIndex, dependentNetworkIndex, effectParameterPairs, updaters);
}

ModelResult* MultinomialChoiceNetworkChangeModel::getChange(
		ProcessState* processState) {

	int nActors = processState->getNumberOfActors();
	std::vector<double> objectiveFunctions(nActors, 0);
	double denominator = 0;

	// iterate over all actors
	for (int j = 0; j < nActors; j++){

		int i = _actorIndex;

		// update tie.
			for (std::vector<Updater*>::iterator it = _updaters->begin(); it != _updaters->end(); ++it){
				(*it)->update(processState, new TieModelResult(i,j)); // TODO check i, j
			}

		// calculate and save objective function
			objectiveFunctions[j] =
					MultinomialChoiceUtils::getValueObjectiveFunction(processState, i, _effectParameterPairs, _debug);
					//getValueObjectiveFunction(processState, i); // processState has been updated previously

		// increase sum (denominator)
			denominator += exp(objectiveFunctions[j]);

			if (_debug) std::cout << "" << objectiveFunctions[j] << std::endl;

		// reset process state
		// So far, this is implemented as a swap back
		// Performance can be improved by implementing a
			// Memento mechanism in the process state
			for (std::vector<Updater*>::iterator it = _updaters->begin(); it != _updaters->end(); ++it){
				(*it)->undoUpdate(processState, new TieModelResult(i,j)); // TODO check i, j
			}
	}

	if (_debug) std::cout << "Active actor: " << _actorIndex << std::endl;
	if (_debug) std::cout << "calculated denominator: " << denominator << std::endl;

	// draw random number
	double randomNumber = Random::getInstance().getRandom();

	// iterate over objective functions until actor is found
	double cumProbability = 0;
	for (int j = 0; j < nActors; j++){
		double probability = exp(objectiveFunctions[j]) / denominator;
		cumProbability += probability;

		// j is the randomly chosen actor
		// create and return ActorModelResult
		if (cumProbability >= randomNumber){
			return new TieModelResult(_actorIndex,j);
		}
	}

	// Should never be reached except an error occurs
	std::cout << "Should not be here MultinomialChoiceModel wrong return type";
	return new TieModelResult(-1,-1);


}

void MultinomialChoiceNetworkChangeModel::setDebugMode(bool verbose) {
	_debug = verbose;
}

void MultinomialChoiceNetworkChangeModel::init(int actorIndex,
		size_t dependentNetworkIndex,
		std::set<std::pair<SaomEffect*, double> *>* effectParameterPairs,
		std::vector<Updater*>* updaters) {

	_actorIndex = actorIndex;
	_dependentNetworkIndex = dependentNetworkIndex;
	_effectParameterPairs = effectParameterPairs;
	_updaters = updaters;
	_debug = false;

}

EmptyMultinomialChoiceNetworkChangeModel::EmptyMultinomialChoiceNetworkChangeModel() {
}

ModelResult* EmptyMultinomialChoiceNetworkChangeModel::getChange(
		ProcessState* processState) {
	return new ActorModelResult(-1);
}
