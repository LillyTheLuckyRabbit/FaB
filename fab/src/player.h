#ifndef PLAYER_H
#define PLAYER_H

#include "common.h"
#include "constants.h"
#include "texture.h"
#include "terrain.h"
#include "text.h"

class Terrain;

class Player {
	public:
	Player(int num, int scoreToWin);
	Player();
	~Player();

	SDL_Rect getCamera() { return camera; }
	void halveCameraHeight();

	void inputLeftStick(const SDL_Event& e);
	void inputRBDown(const SDL_Event& e);
	void inputRBUp(const SDL_Event& e);
	void inputRightStickX(const SDL_Event& e);
	void inputRightStickY(const SDL_Event& e);
	bool inputLeftTrigger(const SDL_Event& e, Terrain& T, vector<int>& terrainUpdateList);
	void inputLBDown(const SDL_Event& e);

	void update(int deltaTime, const Terrain &T);
	int checkCollision(const Terrain &T);

	void render(int camX, int camY, int vX, int vY, bool cross);
	void renderHud(int camX, int camY, int vX, int vY);

	int getX() const { return posX; }
	int getY() const { return posY; }
	int getW() const { return width; }
	int getH() const { return height; }

	int getScore() const { return score; }
	int getHealth() const { return health; }

	private:
	int playerNumber;

	int accelX;
	int accelDir;
	int velX;
	int velY;

	int posX;
	int posY;

	int gravity;

	int width;
	int height;

	int health;
	int score;
	int goal;

	int angle = 0;
	int angleX = 1;
	int angleY = 0;

	bool grounded;
	int dashTime;
	bool dashAvail;
	bool dig;
	bool alive;

	SDL_GameController* controllerPtr;

	SDL_Rect camera;

	TextureWrapper playerTexture;
	TextureWrapper eyeTexture;
	TextureWrapper crossHair;
	TextureWrapper circle;
	TextureWrapper gun;
	Text textRenderer;
};

#endif