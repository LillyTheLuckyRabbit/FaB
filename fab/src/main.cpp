/* File: main.cpp
 * Authors: David Butler and William Coar
 * Description: Entry point and main loop for the game
*/

#include <vector>

#include "common.h"
#include "constants.h"
#include "deltaclock.h"
#include "player.h"
#include "texture.h"
#include "terrain.h"

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

// Globals
SDL_Window* gameWindow = NULL;
SDL_Renderer* gameRenderer = NULL;
DeltaClock frameTimer;

bool initSdlWindow() {
	bool success = true;
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0) {
		cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
		success = false;
	} else {
		gameWindow = SDL_CreateWindow("FaB", SDL_WINDOWPOS_UNDEFINED,
		                              SDL_WINDOWPOS_UNDEFINED, RENDER_WIDTH, RENDER_HEIGHT, SDL_WINDOW_SHOWN);
		if(gameWindow == NULL) {
			cout << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
		} else {
			gameRenderer = SDL_CreateRenderer(gameWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if(gameRenderer == NULL) {
				cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << endl;
				success = false;
			}
			SDL_SetRenderDrawColor(gameRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
			int imgFlags = IMG_INIT_PNG;
			if(!(IMG_Init(imgFlags) & imgFlags)) {
				cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << endl;
				success = false;
			}
			if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
				cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << endl;
				success = false;
			}
		}
	}
	return (success);
}

void closeSdlWindow() {
	SDL_DestroyRenderer(gameRenderer);
	SDL_DestroyWindow(gameWindow);
	gameWindow = NULL;
	gameRenderer = NULL;

	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}

int main(int argc, char* argv[]) {
	int numPlayers = 0;
	#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	freopen("debug.txt", "w", stdout);
	printf("Testing!\n");
	#endif
	while(numPlayers < 2 || numPlayers > 4) {
		cout << "How many players do you have? (2-4)" << endl;
		cin >> numPlayers;
	}

	if(!initSdlWindow()) return (1);
	if(SDL_NumJoysticks() < numPlayers) {
		cout << "You don't have enough controllers connected." << endl;
		cout << "EXPECTED: " << numPlayers << " GOT: " << SDL_NumJoysticks() << endl;
		cout << "TODO: We need to add support for hotplugging controllers." << endl;
		return (1);
	}

	srand(time(NULL));
	Player* players[4];
	for(int i = 0; i < numPlayers; i++) {
		players[i] = new Player(i + 1);
		if(numPlayers > 2) players[i]->halveCameraHeight();
	}

	//Generate terrain vector
	Terrain T(LEVEL_WIDTH,LEVEL_HEIGHT);
	T.generateTerrain(players,numPlayers);
	//T.printLevel();

	vector<SDL_Rect> viewports;
	if(numPlayers == 2) {
		viewports.push_back({0, 0, RENDER_WIDTH / 2, RENDER_HEIGHT});
		viewports.push_back({RENDER_WIDTH / 2, 0, RENDER_WIDTH / 2, RENDER_HEIGHT});
	} else {
		viewports.push_back({0, 0, RENDER_WIDTH / 2, RENDER_HEIGHT / 2});
		viewports.push_back({RENDER_WIDTH / 2, 0, RENDER_WIDTH / 2, RENDER_HEIGHT / 2});
		viewports.push_back({0, RENDER_HEIGHT / 2, RENDER_WIDTH / 2, RENDER_HEIGHT / 2});
		viewports.push_back({RENDER_WIDTH / 2, RENDER_HEIGHT / 2, RENDER_WIDTH / 2, RENDER_HEIGHT / 2});
	}

	bool quit = false;
	SDL_Event e;
	SDL_Rect currentCamera;

	TextureWrapper dirtTexture;
	dirtTexture.loadFromFile("sprites/dirt.png");
	dirtTexture.setColor(96, 96, 96);

	while(!quit) {
		frameTimer.newFrame();

		// Input loop
		while(SDL_PollEvent(&e) != 0) {
			if(e.type == SDL_QUIT) {
				quit = true;
			} else if(e.type == SDL_CONTROLLERAXISMOTION) {
				if(e.caxis.axis ==SDL_CONTROLLER_AXIS_LEFTX) {
					players[e.caxis.which]->inputLeftStick(e);
				}
				if(e.caxis.axis ==SDL_CONTROLLER_AXIS_RIGHTX) {
					players[e.caxis.which]->inputRightStickX(e);
				}
				if(e.caxis.axis ==SDL_CONTROLLER_AXIS_RIGHTY) {
					players[e.caxis.which]->inputRightStickY(e);
				}
			} else if(e.type == SDL_CONTROLLERBUTTONDOWN) {
				if(e.cbutton.button == SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) {
					players[e.cbutton.which]->inputRBDown(e);
				}
			} else if(e.type == SDL_CONTROLLERBUTTONUP) {
				if(e.cbutton.button == SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) {
					players[e.cbutton.which]->inputRBUp(e);
				}
			}
		}

		// Update entities

		// Player update loop
		for(int i = 0; i < numPlayers; i++) {
			players[i]->update(frameTimer.getDelta());
		}	

		// Rendering
		SDL_SetRenderDrawColor(gameRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(gameRenderer);

		// Split-screen rendering loop
		for(int i = 0; i < numPlayers; i++) {
			SDL_RenderSetClipRect(gameRenderer, &viewports[i]);
			currentCamera = players[i]->getCamera();
			for(int y = 0; y < LEVEL_HEIGHT + dirtTexture.getWidth(); y += dirtTexture.getHeight()) {
				for(int x = 0; x < LEVEL_WIDTH + dirtTexture.getWidth(); x += dirtTexture.getWidth()) {
					if(x >= currentCamera.x - dirtTexture.getWidth()) {
						if(y >= currentCamera.y - dirtTexture.getHeight()) {
							dirtTexture.render(x - currentCamera.x + viewports[i].x, y - currentCamera.y + viewports[i].y);
						}
					}
				}
			}

			for(int e = 0; e < numPlayers; e++) {
				if(players[e]->getX() - currentCamera.x < currentCamera.w && players[e]->getX() - currentCamera.x > -(players[e]->getW())) {
					if(players[e]->getY() - currentCamera.y < currentCamera.h && players[e]->getY() - currentCamera.y > -(players[e]->getH())) {
						players[e]->render(currentCamera.x, currentCamera.y, viewports[i].x, viewports[i].y);
					}
				}
			}
		}
		SDL_RenderPresent(gameRenderer);
	}

	for(int i = 0; i < numPlayers; i++) {
		delete players[i];
	}
	closeSdlWindow();
	#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	fclose(stdout);
	#endif
	return (0);
}
