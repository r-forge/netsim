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

void MultinomialChoiceNetworkChangeModel::calculateTieContributionsWithMemento(
		int nActors, const std::vector<double>& objectiveFunctions,
		double denominator, ProcessState* processState) {
/*

	// save process state as memento
	ProcessStateMemento* memento = processState->saveToMemento();

	// iterate over all actors
	for (int j = 0; j < nActors; j++) {
		int i = _actorIndex;
		// update tie.
		for (std::vector<Updater*>::iterator it = _updaters->begin();
				it != _updaters->end(); ++it) {
			TieModelResult* result = new TieModelResult(i, j);
			(*it)->update(processState, result);
			delete result;
		}
		// calculate and save objective function
		objectiveFunctions[j] =
				MultinomialChoiceUtils::getValueObjectiveFunction(processState,
						i, _effectParameterPairs, _debug);
		// increase sum (denominator)
		denominator += exp(objectiveFunctions[j]);
		if (_debug)
			std::cout << "" << objectiveFunctions[j] << std::endl;

		// reset process state
		processState->restoreFromMemento(memento);
	} // iterate over all actors
	delete memento;
*/
}

ModelResult* MultinomialChoiceNetworkChangeModel::getChange(
		ProcessState* processState) {

	int nActors = processState->getNumberOfActors();
	std::vector<double> objectiveFunctions(nActors, 0);
	double denominator = 0;

	// updates objectiveFunctions and denominators
	// calculateTieContributionsWithMemento(nActors,
	//		objectiveFunctions, denominator, processState);

	// calculate effects for the current networks (un-unchanged)
	int nEffects = _effectParameterPairs->size();
	std::vector<std::pair<double, double> > statisticsAndParametersWithoutChange(
			nEffects, std::pair<double, double>(0,0));
	std::vector<std::vector<double> > statisticsWithoutTieContribution (nActors,
			std::vector<double>(nEffects, 0));


	// calculate statistics
	int effectIndex = 0;
	std::set<std::pair<SaomEffect*, double> *>::iterator itEffects =
			_effectParameterPairs->begin();
	for (; itEffects != _effectParameterPairs->end(); ++itEffects){

		int i = _actorIndex;

		// calculate effects without changes
		// save effect statistic
		double statistic = (*itEffects)->first->getEffect(processState, _actorIndex);
		statisticsAndParametersWithoutChange[effectIndex].first += statistic;

		// save effect parameter (beta)
		double parameter = (*itEffects)->second;
		statisticsAndParametersWithoutChange[effectIndex].second =
				parameter;

		// iterate over all actors
		for (int j = 0; j < nActors; j++) {

			NetworkEffect * effect = dynamic_cast<NetworkEffect *>((*itEffects)->first);
			double contribution =
					effect->getEffectContribution(processState, i, j);

			Network * net = processState->getNetwork(_dependentNetworkIndex);
			// TODO: Here, we assume that an update is a tie swap!
			if (net->hasTie(i, j)){

				statisticsWithoutTieContribution[j][effectIndex] =
						statistic - contribution;

			} else{

				if (i != j){
					statisticsWithoutTieContribution[j][effectIndex] =
							statistic + contribution;
				}
				// choosing oneself means changing nothing
				else{
					statisticsWithoutTieContribution[j][effectIndex] =
							statistic;
				}


			}

			objectiveFunctions[j] +=
					statisticsWithoutTieContribution[j][effectIndex] * parameter;


		}  // iterate over all actors

		++effectIndex;
	}

	for (int i = 0; i < objectiveFunctions.size(); i++){
		if (_debug) std::cout << "o.f. of actor "<< i<< ": " <<
				objectiveFunctions[i] << std::endl;
		denominator += exp(objectiveFunctions[i]);
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
			// TieModelResult result(_actorIndex,j);
			return new TieModelResult(_actorIndex,j);
			// return &_tieModelResult;
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
