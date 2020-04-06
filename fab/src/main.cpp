#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	#include <SDL.h>
	#include <SDL_image.h>
	#include <SDL_mixer.h>
#else
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_image.h>
	#include <SDL2/SDL_mixer.h>
#endif

#include <iostream>

using namespace std;

int main() {
	cout << "makefile test" << endl;
	return 1;
}
