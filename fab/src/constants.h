/* File: constants.h
 * Authors: David Butler and William Coar
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
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define RENDER_WIDTH 1200
#define RENDER_HEIGHT 675
#endif
#define LEVEL_WIDTH 4800
#define LEVEL_HEIGHT 1800

//Player constants
#define PLAYER_HIGHGRAV 1500
#define PLAYER_LOWGRAV 700

//Bullet constants
//#define MAX_LIFETIME //No max lifetime since mine projectiles could stick around until there are too many bullets on screen
#define MIN_LIFETIME 5
#define MAX_SPEED 30.0 
#define MIN_SPEED 0.0 //Prone to change, as boomerang projectiles may need negative speed
#define MAX_ACCELERATION 1.5
#define MIN_ACCELERATION 0.5
#define MAX_SIZE 10
#define MIN_SIZE 0.1
#define MAX_BOUNCINESS 1.0
#define MIN_BOUNCINESS 0.0
#define MAX_SHOTDELAY 20 //TODO: Max should be about 1 or 1.5 seconds
//#define MIN_SHOTDELAY //Not required since the min would be limited per tick anyways?
#define MAX_SHOTCOUNT 30
#define MIN_SHOTCOUNT 1
#define MAX_RELOADTIME 100 //TODO: Max reload should be ~4 seconds
#define MIN_RELOADTIME 0
#define MAX_AMMODEPLETION 100
#define MIN_AMMODEPLETION 1 //Bullets with 0 reloadTime could act as infinite ammo guns instead
#define MAX_DAMAGE 60.0 //Both min/max are rather tentative
#define MIN_DAMAGE 0.5 //
#define MAX_GRAVITY 1.0
#define MIN_GRAVITY -1.0

#endif
