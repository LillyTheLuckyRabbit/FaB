#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <fstream>
#include <iostream>

using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::string;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const int LEVEL_WIDTH = 1280;
const int LEVEL_HEIGHT = 960;

const int TILE_WIDTH = 80;
const int TILE_HEIGHT = 80;
const int TOTAL_TILES = 192;
const int TOTAL_TILE_SPRITES = 12;

const int TILE_RED = 0;
const int TILE_GREEN = 1;
const int TILE_BLUE = 2;
const int TILE_CENTER = 3;
const int TILE_TOP = 4;
const int TILE_TOPRIGHT = 5;
const int TILE_RIGHT = 6;
const int TILE_BOTTOMRIGHT = 7;
const int TILE_BOTTOM = 8;
const int TILE_BOTTOMLEFT = 9;
const int TILE_LEFT = 10;
const int TILE_TOPLEFT = 11;

class LTexture {
	public:
	LTexture();

	~LTexture();

	bool loadFromFile(string path, SDL_Renderer*& gRenderer);

	void free();

	void setColor(Uint8 red, Uint8 green, Uint8 blue);

	void setBlendMode(SDL_BlendMode blending);

	void setAlpha(Uint8 alpha);

	void render(int x, int y, SDL_Renderer*& gRenderer, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

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

bool LTexture::loadFromFile(string path, SDL_Renderer*& gRenderer) {
	free();

	SDL_Texture* newTexture = NULL;

	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if(loadedSurface == NULL) {
		cout << "Unable to load image %s! SDL_image Error: " << IMG_GetError();
	} else {
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if(newTexture == NULL) {
			cout << "Unable to create texture from \"" << path << "\"! SDL Error: " << SDL_GetError() << endl;
		} else {
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}
		SDL_FreeSurface(loadedSurface);
	}
	mTexture = newTexture;
	return mTexture != NULL;
}

void LTexture::free() {
	if(mTexture != NULL) {
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::render(int x, int y, SDL_Renderer*& gRenderer, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip) {
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

class Tile {
	public:
	Tile(int x, int y, int tileType, SDL_Renderer*& gRenderer);

	void render(SDL_Rect& camera, SDL_Renderer*& gRenderer);

	int getType() { return mType; }

	SDL_Rect getBox() { return mBox; }

	private:
	SDL_Rect mBox;

	int mType;

	LTexture gTileTexture;

	SDL_Rect gTileClips[TOTAL_TILE_SPRITES];
};

bool checkCollision(SDL_Rect a, SDL_Rect b) {
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

	if(bottomA <= topB) {
		return false;
	}

	if(topA >= bottomB) {
		return false;
	}

	if(rightA <= leftB) {
		return false;
	}

	if(leftA >= rightB) {
		return false;
	}

	return true;
}

Tile::Tile(int x, int y, int tileType, SDL_Renderer*& gRenderer) {
	mBox.x = x;
	mBox.y = y;

	mBox.w = TILE_WIDTH;
	mBox.h = TILE_HEIGHT;

	mType = tileType;

	gTileTexture.loadFromFile("tiles.png", gRenderer);

	gTileClips[TILE_RED].x = 0;
	gTileClips[TILE_RED].y = 0;
	gTileClips[TILE_RED].w = TILE_WIDTH;
	gTileClips[TILE_RED].h = TILE_HEIGHT;

	gTileClips[TILE_GREEN].x = 0;
	gTileClips[TILE_GREEN].y = 80;
	gTileClips[TILE_GREEN].w = TILE_WIDTH;
	gTileClips[TILE_GREEN].h = TILE_HEIGHT;

	gTileClips[TILE_BLUE].x = 0;
	gTileClips[TILE_BLUE].y = 160;
	gTileClips[TILE_BLUE].w = TILE_WIDTH;
	gTileClips[TILE_BLUE].h = TILE_HEIGHT;

	gTileClips[TILE_TOPLEFT].x = 80;
	gTileClips[TILE_TOPLEFT].y = 0;
	gTileClips[TILE_TOPLEFT].w = TILE_WIDTH;
	gTileClips[TILE_TOPLEFT].h = TILE_HEIGHT;

	gTileClips[TILE_LEFT].x = 80;
	gTileClips[TILE_LEFT].y = 80;
	gTileClips[TILE_LEFT].w = TILE_WIDTH;
	gTileClips[TILE_LEFT].h = TILE_HEIGHT;

	gTileClips[TILE_BOTTOMLEFT].x = 80;
	gTileClips[TILE_BOTTOMLEFT].y = 160;
	gTileClips[TILE_BOTTOMLEFT].w = TILE_WIDTH;
	gTileClips[TILE_BOTTOMLEFT].h = TILE_HEIGHT;

	gTileClips[TILE_TOP].x = 160;
	gTileClips[TILE_TOP].y = 0;
	gTileClips[TILE_TOP].w = TILE_WIDTH;
	gTileClips[TILE_TOP].h = TILE_HEIGHT;

	gTileClips[TILE_CENTER].x = 160;
	gTileClips[TILE_CENTER].y = 80;
	gTileClips[TILE_CENTER].w = TILE_WIDTH;
	gTileClips[TILE_CENTER].h = TILE_HEIGHT;

	gTileClips[TILE_BOTTOM].x = 160;
	gTileClips[TILE_BOTTOM].y = 160;
	gTileClips[TILE_BOTTOM].w = TILE_WIDTH;
	gTileClips[TILE_BOTTOM].h = TILE_HEIGHT;

	gTileClips[TILE_TOPRIGHT].x = 240;
	gTileClips[TILE_TOPRIGHT].y = 0;
	gTileClips[TILE_TOPRIGHT].w = TILE_WIDTH;
	gTileClips[TILE_TOPRIGHT].h = TILE_HEIGHT;

	gTileClips[TILE_RIGHT].x = 240;
	gTileClips[TILE_RIGHT].y = 80;
	gTileClips[TILE_RIGHT].w = TILE_WIDTH;
	gTileClips[TILE_RIGHT].h = TILE_HEIGHT;

	gTileClips[TILE_BOTTOMRIGHT].x = 240;
	gTileClips[TILE_BOTTOMRIGHT].y = 160;
	gTileClips[TILE_BOTTOMRIGHT].w = TILE_WIDTH;
	gTileClips[TILE_BOTTOMRIGHT].h = TILE_HEIGHT;
}

void Tile::render(SDL_Rect& camera, SDL_Renderer*& gRenderer) {
	if(checkCollision(camera, mBox)) {
		gTileTexture.render(mBox.x - camera.x, mBox.y - camera.y, gRenderer, &gTileClips[mType]);
	}
}

bool touchesWall(SDL_Rect box, Tile* tiles[]) {
	for(int i = 0; i < TOTAL_TILES; ++i) {
		if((tiles[i]->getType() >= TILE_CENTER) && (tiles[i]->getType() <= TILE_TOPLEFT)) {
			if(checkCollision(box, tiles[i]->getBox())) {
				return true;
			}
		}
	}

	return false;
}

class Dot {
	public:
	static const int DOT_WIDTH = 20;
	static const int DOT_HEIGHT = 20;

	static const int DOT_VEL = 10;

	Dot(SDL_Renderer*& gRenderer);

	void handleEvent(SDL_Event& e);

	void move(Tile* tiles[]);

	void setCamera(SDL_Rect& camera);

	void render(SDL_Rect& camera, SDL_Renderer*& gRenderer);

	private:
	SDL_Rect mBox;
	int mVelX, mVelY;
	LTexture gDotTexture;
};

Dot::Dot(SDL_Renderer*& gRenderer) {
	mBox.x = 0;
	mBox.y = 0;

	mVelX = 0;
	mVelY = 0;

	gDotTexture.loadFromFile("dot.png", gRenderer);
}

void Dot::render(SDL_Rect& camera, SDL_Renderer*& gRenderer) {
	gDotTexture.render(mBox.x - camera.x, mBox.y - camera.y, gRenderer);
}

void Dot::handleEvent(SDL_Event& e) {
	if(e.type == SDL_KEYDOWN && e.key.repeat == 0) {
		switch(e.key.keysym.sym) {
			case SDLK_UP:
				mVelY -= DOT_VEL;
				break;
			case SDLK_DOWN:
				mVelY += DOT_VEL;
				break;
			case SDLK_LEFT:
				mVelX -= DOT_VEL;
				break;
			case SDLK_RIGHT:
				mVelX += DOT_VEL;
				break;
		}
	} else if(e.type == SDL_KEYUP && e.key.repeat == 0) {
		switch(e.key.keysym.sym) {
			case SDLK_UP:
				mVelY += DOT_VEL;
				break;
			case SDLK_DOWN:
				mVelY -= DOT_VEL;
				break;
			case SDLK_LEFT:
				mVelX += DOT_VEL;
				break;
			case SDLK_RIGHT:
				mVelX -= DOT_VEL;
				break;
		}
	}
}

void Dot::move(Tile* tiles[]) {
	mBox.x += mVelX;

	if((mBox.x < 0) || (mBox.x + DOT_WIDTH > LEVEL_WIDTH) || touchesWall(mBox, tiles)) {
		mBox.x -= mVelX;
	}

	mBox.y += mVelY;

	if((mBox.y < 0) || (mBox.y + DOT_HEIGHT > LEVEL_HEIGHT) || touchesWall(mBox, tiles)) {
		mBox.y -= mVelY;
	}
}

void Dot::setCamera(SDL_Rect& camera) {
	camera.x = mBox.x - SCREEN_WIDTH / 2;
	camera.y = mBox.y - SCREEN_HEIGHT / 2;

	if(camera.x < 0) camera.x = 0;
	if(camera.x > LEVEL_WIDTH - SCREEN_WIDTH) camera.x = LEVEL_WIDTH - SCREEN_WIDTH;

	if(camera.y < 0) camera.y = 0;
	if(camera.y > LEVEL_HEIGHT - SCREEN_HEIGHT) camera.y = LEVEL_HEIGHT - SCREEN_HEIGHT;
}

bool setTiles(Tile* tiles[], SDL_Renderer*& gRenderer) {
	bool tilesLoaded = true;

	int x = 0, y = 0;

	ifstream map("lazy.map");

	if(map.fail()) {
		cout << "Unable to load map file!" << endl;
		tilesLoaded = false;
	} else {
		for(int i = 0; i < TOTAL_TILES; ++i) {
			int tileType = -1;

			map >> tileType;

			if(map.fail()) {
				cout << "Error loading map: Unexpected end of file!" << endl;
				tilesLoaded = false;
				break;
			}

			if((tileType >= 0) && (tileType < TOTAL_TILE_SPRITES)) {
				tiles[i] = new Tile(x, y, tileType, gRenderer);
			} else {
				cout << "Error loading map: Invalid tile type at " << i << "!" << endl;
				tilesLoaded = false;
				break;
			}
			x += TILE_WIDTH;

			if(x >= LEVEL_WIDTH) {
				x = 0;

				y += TILE_HEIGHT;
			}
		}
		map.close();
	}
	return tilesLoaded;
}

bool initSdlWindow(SDL_Window*& gWindow, SDL_Renderer*& gRenderer) {
	bool success = true;
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
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
			SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
			int imgFlags = IMG_INIT_PNG;
			if(!(IMG_Init(imgFlags) & imgFlags)) {
				cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << endl;
				success = false;
			}
		}
	}
	return (success);
}

void closeSdlWindow(SDL_Window*& gWindow, SDL_Renderer*& gRenderer) {
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	IMG_Quit();
	SDL_Quit();
}

int main(int argc, char* argv[]) {
	SDL_Window* gWindow = NULL;
	SDL_Renderer* gRenderer = NULL;
	bool quit = false;
	SDL_Event e;
	SDL_Rect camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	Tile* tileSet[TOTAL_TILES];

	if(!initSdlWindow(gWindow, gRenderer)) {
		cout << "Failed to initialize!" << endl;
	} else {
		if(!setTiles(tileSet, gRenderer)) {
			cout << "Failed to set tiles!" << endl;
			return 1;
		}
		Dot dot(gRenderer);
		while(!quit) {
			while(SDL_PollEvent(&e) != 0) {
				if(e.type == SDL_QUIT) {
					quit = true;
				}
				dot.handleEvent(e);
			}

			dot.move(tileSet);
			dot.setCamera(camera);

			SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderClear(gRenderer);

			for(int i = 0; i < TOTAL_TILES; ++i) {
				tileSet[i]->render(camera, gRenderer);
			}

			dot.render(camera, gRenderer);

			SDL_RenderPresent(gRenderer);
		}
	}
	closeSdlWindow(gWindow, gRenderer);
	return (0);
}