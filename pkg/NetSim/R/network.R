# TODO: Add comment
# 
# Author: cws
###############################################################################

create_network <- function(matrix){
	.Call( "create_network", matrix, PACKAGE = "NetSim" )
}


create_network <- function(network, directed = TRUE, reflexive = FALSE){
	.Call( "network_as_matrix", network, directed, reflexive, PACKAGE = "NetSim" )
}

