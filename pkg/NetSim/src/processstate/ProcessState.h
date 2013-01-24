/*
 * ProcessState.h
 *
 *  Created on: Nov 28, 2012
 *      Author: cws
 */

#ifndef PROCESSSTATE_H_
#define PROCESSSTATE_H_

#include <map>
#include <boost/lexical_cast.hpp>
#include <stdexcept>
#include "../network/Network.h"
#include "../attribute/AttributeContainer.h"

/**
 * Defines the process state of the simulated Markov process.
 * A ProcessState is a contains three vector of pointers to Networks,
 * AttributeContainer's and global attributes. Objects are addressed by index.
 */
class ProcessState{

public:

	ProcessState();

	/**
	 * Add a network pointer to the vector of network pointers
	 * @returns unique index
	 */
	size_t addNetwork(Network* network);

	/**
	 * Add a pointer to an AttributeContainter to the vector of network pointers
	 * @returns unique index
	 */
	size_t addAttributeContainer(AttributeContainer* attributeContainer);

	/**
	 * Add a network pointer to a global attribute
	 * @returns unique index
	 */
	size_t addGlobalAttribute(double* attribute);

	/**
	 * Get network from network stack by index
	 */
	Network* getNetwork(size_t index);

	/**
	 * Get AttributeContainter from attribute container stack by index
	 */
	AttributeContainer* getAttributeContainer(size_t index);

	/**
	 * Get global attribute from attribute stack by index
	 */
	double* getGlobalAttribute(size_t index);

	/**
	 * Get the number of actors represented by the elements of
	 * the process state
	 */
	int getNumberOfActors();

	/**
	 * get number of networks
	 */
	int getNumberOfNetworks();

	/**
	 * get number of attribute containers
	 */
	int getNumberOfAttributeContainers();

	/**
	 * get number of global attributes
	 */
	int getNumberOfGlobalAttributes();


private:
	std::map<int, Network*> _networks;
	std::map<int, AttributeContainer*> _attributeContainers;
	std::map<int, double*> _globalAttributes;
	size_t _nNetworks;
	size_t _nAttributeContainers;
	size_t _nGlobalAttributes;
	size_t _nActors;

};




#endif /* PROCESSSTATE_H_ */
