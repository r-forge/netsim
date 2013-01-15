
/*
 * create_process_state.cpp
 *
 *  Created on: Jan 11, 2013
 *      Author: cws
 */

#include "create_process_state.h"
#include "processstate/ProcessState.h"

SEXP create_process_state(SEXP n) {
	Rcpp::CharacterVector name(n);
	// TODO Why does this cause an error in package building?
	// possible reason: link sub folders for builder
	// ProcessState * processState = new ProcessState();
	Rcpp::NumericVector v1(2);
	v1[0] = 1;
	v1[1] = 2;
	return n;
}


