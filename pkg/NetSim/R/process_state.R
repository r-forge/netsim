
create_process_state <- function(name = "default"){
	.Call( "create_process_state", name, PACKAGE = "NetSim" )
}

get_process_state_name <- function(processState){
	.Call("get_process_state_name", processState, PACKAGE = "NetSim")
}

add_network <- function(processState, network, name = "defaultNetwork"){
	.Call("add_network", processState, network, name, PACKAGE = "NetSim")
}

get_network_index <- function(processState, name = "defaultNetwork"){
	.Call("get_network_index", processState, name, PACKAGE = "NetSim")
} 

get_network <- function(processState, name = "defaultNetwork"){
	.Call("get_network", processState, name, PACKAGE = "NetSim")
} 

add_attribute_container <- function(processState, attributeContainer, name = "defaultAttribute"){
	.Call("add_attribute_container", processState, attributeContainer, name, PACKAGE = "NetSim")
}

get_attribute_container_index <- function(processState, name = "defaultAttribute"){
	.Call("get_attribute_container_index", processState, name, PACKAGE = "NetSim")
} 

get_attribute_container <- function(processState, name = "defaultAttribute"){
	.Call("get_attribute_container", processState, name, PACKAGE = "NetSim")
} 

# for testing
print.processState <- function(x){
	print(network_as_matrix(get_network("default")));
}