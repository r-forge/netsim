/*
 * EffectTest.h
 *
 *  Created on: Jan 9, 2013
 *      Author: cws
 */

#ifndef EFFECTTEST_H_
#define EFFECTTEST_H_

#include "../../model/saom/NetworkEffect.h"
#include "../../processstate/ProcessState.h"
#include "../../network/MemoryOneModeNetwork.h"


MemoryOneModeNetwork getSimpleNetwork(){
	MemoryOneModeNetwork network(5);
	network.addTie(0,1);
	network.addTie(0,2);
	network.addTie(0,3);
	return network;
}

MemoryOneModeNetwork getSimpleNetwork2(){
	MemoryOneModeNetwork network(5);
	network.addTie(0,1);
	network.addTie(1,2);
	network.addTie(2,3);
	network.addTie(0,2);
	network.addTie(3,4);
	network.addTie(2,4);
	return network;
}


void densityEffectTest(){
	ProcessState processState;
	MemoryOneModeNetwork network = getSimpleNetwork();
	size_t indexNetwork = processState.addNetwork(&network);

	DensityEffect densityEffect(indexNetwork);
	ASSERT_EQUAL(3, densityEffect.getEffect(&processState, 0));
	ASSERT_EQUAL(0, densityEffect.getEffect(&processState, 1));
}


void twoPathEffectTest(){
	ProcessState processState;
	MemoryOneModeNetwork network = getSimpleNetwork2();
	size_t indexNetwork = processState.addNetwork(&network);

	TwoPathEffect twoPathEffect(indexNetwork);
	ASSERT_EQUAL(2, twoPathEffect.getEffect(&processState, 0));
	ASSERT_EQUAL(2, twoPathEffect.getEffect(&processState, 1));
	ASSERT_EQUAL(0, twoPathEffect.getEffect(&processState, 2));
	// repeat to make sure that the internal sets are not changed
	ASSERT_EQUAL(2, twoPathEffect.getEffect(&processState, 0));
	ASSERT_EQUAL(2, twoPathEffect.getEffect(&processState, 1));
	ASSERT_EQUAL(0, twoPathEffect.getEffect(&processState, 2));

}


cute::suite getTestSaomEffectsSuite(){
	cute::suite s;

	s.push_back(CUTE(densityEffectTest));
	s.push_back(CUTE(twoPathEffectTest));

	return s;
}


#endif /* EFFECTTEST_H_ */
