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
	// TODO this part can be more flexible
	// e.g. only use "active" actors as potential receivers
	std::set<int> actorIDs = processState->getActorIds();
	std::set<int>::iterator itActors;
	std::map<int, int> indexIdMap;
	int nActors = actorIDs.size();

	std::vector<double> objectiveFunctions(nActors, 0);
	double denominator = 0;
	// DEPRECATED
	// updates objectiveFunctions and denominators
	// calculateTieContributionsWithMemento(nActors,
	//		objectiveFunctions, denominator, processState);
	// calculate effects for the current networks (un-unchanged)
	int nEffects = _effectParameterPairs->size();
	std::vector<std::pair<double, double> > statisticsAndParametersWithoutChange(
			nEffects, std::pair<double, double>(0, 0));
	std::vector<std::vector<double> > statisticsWithoutTieContribution(
			nActors, std::vector<double>(nEffects, 0));
	// calculate statistics
	int effectIndex = 0;
	std::set<std::pair<SaomEffect*, double> *>::iterator itEffects =
			_effectParameterPairs->begin();
	for (; itEffects != _effectParameterPairs->end(); ++itEffects) {
		int i = _actorIndex;
		// calculate effects without changes
		// save effect statistic
		double statistic = (*itEffects)->first->getEffect(processState,
				_actorIndex);
		statisticsAndParametersWithoutChange[effectIndex].first +=
				statistic;
		// save effect parameter (beta)
		double parameter = (*itEffects)->second;
		statisticsAndParametersWithoutChange[effectIndex].second =
				parameter;

		// iterate over all actors
		itActors = actorIDs.begin();
		int actorIndex = 0; // the internal array index pointing to  the actor ID
		for (; itActors != actorIDs.end(); ++itActors) {
			int j = *itActors;
			// in the first iteration do create the map
			if (effectIndex == 0)
				indexIdMap[actorIndex] = j;

			NetworkEffect* effect =
					dynamic_cast<NetworkEffect*>((*itEffects)->first);
			double contribution = effect->getEffectContribution(
					processState, i, j);
			Network* net = processState->getNetwork(_dependentNetworkIndex);
			// TODO: Here, we assume that an update is a tie swap!
			if (net->hasTie(i, j)) {
				statisticsWithoutTieContribution[actorIndex][effectIndex] =
						statistic - contribution;
				if (_debug)
					std::cout << "Tie removal from " << i << " to " << j
							<< " contributes " << contribution << std::endl;
			} else {
				// i->j does not exist or i == j
				if (i != j) {
					statisticsWithoutTieContribution[actorIndex][effectIndex] =
							statistic + contribution;
					if (_debug)
						std::cout << "Tie insertion from " << i << " to "
								<< j << " contributes " << contribution
								<< std::endl;
				} else // choosing oneself means changing nothing
				{
					statisticsWithoutTieContribution[actorIndex][effectIndex] =
							statistic;
				}
			}
			objectiveFunctions[actorIndex] +=
					statisticsWithoutTieContribution[actorIndex][effectIndex]
							* parameter;
			actorIndex++;
		} // iterate over all actors
		++effectIndex;
	}

	// denominator
	for (size_t i = 0; i < objectiveFunctions.size(); i++) {
		denominator += exp(objectiveFunctions[i]);
		if (_debug)
			std::cout << "o.f. of actor choice " << i << ": "
					<< objectiveFunctions[i] << std::endl;
	}

	if (_debug)
		std::cout << "Active actor: " << _actorIndex << std::endl;

	if (_debug)
		std::cout << "calculated denominator: " << denominator << std::endl;

	// draw random number
	double randomNumber = Random::getInstance().getRandom();
	// iterate over objective functions until actor is found
	double cumProbability = 0;
	for (int actorIndex = 0; actorIndex < nActors; actorIndex++) {
		double probability = exp(objectiveFunctions[actorIndex]) / denominator;
		cumProbability += probability;
		// indexIdMap[k] is the randomly chosen actor
		// create and return ActorModelResult
		if (cumProbability >= randomNumber) {
			// TieModelResult result(_actorIndex,j);
			return new TieModelResult(_actorIndex, indexIdMap[actorIndex]);
			// return &_tieModelResult;
		}
	}
	// Should never be reached except an error occurs
	std::cout
			<< "Should not be here MultinomialChoiceModel wrong return type";
	return new TieModelResult(-1, -1);
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
