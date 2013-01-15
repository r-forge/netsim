/*
 * ModelManager.h
 *
 *  Created on: Nov 29, 2012
 *      Author: cws
 */

#ifndef MODELMANAGER_H_
#define MODELMANAGER_H_

#include <string>
#include <boost/unordered_map.hpp>
#include "TimeModel.h"
#include "Updater.h"
#include "ChangeModel.h"
#include "Updater.h"

/**
 * manages different set of models and links these to one another:
 * * Each TimeModel can be linked to several ChangeModel's
 * * Each ChangeModel can be linked to several Updater's
 */
class ModelManager{

public:

	ModelManager(){
		// TODO implement in separate file?
	}

	/**
	 * @Deprecated
	 */
	void addModelSequence(){
	}

	void addTimeUpdater(TimeUpdater * timeUpdater){
		_timeUpdaters.push_back(timeUpdater);
	}

	void addTimeModel(TimeModel * timeModel){
		_timeModels.push_back(timeModel);
	}

	void addChangeModel(TimeModel * timeModel, ChangeModel * changeModel){
		_timeModelChangeModelMap[timeModel].push_back(changeModel);
	}

	void addUpdater(ChangeModel * changeModel, Updater * updater){
		_changeModelChangeUpdaterMap[changeModel].push_back(updater);
	}


	std::vector<TimeModel*> getTimeModels(){
		return _timeModels;
	}

	/**
	 * Get all TimeUpdater objects.
	 * These are all time dependent network changes.
	 */
	std::vector<TimeUpdater*> getTimeUpdater(){
		return _timeUpdaters;
	}

	/**
	 * Get the change model based on the index determined
	 * by a comparison of time models
	 */
	std::vector<ChangeModel*> getChangeModels(TimeModel* timeModel){
		// returns default value if lookup is not successful
		return _timeModelChangeModelMap[timeModel];
	}

	/**
	 * Get the change updater based on the index determined
	 * by a comparison of time models
	 */
	std::vector<Updater*> getUpaters(ChangeModel* changeModel){
		return _changeModelChangeUpdaterMap[changeModel];
	}

	std::string getLatestMessage(){
		return " ";
	}

private:

	std::vector<TimeUpdater*> _timeUpdaters;

	std::vector<TimeModel*>_timeModels;
	boost::unordered_map<TimeModel*, std::vector<ChangeModel*> > _timeModelChangeModelMap;
	boost::unordered_map<ChangeModel*, std::vector<Updater*> > _changeModelChangeUpdaterMap;

	// std::vector<ChangeModel*> _changeModels;
	// std::vector<Updater*> _updaters;
};


#endif /* MODELMANAGER_H_ */
