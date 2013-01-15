/*
 * ProcessState.cpp
 *
 *  Created on: Nov 30, 2012
 *      Author: cws
 */

#include "ProcessState.h"

ProcessState::ProcessState() {
	_nActors = 0;
	// vectors are initialized automatically
}


size_t ProcessState::addNetwork(Network* network) {

	if (network->getSize() > _nActors)
		_nActors = network->getSize();

	_networks.insert(_networks.end(), network);
	return -1 + _networks.end()-_networks.begin();
}

size_t ProcessState::addAttributeContainer(AttributeContainer* attributeContainer) {
	if (attributeContainer->getSize() > _nActors)
		_nActors = attributeContainer->getSize();

	_attributeContainers.insert(_attributeContainers.end(), attributeContainer);
	return -1 + _attributeContainers.end() - _attributeContainers.begin();
}

size_t ProcessState::addGlobalAttribute(double* attribute) {
	_globalAttributes.insert(_globalAttributes.end(), attribute);
	// size_t size = (size_t) _globalAttributes.size();
	return -1 + _globalAttributes.end() - _globalAttributes.begin() ;
}

Network* ProcessState::getNetwork(size_t index) {
	return _networks[index];
}

AttributeContainer* ProcessState::getAttributeContainer(size_t index) {
	return _attributeContainers[index];
}


double* ProcessState::getGlobalAttribute(size_t index) {
	return _globalAttributes[index];
}


int ProcessState::getNumberOfActors() {
	return _nActors;
}
