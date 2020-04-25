/* File: main.cpp
 * Authors: David Butler and William Coar
 * Description: Entry point and main loop for the game
*/

#include "common.h"
#include "constants.h"
#include "deltaclock.h"
#include "texture.h"
#include "text.h"
#include "game.h"

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
	//Reroute output to debug.txt for... uh... debugging.
	#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	freopen("debug.txt", "w", stdout);
	#endif
	
	srand(time(NULL));
	if(!initSdlWindow()) return (1);
	if(SDL_NumJoysticks() < 2) {
		cout << "You don't have enough controllers connected." << endl;
		cout << "TODO: We need to add support for hotplugging controllers." << endl;
		closeSdlWindow();
		return (1);
	}

	bool quit = false;
	bool startGame = false;
	int numPlayers = 2;
	stringstream menuText;
	Text textRenderer("sprites/bigfont.png", 34, 32);

	SDL_Event e;
	SDL_GameController* menuPlayer;
	menuPlayer = SDL_GameControllerOpen(0);
	while(!quit) {
		while(SDL_PollEvent(&e) != 0) {
			if(e.type == SDL_QUIT) {
				quit = true;
			} else if(e.type == SDL_CONTROLLERBUTTONDOWN) {
				if(e.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_RIGHT) {
					if(numPlayers < SDL_NumJoysticks()) {
						numPlayers++;
					}
				} else if(e.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_LEFT) {
					if(numPlayers > 2) {
						numPlayers--;
					}
				} else if(e.cbutton.button == SDL_CONTROLLER_BUTTON_A) {
					startGame = true;
				} else if(e.cbutton.button == SDL_CONTROLLER_BUTTON_BACK) {
					quit = true;
				}
			}
		}
		
		SDL_SetRenderDrawColor(gameRenderer, 0x88, 0x88, 0x88, 0xFF);
		SDL_RenderClear(gameRenderer);
		
		menuText.str("");
		menuText << "FaB (SUPER DUPER EARLY PROTO)";
		textRenderer.render(RENDER_WIDTH / 2 - (menuText.str().length() * textRenderer.getFontW() / 2),
								textRenderer.getFontH(), menuText.str());
		menuText.str("");
		menuText << "Use the DPAD to select the number of players,";
		textRenderer.render(RENDER_WIDTH / 2 - (menuText.str().length() * textRenderer.getFontW() / 2),
								textRenderer.getFontH() * 3, menuText.str());
		menuText.str("");
		menuText << "A to start the game, or BACK/SELECT to exit.";
		textRenderer.render(RENDER_WIDTH / 2 - (menuText.str().length() * textRenderer.getFontW() / 2),
								textRenderer.getFontH() * 4, menuText.str());
		menuText.str("");
		menuText << "2";
		if(numPlayers == 2) {
			textRenderer.render(RENDER_WIDTH / 6, textRenderer.getFontH() * 16, menuText.str());
		} else {
			textRenderer.render(RENDER_WIDTH / 6, textRenderer.getFontH() * 16, menuText.str(), 96, 96, 96);

		}
		menuText.str("");
		menuText << "3";
		if(numPlayers == 3) {
			textRenderer.render(RENDER_WIDTH / 2 - textRenderer.getFontW() / 2, textRenderer.getFontH() * 16, menuText.str());
		} else if(SDL_NumJoysticks() < 3) {
			textRenderer.render(RENDER_WIDTH / 2 - textRenderer.getFontW() / 2, textRenderer.getFontH() * 16, menuText.str(), 255, 0, 0);
		} else {
			textRenderer.render(RENDER_WIDTH / 2 - textRenderer.getFontW() / 2, textRenderer.getFontH() * 16, menuText.str(), 96, 96, 96);
		}
		menuText.str("");
		menuText << "4";
		if(numPlayers == 4) {
			textRenderer.render(5 * RENDER_WIDTH / 6, textRenderer.getFontH() * 16, menuText.str());
		} else if(SDL_NumJoysticks() < 4) {
			textRenderer.render(5 * RENDER_WIDTH / 6, textRenderer.getFontH() * 16, menuText.str(), 255, 0, 0);
		} else {
			textRenderer.render(5 * RENDER_WIDTH / 6, textRenderer.getFontH() * 16, menuText.str(), 96, 96, 96);
		}

		SDL_RenderPresent(gameRenderer);
		
		if(startGame) {
			SDL_GameControllerClose(menuPlayer);
			menuPlayer = NULL;
			gameLoop(numPlayers);
			quit = true;
		}
	}

	closeSdlWindow();
	#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	fclose(stdout);
	#endif
	return (0);
}