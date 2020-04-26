#ifndef BULLET_H
#define BULLET_H

#include "angle.h"
#include "common.h"
#include "constants.h"
#include "player.h"
#include "terrain.h"
#include "texture.h"

class Player;
class Terrain;

class Bullet {
	public:
	Bullet(int pNum, int iPosX, int iPosY, int iVelx, int iVelY, string texturePath, int iDam, int iGrav, int iAX, int iRad, bool iTime, int iLifetime, double iBounce, int iNumBounce, bool iPlayer);
	bool update(int deltaTime, Terrain& T, Player* (&players)[4], int numPlayers);  //return 'true' to delete
	bool update(int deltaTime, vector<int> &terrainUpdateList, Terrain& T, Player* (&players)[4], int numPlayers);
	void render(int camX, int camY, int vX, int vY);
	int getX() { return posX; }
	int getY() { return posY; }
	int getH() { return height; }
	int getW() { return width; }
	void reverseVel(int pNum);

	private:
	int posX;
	int posY;
	int velX;
	int velY;
	int accelX;   //Horizontal acceleration
	int gravity;  //vertical acceleration; negative values indicate floating up (e.g., flame-thrower projectile)
	int width;
	int height;

	int damage;         //Damage bullet deals per hitting player (player health max = 100.0)
	int radius;         //Radius of terrain affected by the bullet
	bool timerBullet;   //Whether or not to delete the bullet after a certain amount of time
	int lifetime;       //How long the bullet lasts in ticks
	double bounciness;  //Amount of speed bullet retains upon hitting walls
	int numBounces;     //Number of bounces off of terrain before detroyed (-1 = infinite)
	bool impactPlayer;  //If false, bullets aren't destroyed upon impacting the player
	int playerNum;      //Player the bullet came from/player who reflected the bullet

	TextureWrapper bulletTexture;
};

class Weapon {
	public:
	Weapon(string name, int iAmmo, int iReloadTime, int iShotTime, int iVel, string iBTexture, int iDam, int iGrav = 0, int iAX = 0, int iRad = 4, bool iTime = false, int iLifetime = 0, double iBounce = 1.0, int iNumBounce = 0, bool iPlayer = true);
	int getAmmo() { return ammo; }
	int getTotal() { return totalAmmo; }
	void shoot(vector<Bullet>& bulletVec, int playerNum, int angle, int pCenterX, int pCenterY);
	void update(int deltaTime);
	bool isReloading();
	string getName() const { return weaponName; }

	private:
	string weaponName;
	int ammo;
	int totalAmmo;
	int reloadTime;
	int shotTime;  //Time between shots
	int fireVel;

	int currentReloadTime;
	int currentShotTime;

	// Stats which will be passed on to the bullet
	string bulletTexturePath;
	int accelX;         //Horizontal acceleration
	int gravity;        //vertical acceleration; negative values indicate floating up (e.g., flame-thrower projectile)
	int damage;         //Damage bullet deals per hitting player (player health max = 100.0)
	int radius;         //Radius of terrain affected by the bullet
	bool timerBullet;   //Whether or not to delete the bullet after a certain amount of time
	int lifetime;       //How long the bullet lasts in ticks
	double bounciness;  //Amount of speed bullet retains upon hitting walls
	int numBounces;     //Number of bounces off of terrain before detroyed (-1 = infinite)
	bool impactPlayer;  //If false, bullets aren't destroyed upon impacting the player
};

#endif