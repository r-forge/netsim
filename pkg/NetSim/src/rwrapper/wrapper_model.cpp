/*
 * wrapper_model.cpp
 *
 *  Created on: Jan 22, 2013
 *      Author: cws
 */

#include "wrapper_model.h"

SEXP create_model_manager() {
	BEGIN_RCPP

	ModelManager * modelManager = new ModelManager();

	return Rcpp::XPtr<ModelManager>(modelManager, false);

	END_RCPP;
}

SEXP add_time_model(SEXP modelManager_, SEXP timeModel_) {
	BEGIN_RCPP

	ModelManager * modelManager = Rcpp::XPtr<ModelManager>(modelManager_);
	TimeModel * timeModel = Rcpp::XPtr<TimeModel>(timeModel_);

	modelManager->addTimeModel(timeModel);

	return Rcpp::XPtr<ModelManager>(modelManager, false);

	END_RCPP
}

SEXP add_change_model(SEXP modelManager_, SEXP timeModel_, SEXP changeModel_) {
	BEGIN_RCPP

	ModelManager * modelManager = Rcpp::XPtr<ModelManager>(modelManager_);
	TimeModel * timeModel = Rcpp::XPtr<TimeModel>(timeModel_);
	ChangeModel * changeModel = Rcpp::XPtr<ChangeModel>(changeModel_);

	modelManager->addChangeModel(timeModel, changeModel);

	return Rcpp::XPtr<ModelManager>(modelManager, false);

	END_RCPP
}

SEXP add_updater(SEXP modelManager_, SEXP changeModel_, SEXP updater_) {
	BEGIN_RCPP

	ModelManager * modelManager = Rcpp::XPtr<ModelManager>(modelManager_);
	ChangeModel * changeModel = Rcpp::XPtr<ChangeModel>(changeModel_);
	Updater * updater = Rcpp::XPtr<Updater>(updater_);

	modelManager->addUpdater(changeModel, updater);

	return Rcpp::XPtr<ModelManager>(modelManager, false);

	END_RCPP
}

SEXP create_poisson_model(SEXP param_) {
	BEGIN_RCPP

	double param = Rcpp::as<double>(param_);
	PoissonTimeModel * poissonModel = new PoissonTimeModel(param);

	return Rcpp::XPtr<PoissonTimeModel>(poissonModel, false);

	END_RCPP
}

SEXP create_effect_container() {
	BEGIN_RCPP

	EffectContainerManager * manager = new EffectContainerManager();
	return Rcpp::XPtr<EffectContainerManager>(manager, false);

	END_RCPP
}

SEXP get_effect_type(SEXP name_) {
	BEGIN_RCPP

	std::string name = Rcpp::as<std::string>(name_);

	EffectFactory factory;
	std::string type = factory.getType(name);

	return Rcpp::wrap(type);

	END_RCPP
}

SEXP add_to_effect_container(SEXP effectContainer_, SEXP effect_, SEXP paramValue_) {
	BEGIN_RCPP

	EffectContainerManager * manager = Rcpp::XPtr<EffectContainerManager>(
			effectContainer_);
	SaomEffect * effect = Rcpp::XPtr<SaomEffect>(effect_);
	double parameter = Rcpp::as<double>(paramValue_);

	manager->addToEffectContainer(effect, parameter);

	return Rcpp::XPtr<EffectContainerManager>(manager, false);

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

	return Rcpp::XPtr<MultinomialChoiceNetworkChangeModel>(saom, false);

	END_RCPP
}

SEXP create_tie_swap_updater(SEXP networkIndex_) {
	BEGIN_RCPP

	int networkIndex = Rcpp::as<int>(networkIndex_);

	TieSwapUpdater * tieSwapUpdater =
			new TieSwapUpdater(networkIndex);

	return Rcpp::XPtr<TieSwapUpdater>(tieSwapUpdater, false);

	END_RCPP
}

SEXP create_one_mode_effect(SEXP name_, SEXP networkIndex_) {
	BEGIN_RCPP

	std::string name = Rcpp::as<std::string>(name_);
	int index = Rcpp::as<int>(networkIndex_);

	EffectFactory factory;
	NetworkEffect * effect =
			factory.getOneModeNetworkEffect(name, index);

	Rcpp::XPtr<NetworkEffect> pointer(effect, false);
	pointer.attr("class") = "OneModeNetworkEffect";

	return pointer;

	END_RCPP
}

SEXP create_attribute_one_mode_effect(SEXP name_, SEXP attributeIndex_,
		SEXP networkIndex_) {
	BEGIN_RCPP

	std::string name = Rcpp::as<std::string>(name_);
	int attributeIndex = Rcpp::as<int>(attributeIndex_);
	int networkIndex = Rcpp::as<int>(networkIndex_);

	EffectFactory factory;
	AttributeOneModeNetworkEffect * effect =
			factory.getAttributeOneModeNetworkEffect(name, attributeIndex, networkIndex);

	Rcpp::XPtr<AttributeOneModeNetworkEffect> pointer(effect, false);
	pointer.attr("class") = "AttributeOneModeNetworkEffect";

	return pointer;

	END_RCPP

}

