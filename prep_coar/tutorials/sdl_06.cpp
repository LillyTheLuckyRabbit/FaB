#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

using std::cout;
using std::cin;
using std::endl;
using std::string;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

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
			int imgFlags = IMG_INIT_PNG;
			if(!(IMG_Init(imgFlags) & imgFlags)) {
				cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << endl;
				success = false;
			} else {
				gScreenSurface = SDL_GetWindowSurface(gWindow);
			}
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

SDL_Surface* loadSurface(string imageName, SDL_PixelFormat* format) {
	SDL_Surface* optimizedSurface = NULL;
	SDL_Surface* loadedSurface = IMG_Load(imageName.c_str());
	if(loadedSurface == NULL) {
		cout << "Unable to load image \"" << imageName << "\"! SDL Error:" << SDL_GetError() << endl;
	} else {
		optimizedSurface = SDL_ConvertSurface(loadedSurface, format, 0);
		if(optimizedSurface == NULL) {
			cout << "Unable to optimize image \"" << imageName << "\"! SDL Error: " << SDL_GetError() << endl;
		}
		SDL_FreeSurface(loadedSurface);
	}
	return(optimizedSurface);
}

int main(int argc, char *argv[]) {
	SDL_Window* gWindow = NULL;
	SDL_Surface* gScreenSurface = NULL;
	SDL_Surface* gCurrentSurface = NULL;
	bool quit = false;
	SDL_Event e;
	
	SDL_Rect stretchRect;
	stretchRect.x = 0;
	stretchRect.y = 0;
	stretchRect.w = SCREEN_WIDTH;
	stretchRect.h = SCREEN_HEIGHT;

	if(!initSdlWindow(gWindow, gScreenSurface)) {
		cout << "Failed to initialize!" << endl;
	} else {
		gCurrentSurface = loadSurface("plok.png", gScreenSurface->format);
		while(!quit) {
			while(SDL_PollEvent(&e) != 0) {
				if(e.type ==  SDL_QUIT) {
					quit = true;
				}
			}
			SDL_BlitScaled(gCurrentSurface, NULL, gScreenSurface, &stretchRect);
			SDL_UpdateWindowSurface(gWindow);
		}
	}
	closeSdlWindow(gWindow, gCurrentSurface);
	return(0);
}