/* File: bullet.cpp
 * Authors: David Butler and William Coar
 * Description: This class contains the variables/functions used to create and move bullets.
*/

#include "bullet.h"

Bullet::Bullet(double posx, double posy, double velx, double vely, double direction) {

}

Bullet::~Bullet() {
	
}

/*void Bullet::handleEvent(const SDL_Event& e) {
	if(e.jaxis.value > 0) {
		posX += 100;
	}
	if(e.jaxis.value < 0) {
		posX -= 100;
	}
	if(posX - camera.x < camera.w / 3 && camera.x > 100) {
		camera.x -= 100;
	}
	if(posX - camera.x > 2 * (camera.w / 3) && camera.x < (LEVEL_WIDTH - camera.w - 100)) {
		camera.x += 100;
	}
	//Keep the player's position in bounds
	if (posX > LEVEL_WIDTH) posX = LEVEL_WIDTH;
	if (posY > LEVEL_HEIGHT) posY = LEVEL_HEIGHT;
	if (posX < 0) posX = 0;
	if (posY < 0) posY = 0;
}*/

/* randomBullet creates a bullet with random properties, then returns the new bullet.
 * Certain bullet properties are instead calculated by other properties.
 * For instance, a bullet with "impactGround = false" might deal less damage
 * than its non-bouncy counterparts, but may also have increased lifetime.
Bullet Bullet::randomBullet(){
	Bullet newBullet;
	//Generate random properties
	//TODO: give random properties to all variables under this subsection
	newBullet.impactGround = rand() % 2;
	newBullet.impactPlayer = rand() % 2;
	newBullet.ammoDepletion = rand() % (MAX_AMMODEPLETION-MIN_AMMODEPLETION) + MIN_AMMODEPLETION;
	newBullet.reloadTime = rand() % (MAX_RELOADTIME-MIN_RELOADTIME) + MIN_RELOADTIME;
	newBullet.shotCount = rand() % (MAX_SHOTCOUNT-MIN_SHOTCOUNT) + MIN_SHOTCOUNT;
	newBullet.shotDelay = rand() % MAX_SHOTDELAY + 1;
	newBullet.bounciness = rand() % (MAX_BOUNCINESS-MIN_BOUNCINESS) + MIN_BOUNCINESS;
	newBullet.acceleration = rand() % (MAX_ACCELERATION-MIN_ACCELERATION) + MIN_ACCELERATION;
	//newBullet.lifetime = rand() % (MAX_LIFETIME-MIN_LIFETIME) + MIN_LIFETIME;
	newBullet.lifetime = 200;
	newBullet.gravity = rand() % (MAX_GRAVITY-MIN_GRAVITY) + MIN_GRAVITY;
	
	
	//Calculate remaining properties TODO adjust bonuses to speed/damage as needed
	newBullet.speed = 5.0 + impactGround(1.0) + impactPlayer(1.0) + (shotDelay/10) + ((1-shotCount)/2.0);
	newBullet.damage = 10.0 + ((1.0-acceleration)*20.0) + (reloadTime/10.0) + (ammoDepletion/5.0) + ((200-lifetime)/10.0);
	newBullet.size = damage/30;
	newBullet.size = min(max(MIN_SIZE,newBullet.size),MAX_SIZE);
	return newBullet;
}

/*void Bullet::render(int camX, int camY, int vX, int vY) {
	playerTexture.render((posX - camX) + vX, (posY - camY) + vY);
}*/
