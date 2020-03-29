#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <sstream>

using std::cin;
using std::cout;
using std::endl;
using std::string;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

class LTexture {
	public:
	LTexture();

	~LTexture();

	bool loadFromFile(string path, SDL_Renderer*& gRenderer);

	void free();

	void setColor(Uint8 red, Uint8 green, Uint8 blue);

	void setBlendMode(SDL_BlendMode blending);

	void setAlpha(Uint8 alpha);

	void render(int x,
	            int y,
	            SDL_Renderer*& gRenderer,
	            SDL_Rect* clip = NULL,
	            double angle = 0.0,
	            SDL_Point* center = NULL,
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

bool LTexture::loadFromFile(string path, SDL_Renderer*& gRenderer) {
	free();

	SDL_Texture* newTexture = NULL;

	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if(loadedSurface == NULL) {
		cout << "Unable to load image %s! SDL_image Error: " << IMG_GetError();
	} else {
		SDL_SetColorKey(loadedSurface, SDL_TRUE,
		                SDL_MapRGB(loadedSurface->format, 0xFF, 0, 0xFF));
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if(newTexture == NULL) {
			cout << "Unable to create texture from \"" << path
			     << "\"! SDL Error: " << SDL_GetError() << endl;
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

void LTexture::render(int x,
                      int y,
                      SDL_Renderer*& gRenderer,
                      SDL_Rect* clip,
                      double angle,
                      SDL_Point* center,
                      SDL_RendererFlip flip) {
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

class LWindow {
	public:
	LWindow();

	bool init();

	SDL_Renderer* createRenderer();

	void handleEvent(SDL_Event& e, SDL_Renderer*& gRenderer);

	void free();

	int getWidth();
	int getHeight();

	bool hasMouseFocus();
	bool hasKeyboardFocus();
	bool isMinimized();

	private:
	SDL_Window* mWindow;

	int mWidth;
	int mHeight;

	bool mMouseFocus;
	bool mKeyboardFocus;
	bool mFullScreen;
	bool mMinimized;
};

LWindow::LWindow() {
	mWindow = NULL;
	mMouseFocus = false;
	mKeyboardFocus = false;
	mFullScreen = false;
	mMinimized = false;
	mWidth = 0;
	mHeight = 0;
}

bool LWindow::init() {
	mWindow = SDL_CreateWindow(
	    "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
	    SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if(mWindow != NULL) {
		mMouseFocus = true;
		mKeyboardFocus = true;
		mWidth = SCREEN_WIDTH;
		mHeight = SCREEN_HEIGHT;
	}
	return mWindow != NULL;
}

SDL_Renderer* LWindow::createRenderer() {
	return SDL_CreateRenderer(
	    mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

void LWindow::handleEvent(SDL_Event& e, SDL_Renderer* &gRenderer) {
	if(e.type == SDL_WINDOWEVENT) {
		bool updateCaption = false;
		switch(e.window.event) {
			case SDL_WINDOWEVENT_SIZE_CHANGED:
				mWidth = e.window.data1;
				mHeight = e.window.data2;
				SDL_RenderPresent(gRenderer);
				break;

			case SDL_WINDOWEVENT_EXPOSED:
				SDL_RenderPresent(gRenderer);
				break;

			case SDL_WINDOWEVENT_ENTER:
				mMouseFocus = true;
				updateCaption = true;
				break;

			case SDL_WINDOWEVENT_LEAVE:
				mMouseFocus = false;
				updateCaption = true;
				break;

			case SDL_WINDOWEVENT_FOCUS_GAINED:
				mKeyboardFocus = true;
				updateCaption = true;
				break;

			case SDL_WINDOWEVENT_FOCUS_LOST:
				mKeyboardFocus = false;
				updateCaption = true;
				break;

			case SDL_WINDOWEVENT_MINIMIZED:
				mMinimized = true;
				break;

			case SDL_WINDOWEVENT_MAXIMIZED:
				mMinimized = false;
				break;

			case SDL_WINDOWEVENT_RESTORED:
				mMinimized = false;
				break;
		}
		if(updateCaption) {
			std::stringstream caption;
			caption << "SDL Tutorial - MouseFocus:" << ((mMouseFocus) ? "On" : "Off")
			        << " KeyboardFocus:" << ((mKeyboardFocus) ? "On" : "Off");
			SDL_SetWindowTitle(mWindow, caption.str().c_str());
		}
	} else if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN) {
		if(mFullScreen) {
			SDL_SetWindowFullscreen(mWindow, SDL_FALSE);
			mFullScreen = false;
		} else {
			SDL_SetWindowFullscreen(mWindow, SDL_TRUE);
			mFullScreen = true;
			mMinimized = false;
		}
	}
}

int LWindow::getWidth() {
	return mWidth;
}

int LWindow::getHeight() {
	return mHeight;
}

bool LWindow::hasMouseFocus() {
	return mMouseFocus;
}

bool LWindow::hasKeyboardFocus() {
	return mKeyboardFocus;
}

bool LWindow::isMinimized() {
	return mMinimized;
}

void LWindow::free() {
	if(mWindow) {
		SDL_DestroyWindow(mWindow);
		mWindow = NULL;
	}
}

bool initSdlWindow(LWindow& gWindow, SDL_Renderer*& gRenderer) {
	bool success = true;
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
		success = false;
	} else {
		if(!gWindow.init()) {
			cout << "Window could not be created! SDL_Error: " << SDL_GetError()
			     << endl;
			success = false;
		} else {
			gRenderer = gWindow.createRenderer();
			if(gRenderer == NULL) {
				cout << "Renderer could not be created! SDL Error: " << SDL_GetError()
				     << endl;
				success = false;
			}
			SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
			int imgFlags = IMG_INIT_PNG;
			if(!(IMG_Init(imgFlags) & imgFlags)) {
				cout << "SDL_image could not initialize! SDL_image Error: "
				     << IMG_GetError() << endl;
				success = false;
			}
		}
	}
	return (success);
}

void closeSdlWindow(LWindow& gWindow, SDL_Renderer*& gRenderer) {
	SDL_DestroyRenderer(gRenderer);
	gWindow.free();

	IMG_Quit();
	SDL_Quit();
}

int main(int argc, char* argv[]) {
	LWindow gWindow;
	SDL_Renderer* gRenderer = NULL;
	bool quit = false;
	SDL_Event e;
	LTexture gTexture;
	int scrollingOffset = 0;

	if(!initSdlWindow(gWindow, gRenderer)) {
		cout << "Failed to initialize!" << endl;
	} else {
		if(!gTexture.loadFromFile("plok.png", gRenderer)) {
			cout << "Failed to load background! SDL_image error: " << IMG_GetError()
			     << endl;
			return (1);
		}
		while(!quit) {
			while(SDL_PollEvent(&e) != 0) {
				if(e.type == SDL_QUIT) {
					quit = true;
				}
			}

			gWindow.handleEvent(e, gRenderer);

			if(!gWindow.isMinimized()) {
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

				gTexture.render((gWindow.getWidth() - gTexture.getWidth()) / 2, (gWindow.getHeight() - gTexture.getHeight()) / 2, gRenderer);

				SDL_RenderPresent(gRenderer);
			}
		}
	}
	closeSdlWindow(gWindow, gRenderer);
	return (0);
}