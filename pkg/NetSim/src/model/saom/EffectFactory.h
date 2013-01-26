/*
 * EffectFactory.h
 *
 *  Created on: Jan 22, 2013
 *      Author: cws
 */

#ifndef EFFECTFACTORY_H_
#define EFFECTFACTORY_H_

#include "NetworkEffect.h"
#include <stdexcept>
#include <string>

/**
 * Factory to create SAOM effects by name.
 * Names are according to the naming conventions
 * in SIENA (see SIENA manual p. 100ff)
 */
class EffectFactory{

public:

	OneModeNetworkEffect * getOneModeNetworkEffect(
			std::string name, size_t networkIndex);

	AttributeOneModeNetworkEffect * getAttributeOneModeNetworkEffect(
			std::string name, size_t attributeIndex, size_t networkIndex);

};


#endif /* EFFECTFACTORY_H_ */
