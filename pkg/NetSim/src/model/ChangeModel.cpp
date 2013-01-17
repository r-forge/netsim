/*
 * ChangeModel.cpp
 *
 *  Created on: Jan 17, 2013
 *      Author: cws
 */

#include "ChangeModel.h"

ActorNullChangeModel::ActorNullChangeModel(int id) {
	_id = id;
}

ModelResult* ActorNullChangeModel::getChange(ProcessState* processState) {
	return new ActorModelResult(_id);
}
