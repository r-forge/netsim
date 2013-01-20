/*
 * wrapper_network.h
 *
 *  Created on: Jan 20, 2013
 *      Author: cws
 */

#ifndef WRAPPER_NETWORK_H_
#define WRAPPER_NETWORK_H_

#include <Rcpp.h>
#include <stdexcept>
#include "../network/MemoryOneModeNetwork.h"

RcppExport SEXP create_network(SEXP matrix_, SEXP directed_, SEXP reflexive_);

RcppExport SEXP network_as_matrix(SEXP networkPointer_);

#endif /* WRAPPER_NETWORK_H_ */
