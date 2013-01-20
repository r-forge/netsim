
/*
 * create_process_state.cpp
 *
 *  Created on: Jan 11, 2013
 *      Author: cws
 */

#include "process_state.h"

SEXP create_process_state(SEXP n) {

	Rcpp::CharacterVector name(n);
	std::string nameString =  Rcpp::as<std::string>(name);

	ProcessStateManager * processStateManager = new ProcessStateManager(nameString);

	Rcpp::XPtr<ProcessStateManager> pointer(processStateManager, true);

	return pointer;
}


SEXP get_process_state_name(SEXP pointer) {
	Rcpp::XPtr<ProcessStateManager> processStateManager(pointer);
	std::string name = processStateManager->getName();

	return Rcpp::wrap(name);
}

SEXP add_network(SEXP processStateManager_, SEXP network_, SEXP name_) {
	ProcessStateManager * processStateManager =
			Rcpp::XPtr<ProcessStateManager>(processStateManager_);

	MemoryOneModeNetwork * network =
			Rcpp::XPtr<MemoryOneModeNetwork>(network_);
	std::string name = Rcpp::as<std::string>(name_);

	processStateManager->addNetwork(network, name);

	// is this exactly the same pointer or is there a chance that it has changed?
	return Rcpp::XPtr<ProcessStateManager>(processStateManager, true);

}
