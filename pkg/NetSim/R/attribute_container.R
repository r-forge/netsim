# TODO: Add comment
# 
# Author: cws
###############################################################################

attribute_container_as_list <- function(attributeContainer){
	.Call( "attribute_container_as_list", attributeContainer, PACKAGE = "NetSim" )
}


create_attribute_container <- function(list){
	.Call( "create_attribute_container", list, PACKAGE = "NetSim" )
}

create_scale_attribute_container <- function(list, min = 0, max = 1, by = 1){
	.Call("create_scale_attribute_container", 
			list, min, max, by, PACKAGE = "NetSim")
}

set_value <- function(attributeContainer, i, value){
	.Call("set_value", attributeContainer, i, value, PACKAGE = "NetSim")
}

