#ifndef PLAYER_H
#define PLAYER_H

#include "bullet.h"
#include "common.h"
#include "constants.h"
#include "terrain.h"
#include "text.h"
#include "texture.h"

class Terrain;
class Weapon;
class Bullet;

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
	void inputRightTrigger(const SDL_Event& e, vector<Bullet>& bulletVec);

	bool update(int deltaTime, const Terrain& T, vector<Bullet>& bulletVec);
	int checkCollision(const Terrain& T);

	void render(int camX, int camY, int vX, int vY, bool cross);
	void renderHud(int camX, int camY, int vX, int vY);

	int getX() const { return posX; }
	int getY() const { return posY; }
	int getW() const { return width; }
	int getH() const { return height; }

	int getScore() const { return score; }
	int getHealth() const { return health; }
	bool getAlive() const { return alive; }

	void doDamage(int damage) { health -= damage; }
	void incrementScore() { score++; }

	void switchWeapon(int wepNum);

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
	bool shoot;
	bool alive;
	int respawnTime;

	vector<Weapon> weaponInv;
	int currentWeapon = 0;

	SDL_GameController* controllerPtr;

	SDL_Rect camera;

	TextureWrapper playerTexture;
	TextureWrapper eyeTexture;
	TextureWrapper crossHair;
	TextureWrapper circle;
	TextureWrapper gun;
	TextureWrapper calamari;
	Text textRenderer;
};

#endif