/*
 * EffectContainerManager.cpp
 *
 *  Created on: Jan 22, 2013
 *      Author: cws
 */

#include "EffectContainerManager.h"

EffectContainerManager::EffectContainerManager() {
}

std::set<std::pair<SaomEffect*, double> *>* EffectContainerManager::getEffectContainer() {
	return &_effectContainer;
}

void EffectContainerManager::addToEffectContainer(
		SaomEffect* effect, double parameter) {

	// check if effect is already included. If so, erase it first.
	std::set<std::pair<SaomEffect*, double> *>::iterator it =
			_effectContainer.begin();
	for (; it != _effectContainer.end(); ++it){
		if ( (*it)->first->getName() == effect->getName()){
			_effectContainer.erase(*it);
			std::cout << "Replacing effect " << effect->getName() <<
					". New value: " << parameter << std::endl;
		}
	}

	// if parameter value is zero then do not include effect
	if (parameter != 0)
	_effectContainer.insert(new std::pair<SaomEffect*, double>(
			effect, parameter));
}
