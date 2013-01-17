/*
 * TimeModel.cpp
 *
 *  Created on: Jan 17, 2013
 *      Author: cws
 */

#include "TimeModel.h"

PoissonTimeModel::PoissonTimeModel(double poissonParameter) {
	_poissonParameter = poissonParameter;
}

double PoissonTimeModel::getTimeSpan(ProcessState* processState) {
	double randomNumber = Random::getInstance().getRandom();
	double timeSpan = -1 * log(1 - randomNumber) / _poissonParameter;
	return timeSpan;
}

double PoissonTimeModel::getParameter() {
	return _poissonParameter;
}
