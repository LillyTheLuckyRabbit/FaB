/* File: deltaclock.h
   Author: Lillian Coar and David Butler
   Description: Header for DeltaClock class.
*/
#ifndef DELTACLOCK_H
#define DELTACLOCK_H

#include "common.h"

class DeltaClock {
	public:
	DeltaClock();
	int getDelta() { return deltaTime; }
	void newFrame();

	private:
	int prevTime;
	int deltaTime;
};

#endif