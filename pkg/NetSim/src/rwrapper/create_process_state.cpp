
/*
 * create_process_state.cpp
 *
 *  Created on: Jan 11, 2013
 *      Author: cws
 */

#include "create_process_state.h"

SEXP create_process_state(SEXP n) {

	Rcpp::CharacterVector name(n);
	std::string nameString =  Rcpp::as<std::string>(name);

	ProcessStateManager * processStateManager = new ProcessStateManager(nameString);

	Rcpp::XPtr<ProcessStateManager> pointer(processStateManager, true);

	// register in singleton project manager, return id plus name in R object

	// double rand = Random::getInstance().getRandom();
	// Rcpp::NumericVector v1(2);
	// v1[0] = rand;
	// v1[1] = rand + 1;

	return pointer;
}


