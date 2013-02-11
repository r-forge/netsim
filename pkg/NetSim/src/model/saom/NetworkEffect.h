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
 *
 * TODO: consider removing any string naming from internal
 * effect representation
 */
class SaomEffect {

public:
	virtual ~SaomEffect() { }
	virtual double getEffect(ProcessState * processState, int actorIndex) = 0;
	/**
	 * getEffectContribution is implemented as a fast lookup that
	 * can be run after an initial getEffect().
	 * It returns the value that has to be subtracted from the
	 * effect once all corresponding tie updates are applied.
	 *
	 * It assumes that the effect can be reconstructed by iterating over
	 * all ties (using hasTie) and then summing up the effect contributions.
	 */
	virtual double getEffectContribution(ProcessState * processState,
			int actorIndex1, int actorIndex2) = 0;
	virtual std::string getName() = 0;

};

class OneModeNetworkEffect : public SaomEffect{

public:
	OneModeNetworkEffect(size_t networkIndex);

	std::string getClassName();

protected:
	size_t _networkIndex;

};


class DensityEffect : public OneModeNetworkEffect{

public:
	DensityEffect(size_t networkIndex);

	double getEffect(ProcessState * processState, int actorIndex);

	double getEffectContribution(ProcessState * processState,
				int actorIndex1, int actorIndex2);

	std::string getName();

};

class ReciprocityEffect : public OneModeNetworkEffect{

public:
	ReciprocityEffect(size_t networkIndex);

	double getEffect(ProcessState * processState, int actorIndex);

	double getEffectContribution(ProcessState * processState,
				int actorIndex1, int actorIndex2);

	std::string getName();


};


class TransitivityEffect : public OneModeNetworkEffect{

public:
	TransitivityEffect(size_t networkIndex);

	double getEffect(ProcessState * processState, int actorIndex);

	double getEffectContribution(ProcessState * processState,
				int actorIndex1, int actorIndex2);

	std::string getName();

};

class ThreeCycleEffect : public OneModeNetworkEffect{

public:
	ThreeCycleEffect(size_t networkIndex);

	double getEffect(ProcessState * processState, int actorIndex);

	double getEffectContribution(ProcessState * processState,
				int actorIndex1, int actorIndex2);

	std::string getName();

};


class TwoPathEffect : public OneModeNetworkEffect{

public:
	TwoPathEffect(size_t networkIndex);

	double getEffect(ProcessState * processState, int actorIndex);

	double getEffectContribution(ProcessState * processState,
				int actorIndex1, int actorIndex2);

	std::string getName();

};

class InPopularityEffect : public OneModeNetworkEffect{

public:
	InPopularityEffect(size_t networkIndex);

	double getEffect(ProcessState * processState, int actorIndex);

	double getEffectContribution(ProcessState * processState,
				int actorIndex1, int actorIndex2);

	std::string getName();

};

class OutPopularityEffect : public OneModeNetworkEffect{

public:
	OutPopularityEffect(size_t networkIndex);

	double getEffect(ProcessState * processState, int actorIndex);

	double getEffectContribution(ProcessState * processState,
				int actorIndex1, int actorIndex2);

	std::string getName();

};

class AttributeEffect : public SaomEffect{

public:
	AttributeEffect(size_t attributeIndex);

	std::string getClassName();


protected:
	size_t _attributeIndex;

};

class LinearShapeAttributeEffect : public AttributeEffect{

public:
	LinearShapeAttributeEffect(size_t attributeIndex);

	double getEffect(ProcessState * processState, int actorIndex);

	double getEffectContribution(ProcessState * processState,
				int actorIndex1, int actorIndex2);

	std::string getName();


};

class QuadraticShapeAttributeEffect : public AttributeEffect{

public:
	QuadraticShapeAttributeEffect(size_t attributeIndex);

	double getEffect(ProcessState * processState, int actorIndex);

	double getEffectContribution(ProcessState * processState,
				int actorIndex1, int actorIndex2);

	std::string getName();


};


class AttributeOneModeNetworkEffect : public AttributeEffect, public OneModeNetworkEffect{
public:
	AttributeOneModeNetworkEffect(size_t attributeIndex, size_t networkIndex);
	std::string getClassName();

};

class EgoXEffect : public AttributeOneModeNetworkEffect{

public:
	EgoXEffect(size_t attributeIndex, size_t networkIndex);

	double getEffect(ProcessState * processState, int actorIndex);

	double getEffectContribution(ProcessState * processState,
				int actorIndex1, int actorIndex2);

	std::string getName();

};




class AltXEffect : public AttributeOneModeNetworkEffect{

public:
	AltXEffect(size_t attributeIndex, size_t networkIndex);

	double getEffect(ProcessState * processState, int actorIndex);

	double getEffectContribution(ProcessState * processState,
				int actorIndex1, int actorIndex2);

	std::string getName();
};

/**
 * All similarity related effects share empirical
 * mean of similarity scores based (see SIENA manual p.105, effect 40 "simX")
 */
class SimilarityAttributeOneModeNetworkEffect : public AttributeOneModeNetworkEffect{
public:
	SimilarityAttributeOneModeNetworkEffect(size_t attributeIndex, size_t networkIndex, double meanSimilarityScores);

protected:
	double _meanSimilarityScores;
};

/**
 * The total similarity effect is an implementation of simX and totSim.
 * These effects only differ by the dependent variable (network vs. attribute)
 */
class TotalSimilarityEffect : public SimilarityAttributeOneModeNetworkEffect{

public:
	TotalSimilarityEffect(size_t attributeIndex, size_t networkIndex, double meanSimilarityScores);

	double getEffectContribution(ProcessState * processState,
				int actorIndex1, int actorIndex2);

	double getEffect(ProcessState * processState, int actorIndex);

	std::string getName();
};


#endif /* NETWORKEFFECT_H_ */
