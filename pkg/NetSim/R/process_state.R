
create_process_state <- function(name = "default"){
	.Call( "create_process_state", name, PACKAGE = "NetSim" )
}

get_process_state_name <- function(processState){
	.Call("get_process_state_name", processState, PACKAGE = "NetSim")
}