# TODO: Add comment
# 
# Author: cws
###############################################################################

################################
# Create and update ModelManager

create_model_manager <- function(){
	.Call("create_model_manager", PACKAGE = "NetSim")
}

add_time_model <- function(modelManager, timeModel){
	.Call("add_time_model", modelManager, timeModel, PACKAGE = "NetSim")
}

add_change_model <- function(modelManager, timeModel, changeModel){
	.Call("add_change_model",modelManager, timeModel, changeModel, package = "NetSim")
	
}

add_updater <- function(modelManager, changeModel, updater){
	.Call("add_change_model",modelManager, changeModel, updater, package = "NetSim")
}

##############################
# Create particular models

# time models

create_poisson_model <- function(param = 1){
	.Call("create_poisson_model", param, PACKAGE = "NetSim")
}

# change models

create_effect_container <- function(){
	.Call("create_effect_container", PACKAGE = "NetSim")
}

create_effect <- function(x, ...){
	UseMethod("create_effect")
}

create_effect.character <- function(name, ...){
	type = get_effect_type(name);
	create_effect(type, name, ...)
	
}

create_effect.OneModeNetworkEffect <- function(type, name, networkIndex){
	print("It is a OneModenetworkEffect!!")
	.Call("create_one_mode_effect", name, networkIndex, PACKAGE = "NetSim")
}

get_effect_type <- function(name){
	#.Call("get_effect_type", name, PACKAGE = "NetSim")
	structure(name, class="OneModeNetworkEffect")
}

add_to_effect_container<- function(effectContainer, effect, paramValue){
	.Call("add_to_effect_container", effectContainer, effect, paramValue, PACKAGE = "NetSim")
}

# by default implemented as using the tie swap updater
create_multinomial_choice_network_change_model <- function(
		focalActor, networkIndex, effectContainer){
	updater <- create_tie_swap_updater(networkIndex);
	.Call("create_multinomial_choice_network_change_model",
			focalActor, networkIndex, effectContainer, updater,
			PACKAGE = "NetSim")		
}


# updaters

create_tie_swap_updater <- function(networkIndex){
	.Call("create_tie_swap_updater", networkIndex, PACKAGE = "NetSim")
}



