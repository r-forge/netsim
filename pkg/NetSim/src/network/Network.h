/*
 * NetworkInterface.h
 *
 *  Created on: Nov 28, 2012
 *      Author: cws
 */

#ifndef NETWORK_H_
#define NETWORK_H_

/**
 * Abstract network class
 */
class Network{

public:

	// the following functions are abstract

	virtual ~Network() {}
	virtual bool addTie(int i, int j) = 0;
	virtual bool setTie(int i, int j, double value) = 0;
	virtual bool hasTie(int i, int j) = 0;
	virtual double getTieValue(int i, int j) = 0;
	virtual bool removeTie(int i, int j) = 0;
	virtual int getSize() = 0;

};


#endif /* NETWORK_H_ */
