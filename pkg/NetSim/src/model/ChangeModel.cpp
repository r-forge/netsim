/*
 * ChangeModel.cpp
 *
 *  Created on: Jan 17, 2013
 *      Author: cws
 */

#include "ChangeModel.h"

ModelResult* NullChangeModel::getChange(ProcessState* processState) {
	return new ActorModelResult(0);
}
