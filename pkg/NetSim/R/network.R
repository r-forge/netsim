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

