#ifndef PLAYER_H
#define PLAYER_H

#include "common.h"
#include "constants.h"
#include "texture.h"

class Player {
	public:
	Player(int num);
	~Player();

	SDL_Rect getCamera() { return camera; }
	void halveCameraHeight();

	void handleEvent(const SDL_Event& e);

	void render(int camX, int camY, int vX, int vY);

	int getX() { return posX; }
	int getY() { return posY; }

	private:
	int playerNumber;

	int posX;
	int posY;

	int width;
	int height;

	SDL_Joystick* controllerPtr;

	SDL_Rect camera;

	TextureWrapper playerTexture;
};

#endif