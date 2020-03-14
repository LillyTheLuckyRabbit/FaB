#include <SDL2/SDL.h>
#include <iostream>

using std::cout;
using std::cin;
using std::endl;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main(int argc, char *argv[]) {
	SDL_Window* window = NULL;
	SDL_Surface* screenSurface = NULL;
	if(SDL_Init(SDL_INIT_VIDEO) < 0 ) {
		cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
	} else {
		// title, winposx, winposy, winheight, winwidth, important flag
		window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if(window == NULL) {
			cout << "Window could not be created! SDL_Error: " << SDL_GetError() << endl; 
		} else {
			screenSurface = SDL_GetWindowSurface(window);
			// FFFFFF = white
			SDL_FillRect( screenSurface, NULL,
				SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
			SDL_UpdateWindowSurface(window);
			SDL_Delay(2000); // in milliseconds
		}
	}
	SDL_DestroyWindow(window);
	SDL_Quit();
	return(0);
}