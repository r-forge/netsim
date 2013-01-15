/*
 * Updater.h
 *
 *  Created on: Nov 29, 2012
 *      Author: cws
 */

#ifndef UPDATER_H_
#define UPDATER_H_

#include <math.h>
#include <float.h>
#include <exception>

#include "ModelResult.h"
#include "../processstate/ProcessState.h"
#include "../network/Network.h"
#include "../network/MemoryOneModeNetwork.h"
#include "../model/TimeModel.h"

class Updater{
public:
	virtual ~Updater() { }
	virtual void update(ProcessState* processState, ModelResult* result) = 0;
	virtual void undoUpdate(ProcessState* processState, ModelResult* result) = 0;

};

class ActorUpdater : public Updater{
public:
	virtual void update(ProcessState * processState, ModelResult * result){
		ActorModelResult* actorResult = dynamic_cast<ActorModelResult*>(result);
		updateActor(processState, actorResult);
	}

	virtual void undoUpdate(ProcessState * processState, ModelResult * result){
		ActorModelResult* actorResult = dynamic_cast<ActorModelResult*>(result);
		undoUpdateActor(processState, actorResult);
	}

protected:
	virtual void updateActor(ProcessState * processState, ActorModelResult * result) = 0;
	virtual void undoUpdateActor(ProcessState * processState, ActorModelResult * result) = 0;

};

class TieUpdater : public Updater{
public:
	virtual void update(ProcessState * processState, ModelResult * result){
		// also: check whether conversion works
		TieModelResult* tieResult = dynamic_cast<TieModelResult*>(result);
		if (tieResult != 0) updateTie(processState, tieResult);
				else std::cout << "problem with type conversion in TieUpdater.tieResult";
	}

	virtual void undoUpdate(ProcessState * processState, ModelResult * result){
		TieModelResult* tieResult = dynamic_cast<TieModelResult*>(result);
		if (tieResult != 0) undoUpdateTie(processState, tieResult);
	}

protected:
	virtual void updateTie(ProcessState * processState, TieModelResult * result) = 0;
	virtual void undoUpdateTie(ProcessState * processState, TieModelResult * result) = 0;
};

class ActorAttributeIncreaseUpdater : public Updater{

public:
	ActorAttributeIncreaseUpdater(size_t indexAttribute, size_t indexActor){
		_indexAttribute = indexAttribute;
		_indexActor = indexActor;
	}

	void update(ProcessState* processState, ModelResult* result){
		processState->getAttributeContainer(_indexAttribute)->increaseValue(_indexActor);
	}

	void undoUpdate(ProcessState* processState, ModelResult* result){
		processState->getAttributeContainer(_indexAttribute)->decreaseValue(_indexActor);
	}

private:
	size_t _indexAttribute;
	size_t _indexActor;

};

class GlobalCountUpdater : public Updater{

public:
	GlobalCountUpdater(size_t indexGlobalVariableToUpdate){
		_indexGlobalVariable = indexGlobalVariableToUpdate;
	}

	void update(ProcessState* processState, ModelResult* result){
		//TODO: How does that work in elegant?
		*(processState->getGlobalAttribute(_indexGlobalVariable)) += 1.0;
	}

	void undoUpdate(ProcessState* processState, ModelResult* result){
		*(processState->getGlobalAttribute(_indexGlobalVariable)) -= 1.0;
	}

private:
	size_t _indexGlobalVariable;

};

class TimeUpdater : public Updater, public TimeModel {

public:
	virtual ~TimeUpdater(){ }

	virtual double getTimeSpan(ProcessState * processState) = 0;
	virtual void update(ProcessState* processState, TimeModelResult* timeModelResult) = 0;
	void undoUpdate(ProcessState* processState, TimeModelResult* timeModelResult){
		std::cout << "Undo not implemented for TimeUpdaters.";
	}

};

class ExponentialDecayTimeUpdater : public TimeUpdater {

public:
	ExponentialDecayTimeUpdater(size_t networkIndex, double halfLife, double resetValue){
		init(networkIndex, halfLife);
		_resetValue = resetValue;
	}
	ExponentialDecayTimeUpdater(size_t networkIndex, double halfLife){
		init(networkIndex, halfLife);
	}
	~ExponentialDecayTimeUpdater(){ }

	void update(ProcessState* processState, ModelResult* modelResult){
		// TODO write exception
		std::cout << "Unknown modelResult type in ExponentialDecayUpdater" << std::endl;
	}

	void update(ProcessState* processState, TimeModelResult* timeModelResult){
		MemoryOneModeNetwork* network =
				dynamic_cast<MemoryOneModeNetwork*>(
						processState->getNetwork(_networkIndex));
		double timeSpan = timeModelResult->getDoubleResult();

		network->multiplyTieValues(getMultiplicatorTieValues(timeSpan));
	}

	/**
	 * returns The time until a critical threshold is reached
	 */
	double getTimeSpan(ProcessState * processState){
		MemoryOneModeNetwork* network =
				dynamic_cast<MemoryOneModeNetwork*>(
						processState->getNetwork(_networkIndex));
		double minAboveReset = network->getMinTieValueAbove(_resetValue);
		return getTimeSpan(minAboveReset, _resetValue);
	}

private:
	double _halfLife;
	double _resetValue;
	size_t _networkIndex;

	void init(double networkIndex, double halfLife){
		_halfLife = halfLife;
		_networkIndex = networkIndex;
	}

	double getMultiplicatorTieValues(double timeSpan){
		return exp( - timeSpan * (log(2) / _halfLife) );
	}

	double getTimeSpan(double valueBefore, double valueAfter){
		if (!(valueBefore > valueAfter)) return FLT_MAX; // return "infinity"
		// half-life * [ log(before)/log(2) - log(after)/log(2) ]
		double timeSpan = _halfLife * ( log(valueBefore)/log(2) - log(valueAfter)/log(2) );
		return timeSpan;
	}

};

/**
 * Swaps ties in a network: If a tie exists it is removed,
 * if not is is added with the default value
 */
class TieSwapUpdater : public TieUpdater{

public:
	TieSwapUpdater(size_t networkIndex){
		_networkIndex = networkIndex;
		_tieValuePreviousUpdate = 0;
	}

	void updateTie(ProcessState * processState, TieModelResult * result){
		int i = result->getActorIndex1();
		int j = result->getActorIndex2();
		Network * network = processState->getNetwork(_networkIndex);
		_tieValuePreviousUpdate = network->getTieValue(i, j);
		swapTie(network, i, j);
	}

	/**
	 * Undoes the previously run update. In case of valued networks,
	 * the previous tie value is restored.
	 */
	void undoUpdateTie(ProcessState * processState, TieModelResult * result){
		int i = result->getActorIndex1();
		int j = result->getActorIndex2();
		Network * network = processState->getNetwork(_networkIndex);
		network->setTie(i, j, _tieValuePreviousUpdate);
	}


private:
	size_t _networkIndex;
	double _tieValuePreviousUpdate;
	void swapTie(Network * network, int i, int j){
		if (i == j) return;
		if (network->hasTie(i,j))
			network->removeTie(i,j);
		else if (!network->hasTie(i,j)){
			network->addTie(i,j);
		}

	}
};

class NullUpdater : public Updater{

	void update(ProcessState* processState, ModelResult* result){

	}

	void undoUpdate(ProcessState* processState, ModelResult* result){

	}

};


#endif /* UPDATER_H_ */
