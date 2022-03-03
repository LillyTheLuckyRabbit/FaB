/* File: bullet.cpp
 * Authors: David Butler and Lillian Coar
 * Description: This class contains the variables/functions used to create and move bullets.
 * Additionally, this file contains the weapons class, which handles reloading, shot delay, and passing variables to bullet.
*/

#include "bullet.h"

//Parameter constructor
Bullet::Bullet(int pNum, int iPosX, int iPosY, int iVelx, int iVelY, string texturePath, Mix_Chunk* iSound, int iDam, int iGrav, double iAX, int iRad, bool iTime, int iLifetime, double iBounce, int iNumBounce, bool iPlayer) {
	playerNum = pNum;
	posX = iPosX;
	posY = iPosY;
	velX = iVelx;
	velY = iVelY;
	damage = iDam;
	gravity = iGrav;
	accelX = iAX;
	radius = iRad;
	timerBullet = iTime;
	lifetime = iLifetime;
	bounciness = iBounce;
	numBounces = iNumBounce;
	impactPlayer = iPlayer;

	impactSound = iSound;

	if(!bulletTexture.loadFromFile(texturePath)) {
		cout << "Failed to load bullet texture!" << endl;
		width = 0;
		height = 0;
	} else {
		width = bulletTexture.getWidth();
		height = bulletTexture.getHeight();
	}
}

//Bullet update function. When returning true, the bullet has hit something and is deleted.
bool Bullet::update(int deltaTime, vector<int>& terrainUpdateList, Terrain& T, Player* (&players)[4], int numPlayers) {
	bool exitCondition = false;
	bool hitPlayer = false;
	bool collidesWithTerrain = false;

	//If the bullet is at the edge of the level, treat it like it collided with terrain
	if((posX < 0 || posY < 0) || (posX > LEVEL_WIDTH || posY > LEVEL_HEIGHT)) {
		collidesWithTerrain = true;
	}

	//Calculate X of displacement
	int deltaX = 0;
	velX = trunc(velX + deltaTime / 1000.0);
	if(velX) {
		velX = trunc(velX * accelX);
		deltaX = trunc((velX * deltaTime) / 1000.0);
		posX += deltaX;
	}

	//Calculate Y of displacement
	int deltaY = trunc(((velY * deltaTime) + .5 * gravity * (deltaTime * deltaTime) / 1000.0) / 1000.0);
	posY += deltaY;
	velY = velY + (gravity * deltaTime / 1000.0);

	//Check collision with players
	for(int i = 0; i < numPlayers; i++) {
		if(i != playerNum - 1) {
			if(posX + width >= players[i]->getX() && posX <= players[i]->getX() + players[i]->getW()) {
				if(posY + height >= players[i]->getY() && posY <= players[i]->getY() + players[i]->getH()) {
					if(players[i]->getAlive()) {
						players[i]->doDamage(damage);
						hitPlayer = true;
						if(players[i]->getHealth() <= 0) {
							players[playerNum - 1]->incrementScore();
						}
						if(impactPlayer) {
							exitCondition = true;
						}
						//Pretend we collided with terrain so we can calculate blast radius later
						collidesWithTerrain = true;
					}
				}
			}
		}
	}

	//Check collision with terrain
	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			if(T.getValueAtXY(posX + x, posY + y) == 1) {
				collidesWithTerrain = true;
				break;
			}
		}
		if(collidesWithTerrain) break;
	}

	//If we collided with terrain
	if(collidesWithTerrain) {
		if(Mix_PlayChannel(-1, impactSound, 0) == -1) {
			cout << "Failed to play impact sound! Mix error: " << Mix_GetError() << endl;
		}

		//Change the terrain
		int centerX = (width / 2) + posX;
		int centerY = (height / 2) + posY;
		for(int y = -radius; y <= radius; y++) {
			for(int x = -radius; x <= radius; x++) {
				if(x * x + y * y < radius * radius) {
					if(T.setValueAtXY(centerX + x, centerY + y, 0)) {
						terrainUpdateList.push_back((centerY + y) * LEVEL_WIDTH + (centerX + x));
					}
				}
			}
		}

		//Check the blast radius to see if we got any players
		if(!hitPlayer) {
			for(int i = 0; i < numPlayers; i++) {
				if(i != playerNum - 1) {
					int pDistanceX = (players[i]->getW() / 2 + players[i]->getX()) - centerX;
					int pDistanceY = (players[i]->getH() / 2 + players[i]->getY()) - centerY;
					if(pDistanceX * pDistanceX + pDistanceY * pDistanceY < radius * radius) {
						if(players[i]->getAlive()) {
							players[i]->doDamage(damage);
							if(players[i]->getHealth() <= 0) {
								players[playerNum - 1]->incrementScore();
							}
						}
					}
				}
			}
		}

		//Do we bounce?
		if(numBounces == 0) {
			exitCondition = true;
		} else {
			velX *= -bounciness;
			velY *= -bounciness;
			numBounces--;
		}
	}

	//Has the timer run out?
	if(timerBullet) {  //Bullet won't last forever, check if it should expire.
		lifetime--;
		if(lifetime < 0) exitCondition = true;  //delete boolit
	}

	//If true, delete the bullet
	return (exitCondition);
}

