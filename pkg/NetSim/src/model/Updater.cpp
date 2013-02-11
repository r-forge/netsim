/*
 * Updater.cpp
 *
 *  Created on: Jan 17, 2013
 *      Author: cws
 */

#include "Updater.h"

void ActorUpdater::update(ProcessState* processState, ModelResult* result) {
	ActorModelResult* actorResult = dynamic_cast<ActorModelResult*>(result);
	updateActor(processState, actorResult);
}

void ActorUpdater::undoUpdate(ProcessState* processState, ModelResult* result) {
	ActorModelResult* actorResult = dynamic_cast<ActorModelResult*>(result);
	undoUpdateActor(processState, actorResult);
}

void TieUpdater::update(ProcessState* processState, ModelResult* result) {
	// also: check whether conversion works
	TieModelResult* tieResult = dynamic_cast<TieModelResult*>(result);
	if (tieResult != 0) updateTie(processState, tieResult);
			else std::cout << "problem with type conversion in TieUpdater.tieResult";
}

void TieUpdater::undoUpdate(ProcessState* processState, ModelResult* result) {
	TieModelResult* tieResult = dynamic_cast<TieModelResult*>(result);
	if (tieResult != 0) undoUpdateTie(processState, tieResult);
}

ActorAttributeIncreaseUpdater::ActorAttributeIncreaseUpdater(
		size_t indexAttribute, size_t indexActor) {
	_indexAttribute = indexAttribute;
	_indexActor = indexActor;
}

void ActorAttributeIncreaseUpdater::update(ProcessState* processState,
		ModelResult* result) {
	processState->getAttributeContainer(_indexAttribute)->increaseValue(_indexActor);
}

void ActorAttributeIncreaseUpdater::undoUpdate(ProcessState* processState,
		ModelResult* result) {
	processState->getAttributeContainer(_indexAttribute)->decreaseValue(_indexActor);
}

GlobalCountUpdater::GlobalCountUpdater(size_t indexGlobalVariableToUpdate) {
	_indexGlobalVariable = indexGlobalVariableToUpdate;
}

void GlobalCountUpdater::update(ProcessState* processState,
		ModelResult* result) {
	double value = processState->getGlobalAttribute(_indexGlobalVariable);
	value += 1;
	processState->setGlobalAttribute(_indexGlobalVariable, value);
}

void GlobalCountUpdater::undoUpdate(ProcessState* processState,
		ModelResult* result) {
	double value = processState->getGlobalAttribute(_indexGlobalVariable);
	value -= 1;
	processState->setGlobalAttribute(_indexGlobalVariable, value);
}

ExponentialDecayTimeUpdater::ExponentialDecayTimeUpdater(size_t networkIndex,
		double halfLife, double resetValue) {
	init(networkIndex, halfLife);
	_resetValue = resetValue;
}

ExponentialDecayTimeUpdater::ExponentialDecayTimeUpdater(size_t networkIndex,
		double halfLife) {
	init(networkIndex, halfLife);
}

void ExponentialDecayTimeUpdater::update(ProcessState* processState,
		ModelResult* modelResult) {
	// TODO write exception
	std::cout << "Unknown modelResult type in ExponentialDecayUpdater" << std::endl;
}

void ExponentialDecayTimeUpdater::update(ProcessState* processState,
		TimeModelResult* timeModelResult) {
	double timeSpan = timeModelResult->getDoubleResult();
	update(processState, timeSpan);
}

void ExponentialDecayTimeUpdater::update(ProcessState* processState,
		double timeSpan) {
	ValuedMemoryOneModeNetwork* network =
			dynamic_cast<ValuedMemoryOneModeNetwork*>(
					processState->getNetwork(_networkIndex));

	network->multiplyTieValues(getMultiplicatorTieValues(timeSpan));
}


double ExponentialDecayTimeUpdater::getTimeSpan(ProcessState* processState) {
	ValuedMemoryOneModeNetwork * network =
			dynamic_cast<ValuedMemoryOneModeNetwork*>(
					processState->getNetwork(_networkIndex));
	double minAboveReset = network->getMinTieValueAbove(_resetValue);
	return getTimeSpan(minAboveReset, _resetValue);
}

void ExponentialDecayTimeUpdater::init(double networkIndex, double halfLife) {
	_halfLife = halfLife;
	_networkIndex = networkIndex;
}

double ExponentialDecayTimeUpdater::getMultiplicatorTieValues(double timeSpan) {
	return exp( - timeSpan * (log(2) / _halfLife) );
}

double ExponentialDecayTimeUpdater::getTimeSpan(double valueBefore,
		double valueAfter) {
	if (!(valueBefore > valueAfter)) return FLT_MAX; // return "infinity"
	// half-life * [ log(before)/log(2) - log(after)/log(2) ]
	double timeSpan = _halfLife * ( log(valueBefore)/log(2) - log(valueAfter)/log(2) );
	return timeSpan;
}

TieSwapUpdater::TieSwapUpdater(size_t networkIndex) {
	_networkIndex = networkIndex;
	_tieValuePreviousUpdate = 0;
}

void TieSwapUpdater::updateTie(ProcessState* processState,
		TieModelResult* result) {
	int i = result->getActorIndex1();
	int j = result->getActorIndex2();
	Network * network = processState->getNetwork(_networkIndex);
	_tieValuePreviousUpdate = network->getTieValue(i, j);
	swapTie(network, i, j);
}

// TODO: only works if undo is run directly after update of the same tie
void TieSwapUpdater::undoUpdateTie(ProcessState* processState,
		TieModelResult* result) {
	int i = result->getActorIndex1();
	int j = result->getActorIndex2();
	Network * network = processState->getNetwork(_networkIndex);
	network->setTie(i, j, _tieValuePreviousUpdate);
}

void TieSwapUpdater::swapTie(Network* network, int i, int j) {
	if (i == j) return;
	if (network->hasTie(i, j))
		network->removeTie(i, j);
	else if (!network->hasTie(i, j)){
		network->addTie(i, j);
	}
}

ActorAttributeSetUpdater::ActorAttributeSetUpdater(size_t indexAttribute,
		size_t indexActor) {
	_indexAttribute = indexAttribute;
	_indexActor = indexActor;
	_oldValue = -1;
}

void ActorAttributeSetUpdater::update(ProcessState* processState,
		ModelResult* result) {

	AttributeContainer * attributeContainer = processState->getAttributeContainer(_indexAttribute);

	ValueModelResult * valueModelResult = dynamic_cast<ValueModelResult*>(result);
	_oldValue = attributeContainer->getValue(_indexActor);

	attributeContainer->setValue(_indexActor, valueModelResult->getValue());

}

void ActorAttributeSetUpdater::undoUpdate(ProcessState* processState,
		ModelResult* result) {

	AttributeContainer * attributeContainer = processState->getAttributeContainer(_indexAttribute);
	attributeContainer->setValue(_indexActor, _oldValue);

}
