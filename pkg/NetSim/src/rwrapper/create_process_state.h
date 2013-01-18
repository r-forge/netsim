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

RcppExport SEXP create_process_state(SEXP name);


#endif /* CREATE_PROCESS_STATE_H_ */
