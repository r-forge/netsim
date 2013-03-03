# TODO: Add comment
# 
# Author: cws
###############################################################################

network_as_matrix <- function(matrix){
	.Call( "network_as_matrix", matrix, PACKAGE = "NetSim" )
}


create_network <- function(network, directed = TRUE, reflexive = FALSE){
	.Call( "create_network", network, directed, reflexive, PACKAGE = "NetSim" )
}

set_tie <- function(network, i, j, value){
	.Call("set_tie", network, i, j, value, PACKAGE = "NetSim")
}

#RcppExport SEXP add_random_ties_to_network(SEXP network, SEXP probability);
add_random_ties_to_network <- function(network, probability = 0.5){
	.Call("add_random_ties_to_network", network, probability, PACKAGE = "NetSim")
}

# RcppExport SEXP add_ring_lattice_to_network(SEXP network, SEXP nReciprocalTies);
add_ring_lattice_to_network <- function(network, nReciprocalTies = 2){
	.Call("add_ring_lattice_to_network", network, nReciprocalTies, PACKAGE = "NetSim")
}
