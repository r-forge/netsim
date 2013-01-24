/*
 * ProcessState.cpp
 *
 *  Created on: Nov 30, 2012
 *      Author: cws
 */

#include "ProcessState.h"

ProcessState::ProcessState() {
	_nActors = 0;
	_nNetworks = 0;
	_nAttributeContainers = 0;
	_nGlobalAttributes = 0;
	// vectors are initialized automatically
}


size_t ProcessState::addNetwork(Network* network) {

	if (network->getSize() > _nActors)
		_nActors = network->getSize();

	_networks[_nNetworks] = network;
	return _nNetworks++;

	// _networks.insert(_networks.end(), network);
	// return -1 + _networks.end()-_networks.begin();
}

size_t ProcessState::addAttributeContainer(AttributeContainer* attributeContainer) {
	if (attributeContainer->getSize() > _nActors)
		_nActors = attributeContainer->getSize();

	_attributeContainers[_nAttributeContainers] = attributeContainer;
	return _nAttributeContainers++;

	// _attributeContainers.insert(_attributeContainers.end(), attributeContainer);
	// return -1 + _attributeContainers.end() - _attributeContainers.begin();
}

size_t ProcessState::addGlobalAttribute(double* attribute) {

	_globalAttributes[_nGlobalAttributes] = attribute;
	return _nGlobalAttributes++;

	// _globalAttributes.insert(_globalAttributes.end(), attribute);
	// return -1 + _globalAttributes.end() - _globalAttributes.begin() ;
}

Network* ProcessState::getNetwork(size_t index) {
	if ((index >= _nNetworks))
		throw std::invalid_argument("Network index does not exist: "
				+ boost::lexical_cast<std::string>(index));
	return _networks[index];
}

AttributeContainer* ProcessState::getAttributeContainer(size_t index) {
	if ((index >= _nAttributeContainers))
		throw std::invalid_argument("Attribute container index does not exist: "
				+ boost::lexical_cast<std::string>(index));
	return _attributeContainers[index];
}


double* ProcessState::getGlobalAttribute(size_t index) {
	if ((index >= _nGlobalAttributes))
		throw std::invalid_argument("Global attribute index does not exist: "
				+ boost::lexical_cast<std::string>(index));
	return _globalAttributes[index];
}


int ProcessState::getNumberOfActors() {
	return _nActors;
}

int ProcessState::getNumberOfNetworks() {
	return _nNetworks;
}

int ProcessState::getNumberOfAttributeContainers() {
	return _nAttributeContainers;
}

int ProcessState::getNumberOfGlobalAttributes() {
	return _nGlobalAttributes;
}
