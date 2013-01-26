/*
 * EffectFactory.cpp
 *
 *  Created on: Jan 22, 2013
 *      Author: cws
 */

#include "EffectFactory.h"

OneModeNetworkEffect * EffectFactory::getOneModeNetworkEffect(
		std::string name, size_t networkIndex) {

	// effects ordered according to the SIENA manual

	if(name == "density")
		return new DensityEffect(networkIndex);

	if(name == "recip")
		return new ReciprocityEffect(networkIndex);

	if(name == "transTrip")
		return new TransitivityEffect(networkIndex);

	if(name == "cycle3")
		return new ThreeCycleEffect(networkIndex);

	if(name == "nbrDist2")
		return new TwoPathEffect(networkIndex);

	if(name == "inPop")
		return new InPopularityEffect(networkIndex);

	if(name == "outPop")
		return new OutPopularityEffect(networkIndex);


	std::string errorMessage = "Unknown one mode network effect: ";
	errorMessage += name;
	throw std::invalid_argument(errorMessage);
}

AttributeOneModeNetworkEffect* EffectFactory::getAttributeOneModeNetworkEffect(
		std::string name, size_t networkIndex, size_t attributeIndex) {

	if(name == "altX")
		;

	if(name == "egoX")
		;


	std::string errorMessage = "Unknown attribute one mode network effect: ";
	errorMessage += name;
	throw std::invalid_argument(errorMessage);
}
