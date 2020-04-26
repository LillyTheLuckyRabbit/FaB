/* File: player.cpp
 * Authors: David Butler and William Coar
 * Description: Everything relating to the player lives in this class. This
 * includes the player's health, score, weapons, pointer to the controller, etc.
*/

#include "player.h"

#include "angle.h"

Player::Player(int num, int scoreToWin) {
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
	if(camera.x > LEVEL_WIDTH - camera.w) {
		camera.x = LEVEL_WIDTH - camera.w;
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
	if(!gun.loadFromFile("sprites/gunLarge.png")) {
		cout << "Failed to load the gun sprite!" << endl;
	}

	if(playerNumber == 1) {
		playerTexture.setColor(64, 200, 64);
		crossHair.setColor(64, 200, 64);
		circle.setColor(64, 200, 64);
	} else if(playerNumber == 2) {
		playerTexture.setColor(200, 64, 64);
		crossHair.setColor(200, 64, 64);
		circle.setColor(200, 64, 64);
	} else if(playerNumber == 3) {
		playerTexture.setColor(64, 64, 200);
		crossHair.setColor(64, 64, 200);
		circle.setColor(64, 64, 200);
	} else if(playerNumber == 4) {
		playerTexture.setColor(200, 64, 200);
		crossHair.setColor(200, 64, 200);
		circle.setColor(200, 64, 200);
	}

	circle.setBlendMode(SDL_BLENDMODE_ADD);
	crossHair.setAlpha(128);

	textRenderer.loadFont("sprites/font.png", 17, 16);
	calamari.loadFromFile("sprites/calamari.png");

	health = 100;
	score = 0;
	goal = scoreToWin;
	respawnTime = 10000;

	grounded = false;
	dashAvail = true;
	dashTime = 0;
	dig = true;
	shoot = true;
	alive = true;

	for(int i = 0; i < 4; i++) {
		weaponInv.emplace_back("Peashooter", 4, 500, 50, 2000, "sprites/bullet.png", 20);
	}
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
	if(alive) {
		if(e.caxis.value > 2000) {
			accelDir = 1;
		} else if(e.caxis.value < -2000) {
			accelDir = -1;
		} else {
			accelDir = 0;
		}
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
	if(alive) {
		bool containsTerrain = false;
		if(e.caxis.value > 2500) {
			if(dig) {
				int radius = 20;
				int centerX = (width / 2 + getXComp(angle, radius)) + posX;
				int centerY = (height / 2 + getYComp(angle, radius)) + posY;
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
								if(T.setValueAtXY(centerX + x, centerY + y, 0)) {
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
	return (false);
}

void Player::inputRightTrigger(const SDL_Event& e, vector<Bullet>& bulletVec) {
	if(alive) {
		if(e.caxis.value > 2500) {
			if(shoot) {
				weaponInv[currentWeapon].shoot(bulletVec, playerNumber, angle, posX + width / 2, posY + height / 2);
				shoot = false;
			}
		} else if(e.caxis.value == 0) {
			shoot = true;
		}
	}
}

void Player::inputRBDown(const SDL_Event& e) {
	if(alive) {
		if(e.cbutton.state == SDL_PRESSED) {
			if(grounded) {
				velY = -500;
				posY -= 1;
				gravity = PLAYER_LOWGRAV;
				grounded = false;
			}
		}
	}
}

void Player::inputRBUp(const SDL_Event& e) {
	if(e.cbutton.state == SDL_RELEASED) {
		gravity = PLAYER_HIGHGRAV;
	}
}

void Player::inputLBDown(const SDL_Event& e) {
	if(alive) {
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
}

bool Player::update(int deltaTime, const Terrain& T, vector<Bullet>& bulletVec) {
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
		} else if(grounded) {
			accelX = -2000;
		} else {
			accelX = 0;
		}
	} else if(accelDir == -1) {
		if(velX > -400) {
			accelX = -2000;
		} else if(grounded) {
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
			if(deltaY < 0) velY = velY * -0.1;
			if(checkCollision(T)) {
				posX -= deltaX;
				velX = -0.1 * velX;
			}
		}
	}

	weaponInv[currentWeapon].update(deltaTime);

	if(health <= 0) {
		health = 0;
		alive = false;
	}

	if(!alive) {
		respawnTime -= deltaTime;
	}

	if(respawnTime < 0) {
		alive = true;
		health = 100;
		respawnTime = 10000;
		posX = rand() % (LEVEL_WIDTH - width);
		posY = rand() % (LEVEL_HEIGHT - height);
	}

	if(dashTime > 0) {
		int centerX = posX + width / 2;
		int centerY = posY + height / 2;
		int radius = circle.getWidth() / 2;
		for(auto i = bulletVec.begin(); i != bulletVec.end(); i++) {
			if(i->getX() + i->getH() >= centerX - radius && i->getX() <= centerX + radius) {
				if(i->getY() + i->getH() >= centerY - radius && i->getY() <= centerY + radius) {	
					i->reverseVel(playerNumber);
				}
			}
		}
	}

	return (score == goal);
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
	if(alive) {
		SDL_RendererFlip gunFlip;
		if((angle >= -90 && angle <= 0) || (angle <= 90 && angle >= 0)) {
			gunFlip = SDL_FLIP_HORIZONTAL;
		} else {
			gunFlip = (SDL_RendererFlip)(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
		}
		int gunX = 2;
		int gunY = 18;
		SDL_Point gunPivot;
		gunPivot.x = 2;
		gunPivot.y = 4;
		if(flip == SDL_FLIP_HORIZONTAL) {
			gunX = 24;
			gunY = 9;
			gunPivot.x = 2;
			gunPivot.y = 11;
		}
		gun.render((posX - camX) + vX + gunX, (posY - camY) + vY + gunY, NULL, angle, &gunPivot, gunFlip);
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
		if(dashTime > 0) circle.render(posX - camX + vX - (circle.getWidth() / 2 - width / 2), posY - camY + vY - (circle.getHeight() / 2 - height / 2));
	} else {
		calamari.render((posX - camX) + vX, (posY - camY) + vY, NULL, 0, NULL, flip);
	}
}

void Player::renderHud(int camX, int camY, int vX, int vY) {
	stringstream hudText;

	hudText.str("");
	hudText << "Health: " << health;
	textRenderer.render(textRenderer.getFontW() + vX, camera.h - textRenderer.getFontH() - 4 + vY, hudText.str());

	if(weaponInv[currentWeapon].isReloading()) {
		hudText.str("");
		hudText << "Reloading...";
		textRenderer.render(textRenderer.getFontW() + vX, camera.h - 2 * textRenderer.getFontH() - 4 + vY, hudText.str());
	} else {
		hudText.str("");
		hudText << "Ammo: " << weaponInv[currentWeapon].getAmmo() << "/" << weaponInv[currentWeapon].getTotal();
		textRenderer.render(textRenderer.getFontW() + vX, camera.h - 2 * textRenderer.getFontH() - 4 + vY, hudText.str());
	}

	hudText.str("");
	hudText << weaponInv[currentWeapon].getName();
	textRenderer.render(textRenderer.getFontW() + vX, camera.h - 3 * textRenderer.getFontH() - 4 + vY, hudText.str());

	hudText.str("");
	hudText << "Score: " << score << "/" << goal;
	textRenderer.render(camera.w - ((hudText.str().length() + 1) * textRenderer.getFontW()) + vX, camera.h - textRenderer.getFontH() - 4 + vY, hudText.str());

	if(!alive) {
		hudText.str("");
		hudText << "Repawning in " << respawnTime / 1000 << "...";
		textRenderer.render(camera.w / 2 - (hudText.str().length() * textRenderer.getFontW() / 2) + vX, camera.h / 2 - textRenderer.getFontH() + vY, hudText.str());
	}
}

void Player::halveCameraHeight() {
	camera.h = RENDER_HEIGHT / 2;
	camera.y = posY + RENDER_HEIGHT / 8;
	if(camera.y > LEVEL_HEIGHT - camera.h) {
		camera.y = LEVEL_HEIGHT - camera.h;
	}
}