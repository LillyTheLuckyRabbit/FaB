/* File: bullet.h
 * Authors: David Butler and William Coar
 * Description: This class contains the variables/functions used to create and move bullets.
 * Additionally, this file contains the weapons class, which handles reloading, shot delay, and passing variables to bullet.
*/

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
	Bullet(int pNum, int iPosX, int iPosY, int iVelx, int iVelY, string texturePath, Mix_Chunk* iSound, int iDam, int iGrav, double iAX, int iRad, bool iTime, int iLifetime, double iBounce, int iNumBounce, bool iPlayer);
	bool update(int deltaTime, Terrain& T, Player* (&players)[4], int numPlayers);  //return 'true' to delete
	bool update(int deltaTime, vector<int>& terrainUpdateList, Terrain& T, Player* (&players)[4], int numPlayers);
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
	double accelX;  //Horizontal acceleration
	int gravity;    //vertical acceleration; negative values indicate floating up (e.g., flame-thrower projectile)
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
	Mix_Chunk* impactSound;
};

class Weapon {
	public:
	Weapon(string name, int iAmmo, int iReloadTime, int iShotTime, int iVel, string iBTexture, string iFireSound, string iImpactSound, int iDam, int iGrav = 0, double iAX = 0, int iRad = 4, bool iTime = false, int iLifetime = 0, double iBounce = 1.0, int iNumBounce = 0, bool iPlayer = true, int spread = 0, int count = 1, bool iOneSound = false);
	~Weapon();
	int getAmmo() { return ammo; }
	int getTotal() { return totalAmmo; }
	void shoot(list<Bullet>& bulletList, int playerNum, int angle, int pCenterX, int pCenterY);
	void update(int deltaTime);
	bool isReloading();
	string getName() const { return weaponName; }
	int getSpread() const { return spread; }
	int getCount() const { return count; }

	void stopFireSound();
	int fireSoundChnl;
	Mix_Chunk* fireSound;

	private:
	string weaponName;
	bool oneSound;
	int ammo;
	int totalAmmo;
	int reloadTime;
	int shotTime;  //Time between shots
	int fireVel;

	int currentReloadTime;
	int currentShotTime;

	// Stats which will be passed on to the bullet
	string bulletTexturePath;
	Mix_Chunk* impactSound;
	double accelX;      //Horizontal acceleration. accelX == 1.0 = same speed, accelX < 1.0 = slows down.
	int gravity;        //vertical acceleration; negative values indicate floating up (e.g., flame-thrower projectile)
	int damage;         //Damage bullet deals per hitting player (player health max = 100.0)
	int radius;         //Radius of terrain affected by the bullet
	bool timerBullet;   //Whether or not to delete the bullet after a certain amount of time
	int lifetime;       //How long the bullet lasts in ticks
	double bounciness;  //Amount of speed bullet retains upon hitting walls
	int numBounces;     //Number of bounces off of terrain before detroyed (-1 = infinite)
	bool impactPlayer;  //If false, bullets aren't destroyed upon impacting the player
	int spread;         //Random bullet spread in degrees (0 = no spread)
	int count;          //How many bullets are shot at once
};

#endif