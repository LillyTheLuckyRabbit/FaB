/* File: deltaclock.cpp
 * Authors: David Butler and William Coar
 * Description: Clock class used to calculate delta time
*/

#include "deltaclock.h"

DeltaClock::DeltaClock() {
	prevTime = SDL_GetTicks();
	deltaTime = prevTime;
}

void DeltaClock::newFrame() {
	int currentTime = SDL_GetTicks();
	deltaTime = currentTime - prevTime;
	prevTime = currentTime;
}