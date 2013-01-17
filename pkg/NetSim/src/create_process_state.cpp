
/*
 * create_process_state.cpp
 *
 *  Created on: Jan 11, 2013
 *      Author: cws
 */

#include "create_process_state.h"
#include "processstate/ProcessState.h"
#include "utils/Random.h"

SEXP create_process_state(SEXP n) {
	Rcpp::CharacterVector name(n);
	// TODO Why does this cause an error in package building?
	// possible reason: link sub folders for builder
	// ProcessState * processState = new ProcessState();
	double rand = Random::getInstance().getRandom();
	Rcpp::NumericVector v1(2);
	v1[0] = rand;
	v1[1] = rand + 1;
	return v1;
}


