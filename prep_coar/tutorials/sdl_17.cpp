#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

using std::cout;
using std::cin;
using std::endl;
using std::string;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const int BUTTON_WIDTH = 300;
const int BUTTON_HEIGHT = 200;
const int TOTAL_BUTTONS = 4;

enum LButtonSprite {
	BUTTON_SPRITE_MOUSE_OUT = 0,
	BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
	BUTTON_SPRITE_MOUSE_DOWN = 2,
	BUTTON_SPRITE_MOUSE_UP = 3,
	BUTTON_SPRITE_TOTAL = 4
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

class LButton {
	public:
		LButton();

		void setPosition( int x, int y );

		void handleEvent( SDL_Event* e );
    
		void render(LTexture &gTexture, SDL_Renderer* gRenderer, SDL_Rect* gSpriteClips);

	private:
		SDL_Point mPosition;

		LButtonSprite mCurrentSprite;
};

LButton::LButton() {
	mPosition.x = 0;
	mPosition.y = 0;

	mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
}

void LButton::setPosition(int x, int y) {
	mPosition.x = x;
	mPosition.y = y;
}

void LButton::handleEvent(SDL_Event* e) {
	if(e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP) {
		int x, y;
		SDL_GetMouseState( &x, &y );
		bool inside = true;
		if( x < mPosition.x ) {
			inside = false;
		} else if( x > mPosition.x + BUTTON_WIDTH ) {
			inside = false;
		} else if( y < mPosition.y ) {
			inside = false;
		} else if( y > mPosition.y + BUTTON_HEIGHT ) {
			inside = false;
		}
		if(!inside) {
            mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
        } else {
			switch( e->type ) {
				case SDL_MOUSEMOTION:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
				break;

				case SDL_MOUSEBUTTONDOWN:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
				break;

				case SDL_MOUSEBUTTONUP:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_UP;
				break;
            }
		}
	}
}

void LButton::render(LTexture &gTexture, SDL_Renderer* gRenderer, SDL_Rect* gSpriteClips) {
	gTexture.render(mPosition.x, mPosition.y, gRenderer, &gSpriteClips[mCurrentSprite]);
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
	LTexture gTexture;
	LButton gButtons[TOTAL_BUTTONS];
	bool quit = false;
	SDL_Event e;
	SDL_Rect gSpriteClips[BUTTON_SPRITE_TOTAL];

	if(!initSdlWindow(gWindow, gRenderer)) {
		cout << "Failed to initialize!" << endl;
	} else {
		if(!gTexture.loadFromFile("button.png", gRenderer)) {
			cout << "Failed to load sprite texture." << endl;
			return(1);
		} else {
			gSpriteClips[ 0 ].x = 0;
			gSpriteClips[ 0 ].y = 0;
			gSpriteClips[ 0 ].w = 320;
			gSpriteClips[ 0 ].h = 240;

			gSpriteClips[ 1 ].x = 0;
			gSpriteClips[ 1 ].y = 240;
			gSpriteClips[ 1 ].w = 320;
			gSpriteClips[ 1 ].h = 240;
        
 			gSpriteClips[ 2 ].x = 0;
 			gSpriteClips[ 2 ].y = 480;
 			gSpriteClips[ 2 ].w = 320;
 			gSpriteClips[ 2 ].h = 240;

			gSpriteClips[ 3 ].x = 0;
			gSpriteClips[ 3 ].y = 720;
			gSpriteClips[ 3 ].w = 320;
			gSpriteClips[ 3 ].h = 240;
		
			gButtons[0].setPosition(0, 0);
			gButtons[1].setPosition(320, 0);
			gButtons[2].setPosition(0, 240);
			gButtons[3].setPosition(320, 240);
		}
		while(!quit) {
			while(SDL_PollEvent(&e) != 0) {
				if(e.type ==  SDL_QUIT) {
					quit = true;
				}
				for(int i = 0; i < TOTAL_BUTTONS; ++i) {
						gButtons[i].handleEvent(&e);
				}
			}
			SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderClear(gRenderer);

			for(int i = 0; i < TOTAL_BUTTONS; ++i) {
				gButtons[i].render(gTexture, gRenderer, gSpriteClips);
			}
			
			SDL_RenderPresent(gRenderer);
		}
	}
	closeSdlWindow(gWindow, gRenderer);
	return(0);
}