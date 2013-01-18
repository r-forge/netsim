/*
 * get_process_state_name.cpp
 *
 *  Created on: Jan 18, 2013
 *      Author: cws
 */

#include "get_process_state_name.h"

SEXP get_process_state_name(SEXP pointer) {
	Rcpp::XPtr<ProcessStateManager> processStateManager(pointer);
	std::string name = processStateManager->getName();

	return Rcpp::wrap(name);
}
