/* File: angle.cpp
 * Authors: David Butler and Lillian Coar
 * Description: This file holds functions relating to trigonometry. Specifically,
 * the functions are for getting the x and y components of vectors and finding
 * an angle based on x and y components.
*/

#include "angle.h"
#include "common.h"
#include <math.h>

/* Returns angle in degrees
 *
 *  Quick crash course on SDL angles:
 * From left to right,
 * -180 to 0 is the top hemisphere,
 * 180 to 0 is the bottome hemisphere.
*/
int getDegrees(int x, int y) {
	double degFloat;
	degFloat = atan2((double)y, (double)x) * (180 / M_PI);
	return(trunc(degFloat));
}

// Returns X component of a vector when given its angle and magnitude
int getXComp(int deg, int mag) {
	double xCompFloat, scalar;
	scalar = cos((double)deg * (M_PI / 180));
	xCompFloat = (double)mag * scalar;
	return(trunc(xCompFloat));
}

// Returns Y component of a vector when given its angle and magnitude
int getYComp(int deg, int mag) {
	double yCompFloat, scalar;
	scalar = sin((double)deg * (M_PI / 180));
	yCompFloat = (double)mag * scalar;
	return(trunc(yCompFloat));
}