#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>

using std::cout;
using std::cin;
using std::endl;
using std::string;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

class LTexture {
	public:
		LTexture();

		~LTexture();

		bool loadFromFile(string path, SDL_Renderer* &gRenderer);

		void free();

		void setColor( Uint8 red, Uint8 green, Uint8 blue );

		void setBlendMode(SDL_BlendMode blending);

		void setAlpha(Uint8 alpha);

		void render(int x, int y, SDL_Renderer* &gRenderer, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL,
					SDL_RendererFlip flip = SDL_FLIP_NONE);

		int getWidth();
		int getHeight();

    private:
		SDL_Texture* mTexture;

		int mWidth;
		int mHeight;
};

LTexture::LTexture() {
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture() {
	free();
}

bool LTexture::loadFromFile(string path, SDL_Renderer* &gRenderer) {
	free();

	SDL_Texture* newTexture = NULL;

	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if( loadedSurface == NULL ) {
		cout << "Unable to load image %s! SDL_image Error: " << IMG_GetError();
	} else {
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0xFF, 0, 0xFF));
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if( newTexture == NULL ) {
			cout << "Unable to create texture from \"" << path << "\"! SDL Error: " << SDL_GetError() << endl;
		} else {
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}
		SDL_FreeSurface( loadedSurface );
    }
	mTexture = newTexture;
	return mTexture != NULL;
}

void LTexture::free() {
	if( mTexture != NULL ) {
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
    }
}

void LTexture::render(int x, int y, SDL_Renderer* &gRenderer, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip) {
	SDL_Rect renderQuad = {x, y, mWidth, mHeight};
	if(clip != NULL) {
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}
	SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}

int LTexture::getWidth() {
	return mWidth;
}

int LTexture::getHeight() {
	return mHeight;
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue) {
	SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void LTexture::setBlendMode(SDL_BlendMode blending) {
	SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::setAlpha(Uint8 alpha) {
	SDL_SetTextureAlphaMod(mTexture, alpha);
}

bool initSdlWindow(SDL_Window* &gWindow, SDL_Renderer* &gRenderer) {
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
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if(gRenderer == NULL) {
				cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << endl;
				success = false;
			}
			SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
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
	return(success);
}

void closeSdlWindow(SDL_Window* &gWindow ,SDL_Renderer* &gRenderer) {
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

	Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char *argv[]) {
	SDL_Window* gWindow = NULL;
	SDL_Renderer* gRenderer = NULL;
	LTexture gTexture;
	Mix_Music* gMusic = NULL;
	Mix_Chunk* gSe1 = NULL;
	Mix_Chunk* gSe2 = NULL;
	Mix_Chunk* gSe3 = NULL;
	Mix_Chunk* gSe4 = NULL;
	bool quit = false;
	SDL_Event e;

	if(!initSdlWindow(gWindow, gRenderer)) {
		cout << "Failed to initialize!" << endl;
	} else {
		if(!gTexture.loadFromFile("sesplash.png", gRenderer)) {
			cout << "Failed to load splash texture." << endl;
			return(1);
		}
		gMusic = Mix_LoadMUS("music.wav");
		if(gMusic == NULL) {
			cout << "Failed to load beat music! SDL_mixer Error: " << Mix_GetError() << endl;
			return(1);
		}
		gSe1 = Mix_LoadWAV("1.wav");
		if(gSe1 == NULL) {
			cout << "Failed to load sound effect 1! SDL_mixer Error: " << Mix_GetError() << endl;
			return(1);
		}
		gSe2 = Mix_LoadWAV("2.wav");
		if(gSe2 == NULL) {
			cout << "Failed to load sound effect 2! SDL_mixer Error: " << Mix_GetError() << endl;
			return(1);
		}
		gSe3 = Mix_LoadWAV("3.wav");
		if(gSe3 == NULL) {
			cout << "Failed to load sound effect 3! SDL_mixer Error: " << Mix_GetError() << endl;
			return(1);
		}
		gSe4 = Mix_LoadWAV("4.wav");
		if(gSe4 == NULL) {
			cout << "Failed to load sound effect 4! SDL_mixer Error: " << Mix_GetError() << endl;
			return(1);
		}
		while(!quit) {
			while(SDL_PollEvent(&e) != 0) {
				if(e.type ==  SDL_QUIT) {
					quit = true;
				} else if(e.type == SDL_KEYDOWN) {
					switch(e.key.keysym.sym) {
						case SDLK_1:
						Mix_PlayChannel(-1, gSe1, 0);
						break;
						case SDLK_2:
						Mix_PlayChannel(-1, gSe2, 0);
						break;
						case SDLK_3:
						Mix_PlayChannel(-1, gSe3, 0);
						break;
						case SDLK_4:
						Mix_PlayChannel(-1, gSe4, 0);
						break;
						case SDLK_9:
						if(Mix_PlayingMusic() == 0) {
							Mix_PlayMusic(gMusic, -1);
						} else {
							if(Mix_PausedMusic() == 1) {
								Mix_ResumeMusic();
							} else {
								Mix_PauseMusic();
							}
						}
						break;
						case SDLK_0:
						Mix_HaltMusic();
						break;
					}
				}
			}
			SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderClear(gRenderer);

			gTexture.render(0, 0, gRenderer);
			
			SDL_RenderPresent(gRenderer);
		}
	}
	Mix_FreeChunk(gSe1);
	gSe1 = NULL;
	Mix_FreeChunk(gSe2);
	gSe2 = NULL;
	Mix_FreeChunk(gSe3);
	gSe3 = NULL;
	Mix_FreeChunk(gSe4);
	gSe4 = NULL;
	closeSdlWindow(gWindow, gRenderer);
	return(0);
}