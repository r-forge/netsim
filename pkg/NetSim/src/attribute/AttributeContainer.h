/*
 * AttributeContainer.h
 *
 *  Created on: Nov 26, 2012
 *      Author: cws
 */

#ifndef ATTRIBUTECONTAINER_H_
#define ATTRIBUTECONTAINER_H_

#include <map>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <math.h>

/**
 * Represents a container for valued attributes,
 * e.g., a value measured on a discrete scale
 */
class AttributeContainer{

public:
	AttributeContainer(int n, std::vector<double> values,
			double min = 0.0, double max = 1.0, double by = 1.0);
	AttributeContainer(int n, double value,
				double min = 0.0, double max = 1.0, double by = 1.0);

	virtual ~AttributeContainer();

	/**
	 * Increases the value by the standard size if possible given maximum value
	 */
	bool increaseValue(int i);

	/**
	 * Decreases the value by the standard size if possible given minimum value
	 */
	bool decreaseValue(int i);

	/**
	 * set attribute to max value
	 */
	bool setAsMax(int i);

	/**
	 * set attribute to minimum value
	 */
	bool setAsMin(int i);

	/**
	 *  sets the attribute value
	 *  checks if it is in the range [min, max], if not, false is returned
	 */
	bool setValue(int i, double value);

	/**
	 * Get the attribute value of actor i
	 */
	double getValue(int i);

	/**
	 * Returns if the attribute value of actor i equals the maximum
	 */
	bool isMax(int i);

	/**
	 * Returns if the attribute value of actor i equals the minimum
	 */
	bool isMin(int i);

	/**
	 * Get the number of actors
	 */
	int getSize();

	/**
	 * get minimum value
	 */
	double getMin();

	/**
	 * get maximum value
	 */
	double getMax();




private:
	double _min;
	double _max;
	double _by;
	// TODO test additional utility of boost
	// boost::unordered_map<int, double> _actorValueMap;
	std::map<int, double> _actorValueMap;

	void init(int n, std::vector<double> values,
			double min = 0.0, double max = 1.0, double by = 1.0);
};


#endif /* ATTRIBUTECONTAINER_H_ */
