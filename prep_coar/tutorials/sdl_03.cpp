#include <SDL2/SDL.h>
#include <iostream>

using std::cout;
using std::cin;
using std::endl;

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

bool loadMediaOntoSurface(SDL_Surface* &gSurface) {
	bool success = true;

	gSurface = SDL_LoadBMP("xtoexit.bmp");
	if(gSurface == NULL) {
		cout << "Unable to load image " << "'xtoexit.bmp'" << "! SDL_Error:" << SDL_GetError() << endl;
		success = false;	
	}
	return(success);
}

int main(int argc, char *argv[]) {
	SDL_Window* gWindow = NULL;
	SDL_Surface* gScreenSurface = NULL;
	SDL_Surface* gImage = NULL;
	bool quit = false;
	SDL_Event e;
	if(!initSdlWindow(gWindow, gScreenSurface)) {
		cout << "Failed to initialize!" << endl;
	} else {
		if(loadMediaOntoSurface(gImage)) {
			while(!quit) {
				while(SDL_PollEvent(&e) != 0) {
					if(e.type ==  SDL_QUIT) {
						quit = true;
					}
				}
				SDL_BlitSurface( gImage, NULL, gScreenSurface, NULL );
				SDL_UpdateWindowSurface(gWindow);
			}
		} else {
			cout << "Failed to load media!" << endl;
		}
	}
	closeSdlWindow(gWindow, gImage);
	return(0);
}