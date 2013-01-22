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
	_effectContainer.insert(new std::pair<SaomEffect*, double>(
			effect, parameter));
}
