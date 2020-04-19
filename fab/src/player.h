#ifndef PLAYER_H
#define PLAYER_H

#include "common.h"
#include "constants.h"
#include "texture.h"

class Player {
	public:
	Player(int num);
	Player();
	~Player();

	SDL_Rect getCamera() { return camera; }
	void halveCameraHeight();

	void inputLeftStick(const SDL_Event& e);
	void inputRBDown(const SDL_Event& e);
	void inputRBUp(const SDL_Event& e);
	void inputRightStickX(const SDL_Event& e);
	void inputRightStickY(const SDL_Event& e);

	void update(int deltaTime);

	void render(int camX, int camY, int vX, int vY);

	int getX() const { return posX; }
	int getY() const { return posY; }
	int getW() const { return width; }
	int getH() const { return height; }

	private:
	int playerNumber;

	int velX;
	int velY;

	int posX;
	int posY;

	int gravity;

	int width;
	int height;

	int health;

	int angle = 0;
	int angleX = 1;
	int angleY = 0;

	SDL_GameController* controllerPtr;

	SDL_Rect camera;

	TextureWrapper playerTexture;
	TextureWrapper eyeTexture;
};

#endif