SEXP create_similarity_attribute_one_mode_effect(SEXP name_, SEXP attributeIndex_,
		SEXP networkIndex_, SEXP meanSimilarityScores_) {
	BEGIN_RCPP

	std::string name = Rcpp::as<std::string>(name_);
	int attributeIndex = Rcpp::as<int>(attributeIndex_);
	int networkIndex = Rcpp::as<int>(networkIndex_);
	double meanSimilarityScores = Rcpp::as<double>(meanSimilarityScores_);

	EffectFactory factory;
	SimilarityAttributeOneModeNetworkEffect * effect =
			factory.getSimilarityAttributeOneModeNetworkEffect(name, attributeIndex, networkIndex, meanSimilarityScores);

	Rcpp::XPtr<SimilarityAttributeOneModeNetworkEffect> pointer(effect, false);
	pointer.attr("class") = "SimilarityAttributeOneModeNetworkEffect";

	return pointer;

	END_RCPP

}

SEXP create_attribute_effect(SEXP name_, SEXP attributeIndex_) {
	BEGIN_RCPP

	std::string name = Rcpp::as<std::string>(name_);
	int attributeIndex = Rcpp::as<int>(attributeIndex_);

	EffectFactory factory;
	AttributeEffect * effect =
			factory.getAttributeEffect(name, attributeIndex);

	Rcpp::XPtr<AttributeEffect> pointer(effect, false);
	pointer.attr("class") = "AttributeEffect";

	return pointer;

	END_RCPP

}

SEXP create_multinomial_choice_behavior_change_model(SEXP focalActorIndex_, SEXP attributeIndex_,
		SEXP effectContainer_) {
	BEGIN_RCPP

	int actorIndex = Rcpp::as<int>(focalActorIndex_);
	int attributeIndex = Rcpp::as<int>(attributeIndex_);
	EffectContainerManager * effectManager =
			Rcpp::XPtr<EffectContainerManager>(effectContainer_);



	MultinomialChoiceBehaviorChangeModel * behaviorSaom = new MultinomialChoiceBehaviorChangeModel(
			actorIndex,
			attributeIndex,
			effectManager->getEffectContainer());

	return Rcpp::XPtr<MultinomialChoiceBehaviorChangeModel>(behaviorSaom, false);

	END_RCPP
}

SEXP create_actor_attribute_set_updater(SEXP attributeIndex_, SEXP actorIndex_) {
	BEGIN_RCPP

	int attributeIndex = Rcpp::as<int>(attributeIndex_);
	int actorIndex = Rcpp::as<int>(actorIndex_);

	ActorAttributeSetUpdater * attributeSetUpdater = new ActorAttributeSetUpdater(attributeIndex, actorIndex);

	return Rcpp::XPtr<ActorAttributeSetUpdater>(attributeSetUpdater, false);

	END_RCPP
}

SEXP create_jackson_rogers_change_model(SEXP networkId_,
		SEXP pLinkToParentNode_, SEXP pLinkToNeighborNode_, SEXP nParentNodes_,
		SEXP nNeighborNodes_) {
	BEGIN_RCPP

	int networkId = Rcpp::as<int>(networkId_);
	double pLinkToParentNode = Rcpp::as<double>(pLinkToParentNode_);
	double pLinkToNeighborNode = Rcpp::as<double>(pLinkToNeighborNode_);
	int nParentNodes = Rcpp::as<int>(nParentNodes_);
	int nNeighborNodes = Rcpp::as<int>(nNeighborNodes_);

	JacksonRogers2007ChangeModel * jrModel = new JacksonRogers2007ChangeModel(networkId,
			pLinkToParentNode, pLinkToNeighborNode, nParentNodes, nNeighborNodes);

	return Rcpp::XPtr<JacksonRogers2007ChangeModel>(jrModel, false);

	END_RCPP
}

SEXP create_round_based_time_model(SEXP timerIndex_, SEXP intervalLength_,
		SEXP startTime_) {

	BEGIN_RCPP

	int timerIndex = Rcpp::as<int>(timerIndex_);
	double intervalLength = Rcpp::as<double>(intervalLength_);
	double startTime = Rcpp::as<int>(startTime_);

	RoundBasedTimeModel * rbTimeModel = new RoundBasedTimeModel(timerIndex, intervalLength, startTime);

	return Rcpp::XPtr<RoundBasedTimeModel>(rbTimeModel, false);

	END_RCPP
}

SEXP create_add_ties_from_newborn_actor_updater(SEXP networkIndex_) {
	BEGIN_RCPP

	int networkIndex = Rcpp::as<int>(networkIndex_);

	AddTiesFromNewbornActorUpdater * updater =
			new AddTiesFromNewbornActorUpdater(networkIndex);

	return Rcpp::XPtr<AddTiesFromNewbornActorUpdater>(updater, false);

	END_RCPP
}

SEXP create_timer_updater(SEXP timerIndex_) {
	BEGIN_RCPP

	int timerIndex = Rcpp::as<int>(timerIndex_);

	TimerUpdater * timerUpdater = new TimerUpdater(timerIndex);

	return Rcpp::XPtr<TimerUpdater>(timerUpdater, false);

	END_RCPP
}
