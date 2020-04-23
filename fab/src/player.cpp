/* File: player.cpp
 * Authors: David Butler and William Coar
 * Description: Everything relating to the player lives in this class. This
 * includes the player's health, score, weapons, pointer to the controller, etc.
*/

#include "player.h"

#include "angle.h"

Player::Player(int num) {
	playerNumber = num;
	controllerPtr = SDL_GameControllerOpen(playerNumber - 1);

	width = 29;
	height = 29;
	posX = rand() % (LEVEL_WIDTH - width);
	posY = rand() % (LEVEL_HEIGHT - height);
	velX = 0;
	velY = 0;
	accelX = 0;
	accelDir = 0;

	camera.h = RENDER_HEIGHT;
	camera.w = RENDER_WIDTH / 2;
	camera.x = posX - RENDER_WIDTH / 4;
	if(camera.x < 0) {
		camera.x = 0;
	}
	camera.y = posY - RENDER_HEIGHT / 4;
	if(camera.y < 0) {
		camera.y = 0;
	}

	if(camera.y > LEVEL_HEIGHT - camera.h) {
		camera.y = LEVEL_HEIGHT - camera.h;
	}

	gravity = PLAYER_HIGHGRAV;

	if(!playerTexture.loadFromFile("sprites/player_squid.png")) {
		cout << "Failed to load the player sprite!" << endl;
	}
	if(!eyeTexture.loadFromFile("sprites/player_squid_eye.png")) {
		cout << "Failed to load the eye sprite!" << endl;
	}
	if(!crossHair.loadFromFile("sprites/crosshair.png")) {
		cout << "Failed to load the crosshair sprite!" << endl;
	}
	if(!circle.loadFromFile("sprites/circle.png")) {
		cout << "Failed to load the circle sprite!" << endl;
	}

	if(playerNumber == 1) {
		playerTexture.setColor(64, 200, 64);
		crossHair.setColor(64, 200, 64);
		circle.setColor(191, 55, 191);
	} else if(playerNumber == 2) {
		playerTexture.setColor(200, 64, 64);
		crossHair.setColor(200, 64, 64);
		circle.setColor(55, 191, 191);
	} else if(playerNumber == 3) {
		playerTexture.setColor(64, 64, 200);
		crossHair.setColor(64, 64, 200);
		circle.setColor(191, 191, 55);
	} else if(playerNumber == 4) {
		playerTexture.setColor(200, 64, 200);
		crossHair.setColor(200, 64, 200);
		circle.setColor(55, 191, 55);
	}

	circle.setBlendMode(SDL_BLENDMODE_ADD);
	crossHair.setAlpha(128);

	health = 100;
	score = 0;

	grounded = false;
	dashAvail = true;
	dig = true;
}

Player::Player() {
	controllerPtr = NULL;
}

Player::~Player() {
	if(controllerPtr) {
		SDL_GameControllerClose(controllerPtr);
		controllerPtr = NULL;
	}
}

void Player::inputLeftStick(const SDL_Event& e) {
	if(e.caxis.value > 2000) {
		accelDir = 1;
	} else if(e.caxis.value < -2000) {
		accelDir = -1;
	} else {
		accelDir = 0;
	}
}

void Player::inputRightStickX(const SDL_Event& e) {
	if(e.caxis.value > 2500 || e.caxis.value < -2500) {
		angleX = e.caxis.value;
	}
}

void Player::inputRightStickY(const SDL_Event& e) {
	if(e.caxis.value > 2500 || e.caxis.value < -2500) {
		angleY = e.caxis.value;
	}
}

bool Player::inputLeftTrigger(const SDL_Event& e, Terrain& T, vector<int>& terrainUpdateList) {
	bool containsTerrain = false;
	if(e.caxis.value > 2500) {
		if(dig) {
			int centerX = (width / 2 + getXComp(angle, 20)) + posX;
			int centerY = (height / 2 + getYComp(angle, 20)) + posY;
			int radius = 20;
			for(int y = -radius; y <= radius; y++) {
				for(int x = -radius; x <= radius; x++) {
					if(x * x + y * y < radius * radius) {
						if(T.getValueAtXY(centerX + x, centerY + y) == 1) {
							containsTerrain = true;
							break;
						}
					}
				}
				if(containsTerrain) break;
			}
			if(containsTerrain) {
				for(int y = -radius; y <= radius; y++) {
					for(int x = -radius; x <= radius; x++) {
						if(x * x + y * y < radius * radius) {
							if(T.getValueAtXY(centerX + x, centerY + y) == 1) {
								T.setValueAtXY(centerX + x, centerY + y, 0);
								terrainUpdateList.push_back((centerY + y) * LEVEL_WIDTH + (centerX + x));
							}
						}
					}
				}
				dig = false;
			}
		}
	} else if(e.caxis.value == 0) {
		dig = true;
	}
	return (containsTerrain);
}

void Player::inputRBDown(const SDL_Event& e) {
	if(e.cbutton.state == SDL_PRESSED) {
		if(grounded) {
			velY = -500;
			posY -= 1;
			gravity = PLAYER_LOWGRAV;
			grounded = false;
		}
	}
}

