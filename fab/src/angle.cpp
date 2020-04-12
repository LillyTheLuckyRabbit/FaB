/* File: angle.cpp
 * Authors: David Butler and William Coar
 * Description: This file holds functions relating to trigonometry. Specifically,
 * the functions are for getting the x and y components of vectors and finding
 * an angle based on x and y components.
*/

#include "common.h"
#include <math.h>

int getDegrees(int x, int y) {
	double degFloat;
	degFloat = atan2((double)y, (double)x) * (180 / M_PI);
	return(floor(degFloat));
}

int getXComp(int deg, int mag) {
	double xCompFloat, scalar;
	scalar = cos((double)deg * (180 / M_PI));
	xCompFloat = (double)mag * scalar;
	return(floor(xCompFloat));
}

int getYComp(int deg, int mag) {
	double yCompFloat, scalar;
	scalar = sin((double)deg * (180 / M_PI));
	yCompFloat = (double)mag * scalar;
	return(floor(yCompFloat));
}