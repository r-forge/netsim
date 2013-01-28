/*
 * ValuedMemoryOneModeNetwork.h
 *
 *  Created on: Jan 28, 2013
 *      Author: cws
 */

#ifndef VALUEDMEMORYONEMODENETWORK_H_
#define VALUEDMEMORYONEMODENETWORK_H_

#include "MemoryOneModeNetwork.h"
#include <vector>
#include <utility>
#include <map>
#include <set>

class ValuedMemoryOneModeNetwork : public MemoryOneModeNetwork{

public:

	ValuedMemoryOneModeNetwork(int size, bool directed=true, bool reflexive = false);
	ValuedMemoryOneModeNetwork(std::vector<std::vector<double> > graph, bool directed=true,
			bool reflexive = false);


	/**
	 * Returns the value of the tie with maximum value below a threshold
	 * or @threshold if no such value is found
	 */
	double getMaxTieValueBelow(double threshold);

	/**
	 * Returns the value of the tie with minimum value above a threshold
	 * or @threshold if no such value is found
	 */
	double getMinTieValueAbove(double threshold);


private:
	// edge representation of tie values (positive and zero)
	// multiple variables are stored
	std::map<std::pair<int, int>, double > _tieValueMap;
	std::map<double, std::set<std::pair<int, int> > > _valueTiesMap;

	void init();

	void updateInternalRepresentation(int i, int j,
			double oldValue, double newValue);

};


#endif /* VALUEDMEMORYONEMODENETWORK_H_ */
