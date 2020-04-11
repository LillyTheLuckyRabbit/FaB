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