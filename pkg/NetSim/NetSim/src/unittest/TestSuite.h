/*
 *
 *  Created on: Nov 21, 2012
 *      Author: cws
 */

#ifndef TESTSUITE_H_
#define TESTSUITE_H_

#include "cute.h"
#include "ide_listener.h"
#include "cute_runner.h"
#include "../attribute/AttributeContainer.h"
#include "test0001_network/MemoryOneModeNetworkTest.h"
#include "test0002_attribute/AttributeContainerTest.h"
#include "test0003_saom/SaomTest.h"
#include "test0004_effects/EffectTest.h"



void runSuite(){
	cute::suite s0001 = getTestSuiteMemoryOneModeNetwork();
	cute::suite s0002 = getTestAttributeContainerSuite();
	cute::suite s0003 = getTestSaomSuite();
	cute::suite s0004 = getTestSaomEffectsSuite();
	cute::ide_listener idl;
	cute::makeRunner(idl)(s0001, "Network tests");
	cute::makeRunner(idl)(s0002, "Attribute Container tests");
	cute::makeRunner(idl)(s0003, "SAOM tests");
	cute::makeRunner(idl)(s0004, "SAOM Effect tests");

}






#endif /* TESTSUITE_H_ */
