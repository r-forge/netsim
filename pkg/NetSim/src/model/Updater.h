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
#include "../network/ValuedMemoryOneModeNetwork.h"
#include "../model/TimeModel.h"

class Updater{
public:
	virtual ~Updater() { }
	virtual void update(ProcessState* processState, ModelResult* result) = 0;
	virtual void undoUpdate(ProcessState* processState, ModelResult* result) = 0;

};

class ActorUpdater : public Updater{
public:
	virtual void update(ProcessState * processState, ModelResult * result);

	virtual void undoUpdate(ProcessState * processState, ModelResult * result);

protected:
	virtual void updateActor(ProcessState * processState, ActorModelResult * result) = 0;
	virtual void undoUpdateActor(ProcessState * processState, ActorModelResult * result) = 0;

};

class TieUpdater : public Updater{
public:
	virtual void update(ProcessState * processState, ModelResult * result);

	virtual void undoUpdate(ProcessState * processState, ModelResult * result);

protected:
	virtual void updateTie(ProcessState * processState, TieModelResult * result) = 0;
	virtual void undoUpdateTie(ProcessState * processState, TieModelResult * result) = 0;
};

class ActorAttributeIncreaseUpdater : public Updater{

public:
	ActorAttributeIncreaseUpdater(size_t indexAttribute, size_t indexActor);

	void update(ProcessState* processState, ModelResult* result);

	void undoUpdate(ProcessState* processState, ModelResult* result);

private:
	size_t _indexAttribute;
	size_t _indexActor;

};

class ActorAttributeSetUpdater : public Updater{
public:
	ActorAttributeSetUpdater(size_t indexAttribute, size_t indexActor);

	void update(ProcessState* processState, ModelResult* result);

	void undoUpdate(ProcessState* processState, ModelResult* result);

private:
	size_t _indexAttribute;
	size_t _indexActor;
	double _oldValue;
};

class GlobalCountUpdater : public Updater{

public:
	GlobalCountUpdater(size_t indexGlobalVariableToUpdate);

	void update(ProcessState* processState, ModelResult* result);

	void undoUpdate(ProcessState* processState, ModelResult* result);

private:
	size_t _indexGlobalVariable;

};

class TimeUpdater : public Updater, public TimeModel {

public:
	virtual ~TimeUpdater(){ }

	virtual double getTimeSpan(ProcessState * processState) = 0;
	virtual void update(ProcessState* processState, TimeModelResult* timeModelResult) = 0;
	virtual void update(ProcessState* processState, double timeSpan) = 0;
	// TODO implement with a proper exception
	void undoUpdate(ProcessState* processState, TimeModelResult* timeModelResult){
		std::cout << "Undo not implemented for TimeUpdaters.";
	}

};

class ExponentialDecayTimeUpdater : public TimeUpdater {

public:
	ExponentialDecayTimeUpdater(size_t networkIndex, double halfLife, double resetValue);

	ExponentialDecayTimeUpdater(size_t networkIndex, double halfLife);

	~ExponentialDecayTimeUpdater(){ }

	void update(ProcessState* processState, ModelResult* modelResult);

	void update(ProcessState* processState, TimeModelResult* timeModelResult);

	void update(ProcessState* processState, double timeSpan);

	/**
	 * returns The time until a critical threshold is reached
	 */
	double getTimeSpan(ProcessState * processState);

private:
	double _halfLife;
	double _resetValue;
	size_t _networkIndex;

	void init(double networkIndex, double halfLife);

	double getMultiplicatorTieValues(double timeSpan);

	double getTimeSpan(double valueBefore, double valueAfter);

};

/**
 * Swaps ties in a network: If a tie exists it is removed,
 * if not is is added with the default value
 */
class TieSwapUpdater : public TieUpdater{

public:
	TieSwapUpdater(size_t networkIndex);

	void updateTie(ProcessState * processState, TieModelResult * result);

	/**
	 * Undoes the previously run update. In case of valued networks,
	 * the previous tie value is restored.
	 */
	void undoUpdateTie(ProcessState * processState, TieModelResult * result);


private:
	size_t _networkIndex;
	double _tieValuePreviousUpdate;
	void swapTie(Network * network, int i, int j);

};

class AddTiesFromNewbornActor : public Updater{

	public:
	AddTiesFromNewbornActor(int networkIndex);

	void update(ProcessState* processState, ModelResult* result);
	void undoUpdate(ProcessState* processState, ModelResult* result);

	private:
	int _networkIndex;

};


#endif /* UPDATER_H_ */
