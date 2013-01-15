/*
 * ProcessStateManager.cpp
 *
 *  Created on: Nov 30, 2012
 *      Author: cws
 */

# include "ProcessStateManager.h"

ProcessStateManager::ProcessStateManager(std::string name) {
	_processState = new ProcessState();
	_name = name;
}

void ProcessStateManager::addNetwork(Network* network, std::string name) {
	size_t index = _processState->addNetwork(network);
	_networkNameIndexMap.insert( std::pair<std::string, size_t>(name, index) );
}

void ProcessStateManager::addAttributeContainter(
		AttributeContainer* attributeContainer, std::string name) {
	size_t index = _processState->addAttributeContainer(attributeContainer);
	_attributeContainerNameIndexMap.insert(
			std::pair<std::string, size_t>(name, index));
}

void ProcessStateManager::addGlobalAttribute(double* attribute,
		std::string name) {
	size_t index = _processState->addGlobalAttribute(attribute);
	_globalAttributeNameIndexMap.insert(
			std::pair<std::string, size_t>(name, index));
}

Network* ProcessStateManager::getNetwork(std::string name) {
	size_t index = _networkNameIndexMap[name];
	return _processState->getNetwork(index);
}

AttributeContainer* ProcessStateManager::getAttributeContainer(
		std::string name) {
	size_t index = _attributeContainerNameIndexMap[name];
	return _processState->getAttributeContainer(index);
}

double* ProcessStateManager::getGlobalAttribute(std::string name) {
	size_t index = _globalAttributeNameIndexMap[name];
	return _processState->getGlobalAttribute(index);
}

std::string ProcessStateManager::getName() {
	return _name;
}
