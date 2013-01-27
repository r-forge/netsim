/*
 * MultinomialChoiceChangeModel.h
 *
 *  Created on: Dec 9, 2012
 *      Author: cws
 */

#ifndef MULTINOMIALCHOICENETWORKCHANGEMODEL_H_
#define MULTINOMIALCHOICENETWORKCHANGEMODEL_H_

#include <utility>
#include <math.h>
#include <iostream>
#include "MultinomialChoiceUtils.h"
#include "ChangeModel.h"
#include "saom/NetworkEffect.h"
#include "Updater.h"
#include "ModelResult.h"
#include "../utils/Random.h"

class MultinomialChoiceNetworkChangeModel : public ChangeModel{

public:
	/**
	 * Create a simple model in which choices are based on how
	 * the network would look like after a number of updates
	 */
	MultinomialChoiceNetworkChangeModel(int actorIndex,
			size_t dependentNetworkIndex,
			std::set<std::pair<SaomEffect*, double> *> * effectParameterPairs,
			std::vector<Updater*>* updaters);

	/**
	 * create a multinomial choice model in which the choice
	 * dependes on a process state without changes
	 */
	MultinomialChoiceNetworkChangeModel(int actorIndex,
			size_t dependentNetworkIndex,
			std::set<std::pair<SaomEffect*, double> *> * effectParameterPairs);


	/**
	 * get actor for which the change is to be applied.
	 */
	ModelResult * getChange(ProcessState * processState);

	/**
	 * If set, extra debug messages will be sent to the console
	 */
	void setDebugMode(bool verbose = true);


private:
	int _actorIndex;
	size_t _dependentNetworkIndex;
	std::set<std::pair<SaomEffect*, double> *> * _effectParameterPairs;
	// std::vector<NetworkEffect*> * _effects; // Effect container
	// std::vector<double> * _parameters;
	std::vector<Updater*> * _updaters;
	bool _debug;

	void init(int actorIndex,
			size_t dependentNetworkIndex,
			std::set<std::pair<SaomEffect*, double> *> * effectParameterPairs,
			std::vector<Updater*>* updaters);


};


#endif /* MULTINOMIALCHOICENETWORKCHANGEMODEL_H_ */
