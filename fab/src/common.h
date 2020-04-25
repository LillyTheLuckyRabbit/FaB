/* File: common.h
 * Authors: David Butler and William Coar
 * Description: Common code shared between all files
*/

#ifndef COMMON_H
#define COMMON_H

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#endif

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <algorithm>	//min/max functions
#include <math.h>		//trunc
#include <vector>		//Used by disjoint sets/terrain
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <sstream>
#include <cstring>		//Used by memcpy (according to compiler)

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::stringstream;

#endif