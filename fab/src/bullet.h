#ifndef BULLET_H
#define BULLET_H

#include "common.h"
#include "constants.h"
#include "texture.h"

class Bullet {
	public:
	Bullet(double posx, double posy, double velx = 0, double vely = 0, double direction = 0);
	~Bullet();

	Bullet randomBullet();

	private:
	int lifetime;         //How long the bullet lasts in ticks
	double speed;         //Initial bullet speed
	double acceleration;  //How much speed the bullet will gain/lose (1.0 = constant speed)
	double size;          //Hitbox of bullet (1.0 = original size, 0.5 = half size, etc.)
	double bounciness;    //Amount of speed bullet retains upon hitting walls
	int shotDelay;        //Ticks before the gun can fire another shot
	int shotCount;        //# of bullets created per shot. If "impactPlayer = true," this instead determines how many mini-projectiles the grenade bullet creates upon exploding.
	int reloadTime;       //Ticks before gun is reloaded
	int ammoDepletion;    //Ammount of ammo a gun consumes (player ammo max = 100, so ammoDepletion 34 means the gun can be fired thrice before reloading)
	double damage;        //Damage bullet deals per hitting player (player health max = 100.0)
	bool impactGround;    //If false, bullets bounces off of terrain
	bool impactPlayer;    //If false, bullets don't damage player directly (and instead act like grenades)
	double gravity;       //vertical acceleration; negative values indicate floating up (e.g., flame-thrower projectile)
};

#endif