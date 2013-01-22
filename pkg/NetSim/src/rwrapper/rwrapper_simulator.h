/*
 * rwrapper_simulator.h
 *
 *  Created on: Jan 22, 2013
 *      Author: cws
 */

#ifndef RWRAPPER_SIMULATOR_H_
#define RWRAPPER_SIMULATOR_H_

#include <Rcpp.h>
#include "../simulator/Simulator.h"
#include "../processstate/ProcessStateManager.h"
#include "../model/ModelManager.h"

RcppExport SEXP create_simulator(
		SEXP processState, SEXP modelManager, SEXP periodLength);

RcppExport SEXP simulate(
		SEXP simulator);



#endif /* RWRAPPER_SIMULATOR_H_ */
