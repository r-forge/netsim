/*
 * TimeModel.h
 *
 *  Created on: Nov 28, 2012
 *      Author: cws
 */

#ifndef TIMEMODEL_H_
#define TIMEMODEL_H_

#include "../utils/Random.h"
#include "../processstate/ProcessState.h"

/**
 * Abstract class defining a Time model
 */
class TimeModel{

public:
	virtual ~TimeModel() { }

	/**
	 * get time span until next change. A timespan can never be zero or less.
	 */
	virtual double getTimeSpan(ProcessState * processState) = 0;


};

class PoissonTimeModel : public TimeModel{

public:
	PoissonTimeModel(double poissonParameter);

	double getTimeSpan(ProcessState * processState);

	double getParameter();

private:
	double _poissonParameter;

};



#endif /* TIMEMODEL_H_ */
