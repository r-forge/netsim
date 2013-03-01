/*
 * WattsStrogatzRandomRewiringChangeModel.h
 *
 *  Created on: Feb 26, 2013
 *      Author: cws
 */

#ifndef WATTSSTROGATZRANDOMREWIRINGCHANGEMODEL_H_
#define WATTSSTROGATZRANDOMREWIRINGCHANGEMODEL_H_

#include "ChangeModel.h"
#include "ModelResult.h"
#include "MemoryOneModeNetwork.h"
#include "Random.h"
#include <stdexcept>

class WattsStrogatzRandomRewiringChangeModel : public ChangeModel{

public:
	WattsStrogatzRandomRewiringChangeModel(size_t networkIndex);

	ModelResult * getChange(ProcessState * processState);

	void setDebug(bool debug = true);

private:
	size_t _networkIndex;
	bool _debug;
};



#endif /* WATTSSTROGATZRANDOMREWIRINGCHANGEMODEL_H_ */
