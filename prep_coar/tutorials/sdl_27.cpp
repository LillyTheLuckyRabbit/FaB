#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
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

bool checkCollision( SDL_Rect a, SDL_Rect b ) {
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	leftA = a.x;
	rightA = a.x + a.w;
	topA = a.y;
	bottomA = a.y + a.h;

	leftB = b.x;
	rightB = b.x + b.w;
	topB = b.y;
	bottomB = b.y + b.h;
	
	if( bottomA <= topB ) {
		return false;
	}

	if( topA >= bottomB ) {
		return false;
	}

	if( rightA <= leftB ) {
		return false;
	}

	if( leftA >= rightB ) {
		return false;
	}

	return true;
}

class Dot {
	public:
		static const int DOT_WIDTH = 20;
		static const int DOT_HEIGHT = 20;

		static const int DOT_VEL = 10;

		Dot(SDL_Renderer* &gRenderer);

		void handleEvent(SDL_Event& e);

		void move(SDL_Rect& wall);

		void render(SDL_Renderer* &gRenderer);

	private:
		int mPosX, mPosY;

		int mVelX, mVelY;

		LTexture gDotTexture;

		SDL_Rect mCollider;
};

Dot::Dot(SDL_Renderer* &gRenderer) {
	mPosX = 0;
	mPosY = 0;

	mVelX = 0;
	mVelY = 0;

	mCollider.w = DOT_WIDTH;
	mCollider.h = DOT_HEIGHT;

	gDotTexture.loadFromFile("dot.png", gRenderer);
}

void Dot::handleEvent(SDL_Event& e) {
	if(e.type == SDL_KEYDOWN && e.key.repeat == 0) {
		switch( e.key.keysym.sym ) {
			case SDLK_UP: mVelY -= DOT_VEL; break;
			case SDLK_DOWN: mVelY += DOT_VEL; break;
			case SDLK_LEFT: mVelX -= DOT_VEL; break;
			case SDLK_RIGHT: mVelX += DOT_VEL; break;
		}
	} else if(e.type == SDL_KEYUP && e.key.repeat == 0) {
		switch(e.key.keysym.sym) {
            case SDLK_UP: mVelY += DOT_VEL; break;
            case SDLK_DOWN: mVelY -= DOT_VEL; break;
            case SDLK_LEFT: mVelX += DOT_VEL; break;
            case SDLK_RIGHT: mVelX -= DOT_VEL; break;
        }
	}
}

void Dot::move(SDL_Rect& wall) {
	mPosX += mVelX;
	mCollider.x = mPosX;

	if((mPosX < 0) || (mPosX + DOT_WIDTH > SCREEN_WIDTH) || checkCollision(mCollider, wall)) {
		mPosX -= mVelX;
		mCollider.x = mPosX;
	}

	mPosY += mVelY;
	mCollider.y = mPosY;
	
	if((mPosY < 0) || (mPosY + DOT_HEIGHT > SCREEN_HEIGHT) || checkCollision(mCollider, wall)) {
		mPosY -= mVelY;
		mCollider.y = mPosY;
	}
}


void Dot::render(SDL_Renderer* &gRenderer) {
	gDotTexture.render(mPosX, mPosY, gRenderer);
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
	bool quit = false;
	SDL_Event e;

	SDL_Rect wall;
	wall.x = 300;
	wall.y = 40;
	wall.w = 40;
	wall.h = 400;

	if(!initSdlWindow(gWindow, gRenderer)) {
		cout << "Failed to initialize!" << endl;
	} else {
		Dot dot(gRenderer);
		while(!quit) {
			while(SDL_PollEvent(&e) != 0) {
				if(e.type ==  SDL_QUIT) {
					quit = true;
				}
				dot.handleEvent(e);
			}
			dot.move(wall);
			
			SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderClear(gRenderer);

			SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
			SDL_RenderDrawRect(gRenderer, &wall);

			dot.render(gRenderer);
			
			SDL_RenderPresent(gRenderer);
		}
	}
	closeSdlWindow(gWindow, gRenderer);
	return(0);
}