//The bullet function renders the bullet. It rotates the bullet, but you don't notice
//because all of our sprites are round pellets right now.
void Bullet::render(int camX, int camY, int vX, int vY) {
	SDL_Point bulletCenter;
	bulletCenter.x = width / 2;
	bulletCenter.y = height / 2;
	bulletTexture.render((posX - camX) + vX, (posY - camY) + vY, NULL, getDegrees(velX, velY), &bulletCenter);
}

//Reverses the bullet's direction of velocity (such as when the bullet gets reflected)
void Bullet::reverseVel(int pNum) {
	if(playerNum != pNum) {
		playerNum = pNum;
		velX = -velX;
		velY = -velY;
	}
}

//Paramter constructor
Weapon::Weapon(string name, int iAmmo, int iReloadTime, int iShotTime, int iVel, string iBTexture, string iFireSound, string iImpactSound, int iDam, int iGrav, double iAX, int iRad, bool iTime, int iLifetime, double iBounce, int iNumBounce, bool iPlayer, int iSpread, int iCount, bool iOneSound) {
	weaponName = name;
	totalAmmo = iAmmo;
	ammo = iAmmo;
	reloadTime = iReloadTime;
	shotTime = iShotTime;
	damage = iDam;
	gravity = iGrav;
	accelX = iAX;
	radius = iRad;
	timerBullet = iTime;
	lifetime = iLifetime;
	bounciness = iBounce;
	numBounces = iNumBounce;
	impactPlayer = iPlayer;
	fireVel = iVel;
	bulletTexturePath = iBTexture;
	spread = iSpread;
	count = iCount;
	impactSound = Mix_LoadWAV(iImpactSound.c_str());
	oneSound = iOneSound;
	fireSound = Mix_LoadWAV(iFireSound.c_str());
	if(fireSound == NULL) {
		cout << "Failed to load weapon firing sound '" << iFireSound << "'! Mix Error:" << Mix_GetError() << endl;
	}
	fireSoundChnl = -1;

	currentReloadTime = reloadTime;
	currentShotTime = shotTime;
}

//Weapon destructor for game end
Weapon::~Weapon() {
	if(fireSound) {
		Mix_FreeChunk(fireSound);
		fireSound = NULL;
		fireSoundChnl = -1;
	}
	if(impactSound) {
		Mix_FreeChunk(impactSound);
		impactSound = NULL;
	}
}

//This function creates a bullet
void Weapon::shoot(list<Bullet>& bulletList, int playerNum, int angle, int pCenterX, int pCenterY) {
	if(currentShotTime == shotTime && ammo) {
		int velX = getXComp(angle, fireVel);
		int velY = getYComp(angle, fireVel);
		int posX = getXComp(angle, 30) + pCenterX;
		int posY = getYComp(angle, 30) + pCenterY;
		bulletList.emplace_back(playerNum, posX, posY, velX, velY, bulletTexturePath, impactSound, damage, gravity, accelX, radius, timerBullet, lifetime, bounciness, numBounces, impactPlayer);
		currentShotTime -= 1;
		ammo--;
		if(fireSoundChnl == -1 || !oneSound) {
			fireSoundChnl = Mix_PlayChannel(-1, fireSound, 0);
		}
	}
}

//If we have a weapon with one continuous sound (like the flamethrower) we need to
//stop the sound when we run out of ammo of when we stop firing.
void Weapon::stopFireSound() {
	if(oneSound) {
		if(fireSoundChnl != -1) {
			Mix_HaltChannel(fireSoundChnl);
			fireSoundChnl = -1;
		}
	}
}

//Weapon update function. Also handles reloading and shot delay.
void Weapon::update(int deltaTime) {
	if(!ammo) {
		currentReloadTime -= deltaTime;
		stopFireSound();
	}

	if(currentShotTime < shotTime) currentShotTime -= deltaTime;

	if(currentReloadTime < 0) {
		ammo = totalAmmo;
		currentReloadTime = reloadTime;
	}

	if(currentShotTime < 0) {
		currentShotTime = shotTime;
	}
}

//Could've probably shortened this to "return(!ammo)" but I didn't write this one
bool Weapon::isReloading() {
	if(!ammo) {
		return true;
	}
	return false;
}