/*
 * AttributeContainer.cpp
 *
 *  Created on: Nov 26, 2012
 *      Author: cws
 */

#include "AttributeContainer.h"

AttributeContainer::AttributeContainer(std::vector<double> values,
		double min, double max, double by) {
	init(values, min, max, by);

}

AttributeContainer::AttributeContainer(int n, double value, double min,
		double max, double by) {
	std::vector<double> v(n, value);
	init(v, min, max, by);
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

	// check that value is a multiple of the by attribute + the min value
	double checkedValue = (round((value - _min) / _by) * _by) + _min;

	if (value != checkedValue)
		std::cout << "Incorrect attribute value: " << value << std::endl;

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

double AttributeContainer::getBy() {
	return _by;
}

AttributeContainerMemento* AttributeContainer::saveToMemento() {
	AttributeContainerMemento * memento = new AttributeContainerMemento();
	memento->setActorValueMap(_actorValueMap);
	return memento;
}

void AttributeContainer::restroreFromMemento(
		AttributeContainerMemento* memento) {
	_actorValueMap = memento->getActorValueMap();
}

void AttributeContainer::init(std::vector<double> values, double min,
	double max, double by) {
	// check validity of values
	if (by < 0 )
		throw std::invalid_argument("'by' argument cannot be < 0");
	if (max <= min)
		throw std::invalid_argument("Maximum value is not greater than minimum value");
	if ( (max - min) < by)
		throw std::invalid_argument("'by' argument is greater than max - min value");
	if ( (by > 0) & (fmod( (max - min), by ) != 0.0) )
		throw std::invalid_argument("'max - min' is not a multiple of 'by' argument");

	// initialize variables
	_min = min;
	_max = max;
	_by = by;
	for (int i = 0; i < values.size(); i++){
		if ( (values[i] < _min) | (values[i] > _max) ){
			throw std::invalid_argument("initial value out of range");
		}
		setValue(i, values[i]);
	}

}

void AttributeContainerMemento::setActorValueMap(
		std::map<int, double> actorValueMap) {
	_actorValueMap = actorValueMap;
}

std::map<int, double> AttributeContainerMemento::getActorValueMap() {
	return _actorValueMap;
}
