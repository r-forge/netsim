/*
 * wrapper_model.h
 *
 *  Created on: Jan 22, 2013
 *      Author: cws
 */

#ifndef WRAPPER_MODEL_H_
#define WRAPPER_MODEL_H_


#include <Rcpp.h>
#include "../model/ModelManager.h"
#include "../model/TimeModel.h"
#include "../model/ChangeModel.h"
#include "../model/MultinomialChoiceChangeModel.h"
#include "../model/Updater.h"
#include "../model/saom/NetworkEffect.h"
#include "../model/saom/EffectContainerManager.h"
#include "../model/saom/EffectFactory.h"

RcppExport SEXP create_model_manager();

RcppExport SEXP add_time_model(SEXP modelManager, SEXP timeModel, SEXP name);

RcppExport SEXP add_change_model(SEXP modelManager, SEXP timeModel, SEXP changeModel, SEXP name);

RcppExport SEXP add_updater(SEXP modelManager, SEXP changeModel, SEXP updater);

RcppExport SEXP create_poisson_model(SEXP param);

RcppExport SEXP create_effect_container();

RcppExport SEXP create_one_mode_effect(SEXP name, SEXP networkIndex);

RcppExport SEXP get_effect_type(SEXP name);

RcppExport SEXP add_to_effect_container(SEXP effectContainer, SEXP effect, SEXP paramValue);

/**
 * Multinomial choice network change model.
 * So far, this only implements one updater instead of an updater list.
 */
RcppExport SEXP create_multinomial_choice_network_change_model(
		SEXP networkIndex, SEXP networkName, SEXP effectContainer, SEXP updater);

RcppExport SEXP create_tie_swap_updater(SEXP networkIndex);

#endif /* WRAPPER_MODEL_H_ */
