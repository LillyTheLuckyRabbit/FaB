/* File: deltaclock.cpp
 * Authors: David Butler and William Coar
 * Description: Clock class used to calculate delta time. This keeps the game
 * consistent regardless of frame rate or monitor refresh rate.
*/

#include "deltaclock.h"

//Gets the current delta time
DeltaClock::DeltaClock() {
	prevTime = SDL_GetTicks();
	deltaTime = prevTime;
}

//Run at the beginning of every frame to set the delta time
void DeltaClock::newFrame() {
	int currentTime = SDL_GetTicks();
	deltaTime = currentTime - prevTime;
	prevTime = currentTime;
}