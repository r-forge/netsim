/*
 * ChangeModel.h
 *
 *  Created on: Nov 29, 2012
 *      Author: cws
 */

#ifndef CHANGEMODEL_H_
#define CHANGEMODEL_H_

#include "ModelResult.h"
#include "../processstate/ProcessState.h"

class ChangeModel{

public:
	virtual ~ChangeModel(){ }
	virtual ModelResult * getChange(ProcessState * processState) = 0;
};

class NullChangeModel : public ChangeModel{
	ModelResult * getChange(ProcessState * processState);
};


#endif /* CHANGEMODEL_H_ */
