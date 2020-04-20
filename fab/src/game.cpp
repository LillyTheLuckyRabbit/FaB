#include "game.h"

//Forward declaration of globals
extern SDL_Window* gameWindow;
extern SDL_Renderer* gameRenderer;
extern DeltaClock frameTimer;

void gameLoop(int numPlayers) {
	Uint32 format = SDL_GetWindowPixelFormat(gameWindow);

	Player* players[4];
	for(int i = 0; i < numPlayers; i++) {
		players[i] = new Player(i + 1);
		if(numPlayers > 2) players[i]->halveCameraHeight();
	}

	cout << "Testing terrain generator!\n";
	//Generate terrain vector
	//vector<int> level;
	//level.resize(LEVEL_WIDTH*LEVEL_HEIGHT,1);
	//Generate terrain vector
	Terrain T(LEVEL_WIDTH,LEVEL_HEIGHT);
	T.generateTerrain(players,numPlayers);
	T.printLevel();


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

	int scoreToWin = 15;
	Text textRenderer("sprites/font.png", 17, 16);
	stringstream hudText;

	TextureWrapperStreaming terrainTexture;
	bool updateTerrain = true;
	terrainTexture.createBlank(LEVEL_WIDTH, LEVEL_HEIGHT);
	terrainTexture.setBlendMode(SDL_BLENDMODE_BLEND);

	while(!quit) {
		frameTimer.newFrame();

		// Input loop
		while(SDL_PollEvent(&e) != 0) {
			if(e.type == SDL_QUIT) {
				quit = true;
			} else if(e.type == SDL_CONTROLLERAXISMOTION) {
				if(e.caxis.axis == SDL_CONTROLLER_AXIS_LEFTX) {
					players[e.caxis.which]->inputLeftStick(e);
				}
				if(e.caxis.axis == SDL_CONTROLLER_AXIS_RIGHTX) {
					players[e.caxis.which]->inputRightStickX(e);
				}
				if(e.caxis.axis == SDL_CONTROLLER_AXIS_RIGHTY) {
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
		SDL_SetRenderDrawColor(gameRenderer, 0x00, 0x00, 0x00, 0xFF);
		SDL_RenderClear(gameRenderer);

		// Terrain updating
		if(updateTerrain) {
			if(!terrainTexture.lockTexture()) {
				cout << "Unable to lock terrain texture!" << endl;
			} else {
				SDL_PixelFormat* mappingFormat = SDL_AllocFormat(format);
				Uint32* pixels = (Uint32*)terrainTexture.getPixels();
				int pixelCount = (terrainTexture.getPitch() / 4) * terrainTexture.getHeight();
				Uint32 noTerrain = SDL_MapRGBA(mappingFormat, 0x00, 0x00, 0x00, 0x40);
				Uint32 transparent = SDL_MapRGBA(mappingFormat, 0xFF, 0xFF, 0xFF, 0x00);
				for(int i = 0; i < pixelCount; i++) {
					if(level[i]) {
						pixels[i] = transparent;
					} else {
						pixels[i] = noTerrain;
					}
				}
				terrainTexture.unlockTexture();
				updateTerrain = false;
				SDL_FreeFormat(mappingFormat);
				terrainTexture.setBlendMode(SDL_BLENDMODE_BLEND);
			}
		}

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
			terrainTexture.render(0 + viewports[i].x, 0 + viewports[i].y, &currentCamera);

			for(int e = 0; e < numPlayers; e++) {
				if(players[e]->getX() - currentCamera.x < currentCamera.w && players[e]->getX() - currentCamera.x > -(players[e]->getW())) {
					if(players[e]->getY() - currentCamera.y < currentCamera.h && players[e]->getY() - currentCamera.y > -(players[e]->getH())) {
						if(e == i) {
							players[e]->render(currentCamera.x, currentCamera.y, viewports[i].x, viewports[i].y, true);
						} else {
							players[e]->render(currentCamera.x, currentCamera.y, viewports[i].x, viewports[i].y, false);
						}
					}
				}
			}

			hudText.str("");
			hudText << "Health: " << players[i]->getHealth();
			textRenderer.render(textRenderer.getFontW() + viewports[i].x, viewports[i].h - textRenderer.getFontH() - 4 + viewports[i].y, hudText.str());
			hudText.str("");
			hudText << "Score: " << players[i]->getScore() << "/" << scoreToWin;
			textRenderer.render(viewports[i].w - ((hudText.str().length() + 1) * textRenderer.getFontW()) + viewports[i].x, viewports[i].h - textRenderer.getFontH() - 4 + viewports[i].y, hudText.str());
		}
		SDL_RenderSetClipRect(gameRenderer, NULL);
		SDL_RenderPresent(gameRenderer);
	}

	for(int i = 0; i < numPlayers; i++) {
		delete players[i];
	}
}