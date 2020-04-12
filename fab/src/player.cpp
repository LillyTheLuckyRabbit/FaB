/* File: player.cpp
 * Authors: David Butler and William Coar
 * Description: Everything relating to the player lives in this class. This
 * includes the player's health, score, weapons, pointer to the controller, etc.
*/

#include "player.h"

Player::Player(int num) {
	playerNumber = num;
	controllerPtr = SDL_JoystickOpen(playerNumber - 1);

	width = 29;
	height = 29;
	posX = rand() % (LEVEL_WIDTH - width);
	posY = rand() % (LEVEL_HEIGHT - height);

	camera.h = RENDER_HEIGHT;
	camera.w = RENDER_WIDTH / 2;
	camera.x = posX - RENDER_WIDTH / 4;
	if(camera.x < 0) {
		camera.x = 0;
	}
	camera.y = posY + RENDER_HEIGHT / 4;
	if(camera.y > LEVEL_HEIGHT - camera.h) {
		camera.y = LEVEL_HEIGHT - camera.h;
	}

	if(!playerTexture.loadFromFile("sprites/testpill.png")) {
		cout << "Failed to load the player sprite!" << endl;
	}
}

Player::~Player() {
	SDL_JoystickClose(controllerPtr);
	controllerPtr = NULL;
}

void Player::handleEvent(const SDL_Event& e) {
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
}

void Player::render(int camX, int camY, int vX, int vY) {
	playerTexture.render((posX - camX) + vX, (posY - camY) + vY);
}

void Player::halveCameraHeight() {
	camera.h = RENDER_HEIGHT / 2;
	camera.y = posY + RENDER_HEIGHT / 8;
	if(camera.y > LEVEL_HEIGHT - camera.h) {
		camera.y = LEVEL_HEIGHT - camera.h;
	}
}