/* File: bullet.cpp
 * Authors: David Butler and William Coar
 * Description: This class contains the variables/functions used to create and move bullets.
*/

#include "bullet.h"

Bullet::Bullet(int pNum, int iPosX, int iPosY, int iVelx, int iVelY, string texturePath, int iDam, int iGrav, double iAX, int iRad, bool iTime, int iLifetime, double iBounce, int iNumBounce, bool iPlayer) {
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

	if(!bulletTexture.loadFromFile(texturePath)) {
		cout << "Failed to load bullet texture!" << endl;
		width = 0;
		height = 0;
	} else {
		width = bulletTexture.getWidth();
		height = bulletTexture.getHeight();
	}
}

bool Bullet::update(int deltaTime, vector<int> &terrainUpdateList, Terrain& T, Player* (&players)[4], int numPlayers) {
	int deltaX = 0;
	velX = trunc(velX + deltaTime / 1000.0);
	if(velX) {
		velX = trunc(velX * accelX);
		deltaX = trunc((velX * deltaTime) / 1000.0);
		posX += deltaX;
	}

	int deltaY = trunc(((velY * deltaTime) + .5 * gravity * (deltaTime * deltaTime) / 1000.0) / 1000.0);
	posY += deltaY;
	velY = velY + (gravity * deltaTime / 1000.0);

	for(int i = 0; i < numPlayers; i++) {
		if(i != playerNum - 1) {
			if(posX + width >= players[i]->getX() && posX <= players[i]->getX() + players[i]->getW()) {
				if(posY + height >= players[i]->getY() && posY <= players[i]->getY() + players[i]->getH()) {
					if(players[i]->getAlive()) {
						players[i]->doDamage(damage);
						if(players[i]->getHealth() <= 0) {
							players[playerNum-1]->incrementScore();
						}
						if(impactPlayer) {
							return true;
						}
					}
				}
			}
		}
	}
	bool collidesWithTerrain = false;	
	int centerX = (width / 2) + posX;
	int centerY = (height / 2) + posY;
	for(int y = -radius; y <= radius; y++) {
		for(int x = -radius; x <= radius; x++) {
			if(x * x + y * y < radius * radius) {
				if(T.getValueAtXY(centerX + x, centerY + y) == 1) {
					collidesWithTerrain = true;
					break;
				}
			}
		}
		if(collidesWithTerrain) break;
	}
	if(collidesWithTerrain) {
		for(int y = -radius; y <= radius; y++) {
			for(int x = -radius; x <= radius; x++) {
				if(x * x + y * y < radius * radius) {
					if(T.setValueAtXY(centerX + x, centerY + y, 0)) {
						terrainUpdateList.push_back((centerY + y) * LEVEL_WIDTH + (centerX + x));
					}
				}
			}
		}
		if(numBounces == 0) {
			return(true);
		} else {
			velX *= -bounciness;
			velY *= -bounciness;
			numBounces--;
		}
	}
	if(timerBullet){//Bullet won't last forever, check if it should expire.
		lifetime--;
		if (lifetime < 0) return true; //delete boolit
	}

	if((posX < 0 || posY < 0) || (posX > LEVEL_WIDTH || posY > LEVEL_HEIGHT)) {
		return(true);
	}

	return(false);
}

void Bullet::render(int camX, int camY, int vX, int vY) {
	SDL_Point bulletCenter;
	bulletCenter.x = width / 2;
	bulletCenter.y = height / 2;
	bulletTexture.render((posX - camX) + vX, (posY - camY) + vY, NULL, getDegrees(velX, velY), &bulletCenter);
}

void Bullet::reverseVel(int pNum) {
	playerNum = pNum;
	velX = -velX;
	velY = -velY;
}

Weapon::Weapon(string name, int iAmmo, int iReloadTime, int iShotTime, int iVel, string iBTexture, int iDam, int iGrav, double iAX, int iRad, bool iTime, int iLifetime, double iBounce, int iNumBounce, bool iPlayer, int iSpread, int iCount) {
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
	//carly = iCarly; B)

	currentReloadTime = reloadTime;
	currentShotTime = shotTime;
	
}

void Weapon::shoot(vector<Bullet>& bulletVec, int playerNum, int angle, int pCenterX, int pCenterY) {
	if(currentShotTime == shotTime && ammo) {
		int velX = getXComp(angle, fireVel);
		int velY = getYComp(angle, fireVel);
		int posX = getXComp(angle, 30) + pCenterX;
		int posY = getYComp(angle, 30) + pCenterY;
		bulletVec.emplace_back(playerNum, posX, posY, velX, velY, bulletTexturePath, damage, gravity, accelX, radius, timerBullet, lifetime, bounciness, numBounces, impactPlayer);
		currentShotTime -= 1;
		ammo--;
	}
}

void Weapon::update(int deltaTime) {
	if(!ammo) currentReloadTime -= deltaTime;
	if(currentShotTime < shotTime) currentShotTime -= deltaTime;

	if(currentReloadTime < 0) {
		ammo = totalAmmo;
		currentReloadTime = reloadTime;
	}

	if(currentShotTime < 0) {
		currentShotTime = shotTime;
	}
}

bool Weapon::isReloading() {
	if(!ammo) {
		return true;
	}
	return false;
}