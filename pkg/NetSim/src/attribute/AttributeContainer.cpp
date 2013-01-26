/*
 * AttributeContainer.cpp
 *
 *  Created on: Nov 26, 2012
 *      Author: cws
 */

#include "AttributeContainer.h"

AttributeContainer::AttributeContainer(int n, std::vector<double> values,
		double min, double max, double by) {
	init(n, values, min, max, by);

}

AttributeContainer::AttributeContainer(int n, double value, double min,
		double max, double by) {
	std::vector<double> v(n, value);
	init(n, v, min, max, by);
}


AttributeContainer::~AttributeContainer() {
	// delete _actorValueMap;
}


bool AttributeContainer::increaseValue(int i) {
	return setValue(i, _actorValueMap[i] + _by);
}


bool AttributeContainer::decreaseValue(int i) {
	return setValue(i, _actorValueMap[i] - _by);
}


bool AttributeContainer::setValue(int i, double value) {
	bool r = false;

	// TODO check the "by" attribute
	if ( (value >= _min) & (value <= _max) ){
		_actorValueMap[i] = value;
		r = true;
	}

	return r;
}


double AttributeContainer::getValue(int i) {
	return _actorValueMap[i];
}


bool AttributeContainer::isMax(int i) {
	return ( _actorValueMap[i] == _max);
}


bool AttributeContainer::isMin(int i) {
	return ( _actorValueMap[i] == _min);
}


int AttributeContainer::getSize() {
	return _actorValueMap.size();
}

bool AttributeContainer::setAsMax(int i) {
	return setValue(i, _max);
}

bool AttributeContainer::setAsMin(int i) {
	return setValue(i, _min);
}

double AttributeContainer::getMin() {
	return _min;
}

double AttributeContainer::getMax() {
	return _max;
}

void AttributeContainer::init(int n, std::vector<double> values, double min,
	double max, double by) {
	// check validity of values
	if (by < 0 )
		throw std::invalid_argument("'by' argument cannot be < 0");
	if (max <= min)
		throw std::invalid_argument("Maximum is value smaller than minimum value");
	if ( (max - min) < by)
		throw std::invalid_argument("'by' argument is greater than max - min value");
	std::cout << max << " " << min << " "
			<< max - min << " " << fmod((max - min), by) << std::endl;
	if ( (by > 0) & (fmod( (max - min), by ) != 0.0) )
		throw std::invalid_argument("'max - min' is not a multiple of 'by' argument");

	// initialize variables
	_min = min;
	_max = max;
	_by = by;
	for (int i = 0; i < n; i++){
		if ( (values[i] < _min) | (values[i] > _max) ){
			throw std::invalid_argument("initial value out of range");
		}
		_actorValueMap.insert(
				std::map<int,double>::value_type(i, values[i])
		);
	}

}
