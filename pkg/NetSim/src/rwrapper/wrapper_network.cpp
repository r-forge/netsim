/*
 * wrapper_network.cpp
 *
 *  Created on: Jan 20, 2013
 *      Author: cws
 */

#include "wrapper_network.h"

SEXP create_network(SEXP matrix_, SEXP directed_, SEXP reflexive_) {
	//BEGIN_RCPP

	Rcpp::NumericMatrix matrix(matrix_);
	int nCol = matrix.ncol();
	int nRow = matrix.nrow();

	bool directed = Rcpp::as<bool>(directed_);
	bool reflexive = Rcpp::as<bool>(reflexive_);

	if (nCol != nRow) {
		throw std::invalid_argument("Matrix needs to be quadratic.");
		return R_NilValue;
	}

	MemoryOneModeNetwork * network = new MemoryOneModeNetwork(nCol, directed, reflexive);

	// check if there are problems when ties are set (setTie == false)
	int problematicTies = 0;
	for (int i = 0; i < nRow; i++){
		for (int j = 0; j < nCol; j++){
			if(!network->setTie(i, j,  matrix(i, j) ))
				++problematicTies;
		}
	}
	if (problematicTies > 0)
		Rcpp::Rcout << "Warning: " << problematicTies <<
			" ties could not be set." <<std::endl;

	Rcpp::XPtr<MemoryOneModeNetwork> pointer(network, true);
	return pointer;


	//END_RCPP
}

SEXP network_as_matrix(SEXP networkPointer_) {
	BEGIN_RCPP

	Rcpp::XPtr<MemoryOneModeNetwork> network(networkPointer_);

	int size = network->getSize();

	Rcpp::NumericMatrix matrix(size, size);

	for (int i = 0; i < size; i++){
		for (int j = 0; j < size; j++){
			matrix(i,j) = network->getTieValue(i, j);
		}
	}

	return matrix;

	END_RCPP
}
