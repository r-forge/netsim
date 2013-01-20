/*
 * ModelResult.h
 *
 *  Created on: Nov 29, 2012
 *      Author: cws
 */

#ifndef MODELRESULT_H_
#define MODELRESULT_H_

#include <string>

/**
 * A ModelResult object saves result information about a time or change model.
 * This information can subsequently used by a logger or an updater.
 */
class ModelResult{

public:
	virtual ~ModelResult(){ }
	// double getDoubleResult(){ return -1;}; // TODO is there a nicer way to define an "abstract class" with return functions that differ by type?
	// int getIntResult(){ return -1;};
	// virtual std::string getResultString() = 0;

};

class ActorModelResult : public ModelResult{

public:
	ActorModelResult(int actorIndex);

	int getActorIndex();

private:
	int _actorIndex;


};

class TieModelResult : public ModelResult{
public:
	TieModelResult(int actorIndex1, int actorIndex2);

	int getActorIndex1();

	int getActorIndex2();

private:
	int _actorIndex1;
	int _actorIndex2;


};

class TimeModelResult : public ModelResult{

public:
	TimeModelResult(double timeSpan);

	double getDoubleResult();

	std::string getResultString();


private:
	double _timeSpan;
};

// TODO: implement ActorValueModelResult
// TODO: implement ActorSetModelResult

#endif /* MODELRESULT_H_ */