# TODO: Add comment
# 
# Author: cws
###############################################################################


reset_random_seed <- function(seed){
	if (seed < 0) stop("Seed must be a positive integer")
	.Call("reset_random_seed", seed, PACKAGE = "NetSim")
}

get_random_seed <- function(){
	.Call("get_random_seed", PACKAGE = "NetSim")
}
