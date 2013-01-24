
/*
 * wrapper_process_state.cpp
 *
 *  Created on: Jan 11, 2013
 *      Author: cws
 */

#include "wrapper_process_state.h"

SEXP create_process_state(SEXP n) {
	BEGIN_RCPP

	Rcpp::CharacterVector name(n);
	std::string nameString =  Rcpp::as<std::string>(name);

	ProcessStateManager * processStateManager = new ProcessStateManager(nameString);

	Rcpp::XPtr<ProcessStateManager> pointer(processStateManager, false);

	pointer.attr("class") = "processState";

	return pointer;

	END_RCPP
}


SEXP get_process_state_name(SEXP pointer) {
	BEGIN_RCPP

	Rcpp::XPtr<ProcessStateManager> processStateManager(pointer);
	std::string name = processStateManager->getName();

	return Rcpp::wrap(name);

	END_RCPP
}

SEXP add_network(SEXP processStateManager_, SEXP network_, SEXP name_) {
	BEGIN_RCPP

	ProcessStateManager * processStateManager =
			Rcpp::XPtr<ProcessStateManager>(processStateManager_);

	MemoryOneModeNetwork * network =
			Rcpp::XPtr<MemoryOneModeNetwork>(network_);
	std::string name = Rcpp::as<std::string>(name_);

	processStateManager->addNetwork(network, name);

	// is this exactly the same pointer or is there a chance that it has changed?
	return Rcpp::XPtr<ProcessStateManager>(processStateManager, true);

	END_RCPP
}

SEXP get_network_index(SEXP processStateManager_, SEXP name_) {
	BEGIN_RCPP

	ProcessStateManager * processStateManager =
			Rcpp::XPtr<ProcessStateManager>(processStateManager_);
	std::string name = Rcpp::as<std::string>(name_);

	size_t index = processStateManager->getNetworkIndex(name);

	return Rcpp::wrap(index);

	END_RCPP
}

SEXP get_network(SEXP processStateManager_, SEXP name_) {
	BEGIN_RCPP

	ProcessStateManager * processStateManager =
			Rcpp::XPtr<ProcessStateManager>(processStateManager_);
	std::string name = Rcpp::as<std::string>(name_);

	Network * network = processStateManager->getNetwork(name);

	return Rcpp::XPtr<Network>(network, false);

	END_RCPP
}
