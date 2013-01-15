/*
 * Random.cpp
 *
 *  Created on: Nov 30, 2012
 *      Author: cws
 */

#include "Random.h"

Random& Random::getInstance(int seed) {
    // The only instance
    // Guaranteed to be lazy initialized
    // Guaranteed that it will be destroyed correctly
    static Random instance(seed);
    return instance;
}

double Random::getRandom() {
	double randomNumber = ( (double)rand() / (double)RAND_MAX );
	return randomNumber;
}

size_t Random::getRandMax() {
	return RAND_MAX;
}

Random::Random(int seed) {
	//TODO: Is the generator REALLY only initialized once?
    srand(seed);
    _seed = seed;
}

Random::Random(){
	std::cerr << "Random() should have not been called";
	_seed = -1; // should never be called
}
