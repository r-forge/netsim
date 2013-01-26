# TODO: Add comment
# 
# Author: cws
###############################################################################


create_simulator <- function(processState, modelManager, 
		periodLength = 1, verbose = TRUE){
	.Call("create_simulator", processState, modelManager, periodLength, verbose,
			PACKAGE = "NetSim")
}

simulate <- function(simulator){
	.Call("simulate", simulator, PACKAGE = "NetSim")
}

get_iteration_steps <- function(simulator){
	.Call("get_iteration_steps", simulator, PACKAGE = "NetSim")
}