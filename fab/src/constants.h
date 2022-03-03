/* File: constants.h
 * Authors: David Butler and Lillian Coar
 * Description: Constants used in the game
*/

// ALSO TO NOTE:
// https://wiki.libsdl.org/SDL_GameControllerAxis
// https://wiki.libsdl.org/SDL_GameControllerButton

#ifndef CONSTANTS_H
#define CONSTANT_H

//Level constants
#define RENDER_WIDTH 1600
#define RENDER_HEIGHT 900

//Windows is weird with how it handles window rendering. Things got cut off.
//David worked around this by lowering the rendering resolution.
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define RENDER_WIDTH 1500
#define RENDER_HEIGHT 750
#endif

#define LEVEL_WIDTH 4800
#define LEVEL_HEIGHT 1800

//Player constants
#define PLAYER_HIGHGRAV 1500
#define PLAYER_LOWGRAV 700

#endif