#include <SDL2/SDL.h>
#include <iostream>

using std::cout;
using std::cin;
using std::endl;
using std::string;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

enum KeyPressSurfaces {
    KEY_PRESS_SURFACE_DEFAULT,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_TOTAL
};

bool initSdlWindow(SDL_Window* &gWindow, SDL_Surface* &gScreenSurface) {
	bool success = true;
	if(SDL_Init(SDL_INIT_VIDEO) < 0 ) {
		cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
		success = false;
	} else {
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if(gWindow == NULL) {
			cout << "Window could not be created! SDL_Error: " << SDL_GetError() << endl; 
		} else {
			gScreenSurface = SDL_GetWindowSurface(gWindow);
		}
	}
	return(success);
}

void closeSdlWindow(SDL_Window* &gWindow ,SDL_Surface* &gSurface) {
	SDL_FreeSurface(gSurface);
    gSurface = NULL;

	SDL_DestroyWindow(gWindow);
    gWindow = NULL;

	SDL_Quit();
}

SDL_Surface* loadSurface(string imageName) {
	SDL_Surface* gSurface = SDL_LoadBMP(imageName.c_str());
	if(gSurface == NULL) {
		cout << "Unable to load image \"" << imageName << "\"! SDL_Error:" << SDL_GetError() << endl;
	}
	return(gSurface);
}

bool loadImagesToKeyArray(SDL_Surface* gKeyPressSurfaces[]) {
	bool success = true;
	
	gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] = loadSurface("04/default.bmp");
	if(gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT] == NULL){
		cout << "Failed to load default image!" << endl;
		success = false;
	}
	gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] = loadSurface("04/left.bmp");
	if(gKeyPressSurfaces[ KEY_PRESS_SURFACE_LEFT] == NULL){
		cout << "Failed to load left image!" << endl;
		success = false;
	}
	gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] = loadSurface("04/down.bmp");
	if(gKeyPressSurfaces[ KEY_PRESS_SURFACE_DOWN] == NULL){
		cout << "Failed to load down image!" << endl;
		success = false;
	}
	gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] = loadSurface("04/up.bmp");
	if(gKeyPressSurfaces[ KEY_PRESS_SURFACE_UP] == NULL){
		cout << "Failed to load up image!" << endl;
		success = false;
	}
	gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] = loadSurface("04/right.bmp");
	if(gKeyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT] == NULL){
		cout << "Failed to load right image!" << endl;
		success = false;
	}

	return(success);
}

int main(int argc, char *argv[]) {
	SDL_Window* gWindow = NULL;
	SDL_Surface* gScreenSurface = NULL;
	SDL_Surface* gCurrentSurface = NULL;
	SDL_Surface* gKeyPressSurfaces[KEY_PRESS_SURFACE_TOTAL];
	bool quit = false;
	SDL_Event e;
	
	loadImagesToKeyArray(gKeyPressSurfaces);
	gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];

	if(!initSdlWindow(gWindow, gScreenSurface)) {
		cout << "Failed to initialize!" << endl;
	} else {
		while(!quit) {
			while(SDL_PollEvent(&e) != 0) {
				if(e.type ==  SDL_QUIT) {
					quit = true;
				} else if (e.type == SDL_KEYDOWN) {
					 switch( e.key.keysym.sym ) {
						case SDLK_UP:
						gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_UP ];
						break;

						case SDLK_DOWN:
						gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_DOWN ];
						break;

						case SDLK_LEFT:
						gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_LEFT ];
						break;

						case SDLK_RIGHT:
						gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT ];
						break;

						default:
						gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ];
						break;
                    }
				}
			}
			SDL_BlitSurface(gCurrentSurface, NULL, gScreenSurface, NULL);
			SDL_UpdateWindowSurface(gWindow);
		}
	}
	closeSdlWindow(gWindow, gCurrentSurface);
	return(0);
}