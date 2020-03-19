#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

using std::cout;
using std::cin;
using std::endl;
using std::string;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

enum KeyPressTextures {
    KEY_PRESS_TEXTURE_DEFAULT,
    KEY_PRESS_TEXTURE_UP,
    KEY_PRESS_TEXTURE_DOWN,
    KEY_PRESS_TEXTURE_LEFT,
    KEY_PRESS_TEXTURE_RIGHT,
    KEY_PRESS_TEXTURE_TOTAL
};

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
		}
	}
	return(success);
}

void closeSdlWindow(SDL_Window* &gWindow ,SDL_Renderer* &gRenderer) {
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char *argv[]) {
	SDL_Window* gWindow = NULL;
	SDL_Renderer* gRenderer = NULL;
	LTexture arrayOfTextures[KEY_PRESS_TEXTURE_TOTAL];
	LTexture* currentTexture;
	bool quit = false;
	SDL_Event e;

	if(!initSdlWindow(gWindow, gRenderer)) {
		cout << "Failed to initialize!" << endl;
	} else {
		if(!arrayOfTextures[KEY_PRESS_TEXTURE_DEFAULT].loadFromFile("04/default.png", gRenderer)) {
			cout << "Failed to load default texture." << endl;
			return(1);
		}
		if(!arrayOfTextures[KEY_PRESS_TEXTURE_UP].loadFromFile("04/up.png", gRenderer)) {
			cout << "Failed to load up texture." << endl;
			return(1);
		}
		if(!arrayOfTextures[KEY_PRESS_TEXTURE_DOWN].loadFromFile("04/down.png", gRenderer)) {
			cout << "Failed to load down texture." << endl;
			return(1);
		}
		if(!arrayOfTextures[KEY_PRESS_TEXTURE_LEFT].loadFromFile("04/left.png", gRenderer)) {
			cout << "Failed to load left texture." << endl;
			return(1);
		}
		if(!arrayOfTextures[KEY_PRESS_TEXTURE_RIGHT].loadFromFile("04/right.png", gRenderer)) {
			cout << "Failed to load right texture." << endl;
			return(1);
		}
		currentTexture = &arrayOfTextures[KEY_PRESS_TEXTURE_DEFAULT];
		while(!quit) {
			while(SDL_PollEvent(&e) != 0) {
				if(e.type ==  SDL_QUIT) {
					quit = true;
				}
				const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
				if( currentKeyStates[ SDL_SCANCODE_UP ] ) {
					currentTexture = &arrayOfTextures[KEY_PRESS_TEXTURE_UP];
				} else if( currentKeyStates[ SDL_SCANCODE_DOWN ] ) {
					currentTexture = &arrayOfTextures[KEY_PRESS_TEXTURE_DOWN];
				} else if( currentKeyStates[ SDL_SCANCODE_LEFT ] ) {
					currentTexture = &arrayOfTextures[KEY_PRESS_TEXTURE_LEFT];
				} else if( currentKeyStates[ SDL_SCANCODE_RIGHT ] ) {
					currentTexture = &arrayOfTextures[KEY_PRESS_TEXTURE_RIGHT];
				} else {
					currentTexture = &arrayOfTextures[KEY_PRESS_TEXTURE_DEFAULT];
                }
			}
			SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderClear(gRenderer);

			currentTexture->render(0, 0, gRenderer);
			
			SDL_RenderPresent(gRenderer);
		}
	}
	closeSdlWindow(gWindow, gRenderer);
	return(0);
}