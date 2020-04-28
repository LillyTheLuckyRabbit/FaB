/* File: game.cpp
   Author: William Coar and David Butler
   Description: This file deals mainly with gameloop functions,
   such as rendering, updating terrain/players/bullets, and so on.
*/

#include "game.h"

//Forward declaration of globals
extern SDL_Window* gameWindow;
extern SDL_Renderer* gameRenderer;
extern DeltaClock frameTimer;

//Entry point for the *actual* game
void gameLoop(int numPlayers) {
	Uint32 format = SDL_GetWindowPixelFormat(gameWindow);

	//Initialize players (and halve camera height if there are 3 or 4 players).
	Player* players[4];
	for(int i = 0; i < numPlayers; i++) {
		players[i] = new Player(i + 1, 15);
		if(numPlayers > 2) players[i]->halveCameraHeight();
	}

	//Create a random level
	Terrain T(LEVEL_WIDTH,LEVEL_HEIGHT);
	T.generateTerrain(players,numPlayers);

	//This is a handy vector to keep screen coordinates for the viewports
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

	TextureWrapperStreaming terrainMask;
	terrainMask.createBlank(LEVEL_WIDTH, LEVEL_HEIGHT);
	terrainMask.setBlendMode(SDL_BLENDMODE_MOD);
	
	//Initialize the terrain mask texture. This should be the only time
	//we have to update the entire thing.
	if(!terrainMask.lockTexture()) {
		cout << "Unable to lock terrain texture!" << endl;
	} else {
		SDL_PixelFormat* mappingFormat = SDL_AllocFormat(format);
		Uint32* pixels = (Uint32*)terrainMask.getPixels();
		int pixelCount = (terrainMask.getPitch() / 4) * terrainMask.getHeight();
		Uint32 noTerrain = SDL_MapRGB(mappingFormat, 0x22, 0x22, 0x22);
		Uint32 transparent = SDL_MapRGB(mappingFormat, 0xFF, 0xFF, 0xFF);
		for(int i = 0; i < pixelCount; i++) {
			if(T.getValueAtIndex(i)) {
				pixels[i] = transparent;
			} else {
				pixels[i] = noTerrain;
			}
		}
		terrainMask.unlockTexture();
		SDL_FreeFormat(mappingFormat);
	}

	//This list is used for partial texture updates on the terrain mask
	vector<int> terrainUpdateList;
	terrainUpdateList.clear();

	//This list holds the bullet entities
	list<Bullet> bulletList;

	while(!quit) {
		frameTimer.newFrame();

		// Input loop
		while(SDL_PollEvent(&e) != 0) {
			if(e.type == SDL_QUIT) {
				quit = true;
			} else if(e.type == SDL_CONTROLLERAXISMOTION) {
				if(e.caxis.axis == SDL_CONTROLLER_AXIS_LEFTX) {
					//Movement
					players[e.caxis.which]->inputLeftStick(e);
				}
				if(e.caxis.axis == SDL_CONTROLLER_AXIS_RIGHTX) {
					//Aiming (X)
					players[e.caxis.which]->inputRightStickX(e);
				}
				if(e.caxis.axis == SDL_CONTROLLER_AXIS_RIGHTY) {
					//Aiming (Y)
					players[e.caxis.which]->inputRightStickY(e);
				}
				if(e.caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERLEFT) {
					//Instead of have a seperate action function, digging is handled
					//in the input function. That's why there's more parameters here
					//than in the other functions you see.
					players[e.caxis.which]->inputLeftTrigger(e, T, terrainUpdateList);
				}
				if(e.caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERRIGHT) {
					//Shooting
					players[e.caxis.which]->inputRightTrigger(e);
				}
			} else if(e.type == SDL_CONTROLLERBUTTONDOWN) {
				if(e.cbutton.button == SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) {
					//Jumping
					players[e.cbutton.which]->inputRBDown(e);
				}
				if(e.cbutton.button == SDL_CONTROLLER_BUTTON_LEFTSHOULDER) {
					//Dashing
					players[e.cbutton.which]->inputLBDown(e);
				}
				if(e.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_UP) {
					//Weapon Slot 1
					players[e.cbutton.which]->switchWeapon(0);
				}
				if(e.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_RIGHT) {
					//Weapon Slot 2
					players[e.cbutton.which]->switchWeapon(1);
				}
				if(e.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_DOWN) {
					//Weapon Slot 3
					players[e.cbutton.which]->switchWeapon(2);
				}
				if(e.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_LEFT) {
					//Weapon Slot 4
					players[e.cbutton.which]->switchWeapon(3);
				}
			} else if(e.type == SDL_CONTROLLERBUTTONUP) {
				if(e.cbutton.button == SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) {
					//Raising gravity when jump button is released for
					//tighter control
					players[e.cbutton.which]->inputRBUp(e);
				}
			}
		}

		// Player update loop
		for(int i = 0; i < numPlayers; i++) {
			if(players[i]->update(frameTimer.getDelta(), T, bulletList)) quit = true;
		}
		
		// Update entities (yes, two statement for loops *are* a thing)
		for(auto i = bulletList.begin(); i != bulletList.end();) {
			if(i->update(frameTimer.getDelta(), terrainUpdateList, T, players, numPlayers)) {
				i = bulletList.erase(i);
				if(bulletList.empty()) break;
			} else {
				i++;
			}
		}

		// Rendering
		SDL_SetRenderDrawColor(gameRenderer, 0x00, 0x00, 0x00, 0xFF);
		SDL_RenderClear(gameRenderer);

		// Terrain updating
		if(terrainUpdateList.size()) {
			if(!terrainMask.lockTexture()) {
				cout << "Unable to lock terrain texture!" << endl;
			} else {
				SDL_PixelFormat* mappingFormat = SDL_AllocFormat(format);
				Uint32* pixels = (Uint32*)terrainMask.getPixels();
				Uint32 noTerrain = SDL_MapRGB(mappingFormat, 0x22, 0x22, 0x22);
				Uint32 transparent = SDL_MapRGB(mappingFormat, 0xFF, 0xFF, 0xFF);
				for(size_t i = 0; i < terrainUpdateList.size(); i++) {
					if(T.getValueAtIndex(terrainUpdateList[i])) {
						pixels[terrainUpdateList[i]] = transparent;
					} else {
						pixels[terrainUpdateList[i]] = noTerrain;
					}
				}
				terrainMask.unlockTexture();
				SDL_FreeFormat(mappingFormat);
				terrainUpdateList.clear();
			}
		}

		// Split-screen rendering loop
		for(int i = 0; i < numPlayers; i++) {
			SDL_RenderSetClipRect(gameRenderer, &viewports[i]);
			currentCamera = players[i]->getCamera();
			
			//Render tiling dirt texture
			for(int y = 0; y < LEVEL_HEIGHT + dirtTexture.getWidth(); y += dirtTexture.getHeight()) {
				for(int x = 0; x < LEVEL_WIDTH + dirtTexture.getWidth(); x += dirtTexture.getWidth()) {
					if(x >= currentCamera.x - dirtTexture.getWidth()) {
						if(y >= currentCamera.y - dirtTexture.getHeight()) {
							dirtTexture.render(x - currentCamera.x + viewports[i].x, y - currentCamera.y + viewports[i].y);
						}
					}
				}
			}
			//Render mask texture
			terrainMask.render(0 + viewports[i].x, 0 + viewports[i].y, &currentCamera);

			//Render players
			for(int e = 0; e < numPlayers; e++) {
				if(players[e]->getX() - currentCamera.x < currentCamera.w && players[e]->getX() - currentCamera.x > -(players[e]->getW())) {
					if(players[e]->getY() - currentCamera.y < currentCamera.h && players[e]->getY() - currentCamera.y > -(players[e]->getH())) {
						//Render the crosshair if the player is the one the viewport belongs to
						if(e == i) {
							players[e]->render(currentCamera.x, currentCamera.y, viewports[i].x, viewports[i].y, true);
						} else {
							players[e]->render(currentCamera.x, currentCamera.y, viewports[i].x, viewports[i].y, false);
						}
					}
				}
			}

			//Render the bullets
			for(auto b = bulletList.begin(); b != bulletList.end(); b++) {
				b->render(currentCamera.x, currentCamera.y, viewports[i].x, viewports[i].y);
			}

			//Render the player HUDs
			players[i]->renderHud(currentCamera.x, currentCamera.y, viewports[i].x, viewports[i].y);
		}
		SDL_RenderSetClipRect(gameRenderer, NULL);
		SDL_RenderPresent(gameRenderer);
	}

	//Deallocate everything
	for(int i = 0; i < numPlayers; i++) {
		delete players[i];
	}
}
