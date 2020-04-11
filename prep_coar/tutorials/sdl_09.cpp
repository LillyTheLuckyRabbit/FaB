#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

using std::cout;
using std::cin;
using std::endl;
using std::string;

const int RENDER_WIDTH = 640;
const int RENDER_HEIGHT = 480;

bool initSdlWindow(SDL_Window* &gWindow, SDL_Renderer* &gRenderer) {
	bool success = true;
	if(SDL_Init(SDL_INIT_VIDEO) < 0 ) {
		cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
		success = false;
	} else {
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED, RENDER_WIDTH, RENDER_HEIGHT, SDL_WINDOW_SHOWN);
		if(gWindow == NULL) {
			cout << "Window could not be created! SDL_Error: " << SDL_GetError() << endl; 
		} else {
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
			int imgFlags = IMG_INIT_PNG;
			if(!(IMG_Init(imgFlags) & imgFlags)) {
				cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << endl;
				success = false;
			}
		}
	}
	return(success);
}

void closeSdlWindow(SDL_Window* &gWindow ,SDL_Renderer* &gRenderer, SDL_Texture* &gTexture) {
	SDL_DestroyTexture( gTexture );
    gTexture = NULL;

    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    gRenderer = NULL;

    IMG_Quit();
    SDL_Quit();
}

SDL_Texture* loadTexture(string imageName, SDL_Renderer* &gRenderer) {
	SDL_Texture* newTexture = NULL;
	SDL_Surface* loadedSurface = IMG_Load(imageName.c_str());
	if(loadedSurface == NULL) {
		cout << "Unable to load image \"" << imageName << "\"! SDL Error:" << SDL_GetError() << endl;
	} else {
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if(newTexture == NULL) {
			cout << "Unable to create texture from \"" << imageName << "\"! SDL Error: " << SDL_GetError() << endl;
		}
		SDL_FreeSurface(loadedSurface);
	}
	return(newTexture);
}

int main(int argc, char *argv[]) {
	SDL_Window* gWindow = NULL;
	SDL_Renderer* gRenderer = NULL;
	SDL_Texture* gTexture = NULL;
	bool quit = false;
	SDL_Event e;
	
	SDL_Rect topLeftViewport;
    topLeftViewport.x = 0;
    topLeftViewport.y = 0;
    topLeftViewport.w = RENDER_WIDTH / 2;
    topLeftViewport.h = RENDER_HEIGHT / 2;

	SDL_Rect topRightViewport;
    topRightViewport.x = RENDER_WIDTH / 2;
    topRightViewport.y = 0;
    topRightViewport.w = RENDER_WIDTH / 2;
    topRightViewport.h = RENDER_HEIGHT / 2;

	SDL_Rect bottomViewport;
    bottomViewport.x = 0;
    bottomViewport.y = RENDER_HEIGHT / 2;
    bottomViewport.w = RENDER_WIDTH;
    bottomViewport.h = RENDER_HEIGHT / 2;
    SDL_RenderSetViewport( gRenderer, &bottomViewport );


	if(!initSdlWindow(gWindow, gRenderer)) {
		cout << "Failed to initialize!" << endl;
	} else {
		gTexture = loadTexture("plok.png", gRenderer);
		while(!quit) {
			while(SDL_PollEvent(&e) != 0) {
				if(e.type ==  SDL_QUIT) {
					quit = true;
				}
			}
			SDL_RenderClear(gRenderer);

			SDL_RenderSetViewport(gRenderer, &topLeftViewport);
			SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);

			SDL_RenderSetViewport(gRenderer, &topRightViewport);
			SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);

			SDL_RenderSetViewport(gRenderer, &bottomViewport);
			SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);

			SDL_RenderPresent(gRenderer);
		}
	}
	closeSdlWindow(gWindow, gRenderer, gTexture);
	return(0);
}