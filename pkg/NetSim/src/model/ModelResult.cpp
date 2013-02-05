/*
 * ModelResult.cpp
 *
 *  Created on: Jan 17, 2013
 *      Author: cws
 */

#include "ModelResult.h"

ActorModelResult::ActorModelResult(int actorIndex) {
	_actorIndex = actorIndex;
}

int ActorModelResult::getActorIndex() {
	return _actorIndex;
}

TieModelResult::TieModelResult(int actorIndex1, int actorIndex2) {
	_actorIndex1 = actorIndex1;
	_actorIndex2 = actorIndex2;
}

int TieModelResult::getActorIndex1() {
	return _actorIndex1;
}

TieModelResult& TieModelResult::operator =(const TieModelResult& tmr) {
	_actorIndex1 = tmr._actorIndex1;
	_actorIndex1 = tmr._actorIndex2;
	return *this;
}

int TieModelResult::getActorIndex2() {
	return _actorIndex2;
}

TimeModelResult::TimeModelResult(double timeSpan) {
	_timeSpan = timeSpan;
}

double TimeModelResult::getDoubleResult() {
	return _timeSpan;
}

std::string TimeModelResult::getResultString() {
	return ""; //TODO implement
}

ValueModelResult::ValueModelResult(double value) {
	_value = value;
}

double ValueModelResult::getValue() {
	return _value;
}
