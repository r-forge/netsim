# TODO: Add comment
# 
# Author: cws
###############################################################################


create_simulator <- function(processState, modelManager, periodLength){
	.Call("create_simulator", processState, modelManager, periodLength,
			PACKAGE = "NetSim")
}

simulate <- function(simulator){
	.Call("simulate", simulator, PACKAGE = "NetSim")
}