void Player::inputRBUp(const SDL_Event& e) {
	if(e.cbutton.state == SDL_RELEASED) {
		gravity = PLAYER_HIGHGRAV;
	}
}

void Player::inputLBDown(const SDL_Event& e) {
	if(e.cbutton.state == SDL_PRESSED) {
		if(dashAvail) {
			velY = getYComp(angle, 1000);
			velX = getXComp(angle, 1000);
			gravity = PLAYER_HIGHGRAV;
			grounded = false;
			dashAvail = false;
			dashTime = 1000;
		}
	}
}

void Player::update(int deltaTime, const Terrain& T) {
	if(posX - camera.x < camera.w / 6 && camera.x > 0) {
		camera.x -= 2;
		if(posX - width < camera.x) {
			camera.x -= 5;
		}
	}
	if(posX - camera.x > 5 * (camera.w / 6) && camera.x < LEVEL_WIDTH - camera.w) {
		camera.x += 2;
		if(posX > camera.x + camera.w) {
			camera.x += 5;
		}
	}
	if(posY - camera.y < camera.h / 3 && camera.y > 0) {
		camera.y -= 2;
		if(posY - height < camera.y) {
			camera.y -= 5;
		}
	}
	if(posY - camera.y > 2 * (camera.h / 3) && camera.y < LEVEL_HEIGHT - camera.h) {
		camera.y += 2;
		if(posY > camera.y + camera.h) {
			camera.y += 5;
		}
	}
	if(!dashAvail && dashTime > 0) {
		dashTime -= deltaTime;
	}

	angle = getDegrees(angleX, angleY);

	if(accelDir == 0) {
		if(velX > 0 && grounded) {
			accelX = -2000;
		} else if(velX < 0 && grounded) {
			accelX = 2000;
		} else {
			accelX = 0;
		}
	} else if(accelDir == 1) {
		if(velX < 400) {
			accelX = 2000;
		} else if(grounded){
			accelX = -2000;
		} else {
			accelX = 0;
		}
	} else if(accelDir == -1) {
		if(velX > -400) {
			accelX = -2000;
		} else if(grounded){
			accelX = 2000;
		} else {
			accelX = 0;
		}
	}

	int deltaX = 0;
	velX = trunc(velX + accelX * deltaTime / 1000.0);
	if(velX) {
		deltaX = trunc((velX * deltaTime) / 1000.0);
		posX += deltaX;
	}

	int deltaY = trunc(((velY * deltaTime) + .5 * gravity * (deltaTime * deltaTime) / 1000.0) / 1000.0);
	posY += deltaY;
	if(velY >= 0) gravity = PLAYER_HIGHGRAV;
	velY = velY + (gravity * deltaTime / 1000.0);

	if(checkCollision(T)) {
		if(deltaY > 0) grounded = true;
		if(dashTime <= 0 && !dashAvail) {
			dashAvail = true;
		}
		if(checkCollision(T) < height / 2 && posY < LEVEL_HEIGHT - height) {
			posY -= checkCollision(T);
			velY = 0;
		} else if(posY > LEVEL_HEIGHT - height) {
			posY -= deltaY;
			velY = 0;
		} else {
			posY -= deltaY;
			if(checkCollision(T)) {
				posX -= deltaX;
				velX = -0.1 * velX;
			}
		}
	}
}

int Player::checkCollision(const Terrain& T) {
	if(posX < 0 || posX > LEVEL_WIDTH - width || posY < 0 || posY > LEVEL_HEIGHT - height) {
		return (height);
	}
	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			if(T.getValueAtXY(posX + x, posY + y)) {
				return (height - y);
			}
		}
	}
	return (0);
}

void Player::render(int camX, int camY, int vX, int vY, bool cross) {
	SDL_RendererFlip flip;
	if((angle >= -90 && angle <= 0) || (angle <= 90 && angle >= 0)) {
		flip = SDL_FLIP_HORIZONTAL;
	} else {
		flip = SDL_FLIP_NONE;
	}
	playerTexture.render((posX - camX) + vX, (posY - camY) + vY, NULL, 0, NULL, flip);
	int eyeX = 10;
	if(flip == SDL_FLIP_HORIZONTAL) eyeX = 11;
	SDL_Point eyeCenter;
	eyeCenter.x = 4;
	eyeCenter.y = 4;
	eyeTexture.render(posX - camX + eyeX + vX, posY - camY + 10 + vY, NULL, angle, &eyeCenter, SDL_FLIP_HORIZONTAL);
	if(cross) {
		SDL_Point playerCenter;
		playerCenter.x = -100;
		playerCenter.y = 0;
		crossHair.render(posX - camX + vX + 100, posY - camY + vY + height / 2, NULL, angle, &playerCenter);
	}
	if(dashTime > 0) circle.render(posX - camX + vX - 5, posY - camY + vY - 5);
}

void Player::halveCameraHeight() {
	camera.h = RENDER_HEIGHT / 2;
	camera.y = posY + RENDER_HEIGHT / 8;
	if(camera.y > LEVEL_HEIGHT - camera.h) {
		camera.y = LEVEL_HEIGHT - camera.h;
	}
}