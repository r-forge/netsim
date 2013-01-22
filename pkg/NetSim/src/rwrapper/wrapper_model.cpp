/*
 * wrapper_model.cpp
 *
 *  Created on: Jan 22, 2013
 *      Author: cws
 */

#include "wrapper_model.h"

SEXP create_model_manager() {
	BEGIN_RCPP

	ModelManager * modelManager = new ModelManager;
	return Rcpp::XPtr<ModelManager>(modelManager, true);

	END_RCPP;
}

SEXP add_time_model(SEXP modelManager_, SEXP timeModel_) {
	BEGIN_RCPP

	ModelManager * modelManager = Rcpp::XPtr<ModelManager>(modelManager_);
	TimeModel * timeModel = Rcpp::XPtr<TimeModel>(timeModel_);

	modelManager->addTimeModel(timeModel);

	return Rcpp::XPtr<ModelManager>(modelManager, true);

	END_RCPP
}

SEXP add_change_model(SEXP modelManager_, SEXP timeModel_, SEXP changeModel_) {
	BEGIN_RCPP

	ModelManager * modelManager = Rcpp::XPtr<ModelManager>(modelManager_);
	TimeModel * timeModel = Rcpp::XPtr<TimeModel>(timeModel_);
	ChangeModel * changeModel = Rcpp::XPtr<ChangeModel>(changeModel_);

	modelManager->addChangeModel(timeModel, changeModel);

	return Rcpp::XPtr<ModelManager>(modelManager, true);

	END_RCPP
}

SEXP add_updater(SEXP modelManager_, SEXP changeModel_, SEXP updater_) {
	BEGIN_RCPP

	ModelManager * modelManager = Rcpp::XPtr<ModelManager>(modelManager_);
	ChangeModel * changeModel = Rcpp::XPtr<ChangeModel>(changeModel_);
	Updater * updater = Rcpp::XPtr<Updater>(updater_);

	modelManager->addUpdater(changeModel, updater);

	return Rcpp::XPtr<ModelManager>(modelManager, true);

	END_RCPP
}

SEXP create_poisson_model(SEXP param_) {
	BEGIN_RCPP

	double param = Rcpp::as<double>(param_);
	PoissonTimeModel * poissonModel = new PoissonTimeModel(param);

	return Rcpp::XPtr<PoissonTimeModel>(poissonModel, true);

	END_RCPP
}

SEXP create_effect_container() {
	BEGIN_RCPP

	EffectContainerManager * manager = new EffectContainerManager();
	return Rcpp::XPtr<EffectContainerManager>(manager, true);

	END_RCPP
}

SEXP create_one_mode_effect(SEXP name_, SEXP networkIndex_) {
	BEGIN_RCPP

	std::string name = Rcpp::as<std::string>(name_);
	int index = Rcpp::as<int>(networkIndex_);

	EffectFactory factory;
	OneModeNetworkEffect * effect =
			factory.getOneModeNetworkEffect(name, index);

	return Rcpp::XPtr<OneModeNetworkEffect>(effect, true);

	END_RCPP
}

SEXP get_effect_type(SEXP name_) {
	BEGIN_RCPP

	END_RCPP
}

SEXP add_to_effect_container(SEXP effectContainer_, SEXP effect_, SEXP paramValue_) {
	BEGIN_RCPP

	EffectContainerManager * manager = Rcpp::XPtr<EffectContainerManager>(
			effectContainer_);
	SaomEffect * effect = Rcpp::XPtr<SaomEffect>(effect_);
	double parameter = Rcpp::as<double>(paramValue_);

	manager->addToEffectContainer(effect, parameter);

	return Rcpp::XPtr<EffectContainerManager>(manager, true);

	END_RCPP
}

SEXP create_multinomial_choice_network_change_model(SEXP focalActor_,
		SEXP networkIndex_, SEXP effectContainer_, SEXP updater_) {
	BEGIN_RCPP

	int focalActor = Rcpp::as<int>(focalActor_);
	int networkIndex= Rcpp::as<int>(networkIndex_);


	EffectContainerManager * effectManager =
			Rcpp::XPtr<EffectContainerManager>(effectContainer_);
	Updater * updater = Rcpp::XPtr<Updater>(updater_);

	std::vector<Updater* > * updateList = new std::vector<Updater*>(1, updater);

	MultinomialChoiceNetworkChangeModel * saom =
			new MultinomialChoiceNetworkChangeModel(
			focalActor,
			networkIndex,
			effectManager->getEffectContainer(),
			updateList);

	return Rcpp::XPtr<MultinomialChoiceNetworkChangeModel>(saom, true);

	END_RCPP
}

SEXP create_tie_swap_updater(SEXP networkIndex_) {
	BEGIN_RCPP

	int networkIndex = Rcpp::as<int>(networkIndex_);

	TieSwapUpdater * tieSwapUpdater =
			new TieSwapUpdater(networkIndex);

	return Rcpp::XPtr<TieSwapUpdater>(tieSwapUpdater, true);

	END_RCPP
}
