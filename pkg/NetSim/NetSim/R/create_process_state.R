create_process_state <- function(name = "foo"){
	.Call( "create_process_state", name, PACKAGE = "NetSim" )
}

