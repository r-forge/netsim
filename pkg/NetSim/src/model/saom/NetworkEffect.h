/*
 * NetworkEffect.h
 *
 *  Created on: Dec 9, 2012
 *      Author: cws
 */

#ifndef NETWORKEFFECT_H_
#define NETWORKEFFECT_H_

#include "../../processstate/ProcessState.h"
#include "../../network/MemoryOneModeNetwork.h"

/**
 * Abstract class of network effects to be used in
 * multinomial choice models
 */
class SaomEffect {

public:
	virtual ~SaomEffect() { }
	virtual double getEffect(ProcessState * processState, int actorIndex) = 0;
	virtual std::string getName() = 0;

};

class OneModeNetworkEffect : public SaomEffect{

public:
	OneModeNetworkEffect(size_t networkIndex);

protected:
	size_t _networkIndex;

};


class DensityEffect : public OneModeNetworkEffect{

public:
	DensityEffect(size_t networkIndex);

	double getEffect(ProcessState * processState, int actorIndex);

	std::string getName();

};

class ReciprocityEffect : public OneModeNetworkEffect{

public:
	ReciprocityEffect(size_t networkIndex);

	double getEffect(ProcessState * processState, int actorIndex);

	std::string getName();


};


class TransitivityEffect : public OneModeNetworkEffect{

public:
	TransitivityEffect(size_t networkIndex);

	double getEffect(ProcessState * processState, int actorIndex);

	std::string getName();

};

class ThreeCycleEffect : public OneModeNetworkEffect{

public:
	ThreeCycleEffect(size_t networkIndex);

	double getEffect(ProcessState * processState, int actorIndex);

	std::string getName();

};


class TwoPathEffect : public OneModeNetworkEffect{

public:
	TwoPathEffect(size_t networkIndex);

	double getEffect(ProcessState * processState, int actorIndex);

	std::string getName();

};

class InPopularityEffect : public OneModeNetworkEffect{

public:
	InPopularityEffect(size_t networkIndex);

	double getEffect(ProcessState * processState, int actorIndex);

	std::string getName();

};

class OutPopularityEffect : public OneModeNetworkEffect{

public:
	OutPopularityEffect(size_t networkIndex);

	double getEffect(ProcessState * processState, int actorIndex);

	std::string getName();

};

#endif /* NETWORKEFFECT_H_ */
