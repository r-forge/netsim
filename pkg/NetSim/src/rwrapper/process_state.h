/*
 * create_process_state.h
 *
 *  Created on: Jan 11, 2013
 *      Author: cws
 */

#ifndef CREATE_PROCESS_STATE_H_
#define CREATE_PROCESS_STATE_H_

#include <Rcpp.h>
#include "../processstate/ProcessState.h"
#include "../processstate/ProcessStateManager.h"
#include "../network/MemoryOneModeNetwork.h"

RcppExport SEXP create_process_state(SEXP name);

RcppExport SEXP get_process_state_name(SEXP pointer);

RcppExport SEXP add_network(SEXP processStateManager, SEXP networkPointer, SEXP name);


#endif /* CREATE_PROCESS_STATE_H_ */
