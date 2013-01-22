# TODO: Add comment
# 
# Author: cws
###############################################################################


create_simulator <- function(processState, modelManager, periodLength, verbose = FALSE){
	.Call("create_simulator", processState, modelManager, periodLength, verbose,
			PACKAGE = "NetSim")
}

simulate <- function(simulator){
	.Call("simulate", simulator, PACKAGE = "NetSim")
}

