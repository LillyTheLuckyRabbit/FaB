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
const int TOTAL_WINDOWS = 3;

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

	void handleEvent(SDL_Event& e);

	void focus();

	void render();

	void free();

	int getWidth();
	int getHeight();

	bool hasMouseFocus();
	bool hasKeyboardFocus();
	bool isMinimized();
	bool isShown() { return mShown; };

	private:
	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	int mWindowID;

	int mWidth;
	int mHeight;

	bool mMouseFocus;
	bool mKeyboardFocus;
	bool mFullScreen;
	bool mMinimized;
	bool mShown;
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
	mWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if(mWindow != NULL) {
		mMouseFocus = true;
		mKeyboardFocus = true;
		mWidth = SCREEN_WIDTH;
		mHeight = SCREEN_HEIGHT;

		mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if(mRenderer == NULL) {
			cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << endl;
			SDL_DestroyWindow(mWindow);
			mWindow = NULL;
		} else {
			SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

			mWindowID = SDL_GetWindowID(mWindow);

			mShown = true;
		}
	} else {
		cout << "Window could not be created! SDL Error: " << SDL_GetError() << endl;
	}

	return mWindow != NULL && mRenderer != NULL;
}

void LWindow::handleEvent(SDL_Event& e) {
	if(e.type == SDL_WINDOWEVENT && e.window.windowID == mWindowID) {
		bool updateCaption = false;
		switch(e.window.event) {
			case SDL_WINDOWEVENT_SHOWN:
				mShown = true;
				break;

			case SDL_WINDOWEVENT_HIDDEN:
				mShown = false;
				break;

			case SDL_WINDOWEVENT_SIZE_CHANGED:
				mWidth = e.window.data1;
				mHeight = e.window.data2;
				SDL_RenderPresent(mRenderer);
				break;

			case SDL_WINDOWEVENT_EXPOSED:
				SDL_RenderPresent(mRenderer);
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

			case SDL_WINDOWEVENT_CLOSE:
				SDL_HideWindow(mWindow);
				break;
		}

		if(updateCaption) {
			std::stringstream caption;
			caption << "SDL Tutorial - ID: " << mWindowID << " MouseFocus:" << ((mMouseFocus) ? "On" : "Off") << " KeyboardFocus:" << ((mKeyboardFocus) ? "On" : "Off");
			SDL_SetWindowTitle(mWindow, caption.str().c_str());
		}
	}
}

void LWindow::focus() {
	if(!mShown) {
		SDL_ShowWindow(mWindow);
	}

	SDL_RaiseWindow(mWindow);
}

void LWindow::render() {
	if(!mMinimized) {
		SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(mRenderer);

		SDL_RenderPresent(mRenderer);
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

bool initSdlWindow(LWindow (&gWindows)[TOTAL_WINDOWS]) {
	bool success = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << endl;
		success = false;
	} else {
		if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
			cout << "Warning: Linear texture filtering not enabled!" << endl;
		}

		if(!gWindows[0].init()) {
			cout << "Window 0 could not be created!" << endl;
			success = false;
		}
	}

	return success;
}

void closeSdlWindow(LWindow (&gWindows)[TOTAL_WINDOWS]) {
	for(int i = 0; i < TOTAL_WINDOWS; ++i) {
		gWindows[i].free();
	}

	//	IMG_Quit();
	SDL_Quit();
}

int main(int argc, char* argv[]) {
	LWindow gWindows[TOTAL_WINDOWS];
	bool quit = false;
	SDL_Event e;
	//LTexture gTexture;

	if(!initSdlWindow(gWindows)) {
		cout << "Failed to initialize!" << endl;
	} else {
		for(int i = 1; i < TOTAL_WINDOWS; ++i) {
			gWindows[i].init();
		}
		//if(!gTexture.loadFromFile("plok.png", gRenderer)) {
		//	cout << "Failed to load background! SDL_image error: " << IMG_GetError()
		//	     << endl;
		//	return (1);
		//}
		while(!quit) {
			while(SDL_PollEvent(&e) != 0) {
				if(e.type == SDL_QUIT) {
					quit = true;
				}
				for(int i = 0; i < TOTAL_WINDOWS; ++i) {
					gWindows[i].handleEvent(e);
				}
				if(e.type == SDL_KEYDOWN) {
					switch(e.key.keysym.sym) {
						case SDLK_1:
							gWindows[0].focus();
							break;

						case SDLK_2:
							gWindows[1].focus();
							break;

						case SDLK_3:
							gWindows[2].focus();
							break;
					}
				}
			}

			for(int i = 0; i < TOTAL_WINDOWS; ++i) {
				gWindows[i].render();
			}

			bool allWindowsClosed = true;
			for(int i = 0; i < TOTAL_WINDOWS; ++i) {
				if(gWindows[i].isShown()) {
					allWindowsClosed = false;
					break;
				}
			}

			if(allWindowsClosed) quit = true;
		}
	}
	closeSdlWindow(gWindows);
	return (0);
}