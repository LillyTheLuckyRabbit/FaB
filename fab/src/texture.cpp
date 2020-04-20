/* File: texture.cpp
 * Authors: David Butler and William Coar
 * Description: Wrapper class for SDL texture
*/

#include "texture.h"

// Forward declarations of globals
extern SDL_Window* gameWindow;
extern SDL_Renderer* gameRenderer;

TextureWrapper::TextureWrapper() {
	texturePtr = NULL;
	textureWidth = 0;
	textureHeight = 0;
}

TextureWrapper::~TextureWrapper() {
	free();
}

bool TextureWrapper::loadFromFile(string path) {
	free();

	SDL_Texture* newTexture = NULL;

	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if(loadedSurface == NULL) {
		cout << "Unable to load image! SDL_image Error: " << IMG_GetError();
	} else {
		newTexture = SDL_CreateTextureFromSurface(gameRenderer, loadedSurface);
		if(newTexture == NULL) {
			cout << "Unable to create texture from \"" << path << "\"! SDL Error: " << SDL_GetError() << endl;
		} else {
			textureWidth = loadedSurface->w;
			textureHeight = loadedSurface->h;
		}
		SDL_FreeSurface(loadedSurface);
	}
	texturePtr = newTexture;
	return texturePtr != NULL;
}

void TextureWrapper::free() {
	if(texturePtr != NULL) {
		SDL_DestroyTexture(texturePtr);
		texturePtr = NULL;
		textureWidth = 0;
		textureHeight = 0;
	}
}

void TextureWrapper::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip) {
	SDL_Rect renderQuad = {x, y, textureWidth, textureHeight};
	if(clip != NULL) {
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}
	SDL_RenderCopyEx(gameRenderer, texturePtr, clip, &renderQuad, angle, center, flip);
}

int TextureWrapper::getWidth() {
	return textureWidth;
}

int TextureWrapper::getHeight() {
	return textureHeight;
}

void TextureWrapper::setColor(Uint8 red, Uint8 green, Uint8 blue) {
	SDL_SetTextureColorMod(texturePtr, red, green, blue);
}

void TextureWrapper::setBlendMode(SDL_BlendMode blending) {
	SDL_SetTextureBlendMode(texturePtr, blending);
}

void TextureWrapper::setAlpha(Uint8 alpha) {
	SDL_SetTextureAlphaMod(texturePtr, alpha);
}

TextureWrapperStreaming::TextureWrapperStreaming() {
	texturePtr = NULL;
	textureWidth = 0;
	textureHeight = 0;
	pixelsPtr = NULL;
	pitch = 0;
}

void TextureWrapperStreaming::free() {
	if(texturePtr != NULL) {
		SDL_DestroyTexture(texturePtr);
		texturePtr = NULL;
		textureWidth = 0;
		textureHeight = 0;
		pixelsPtr = NULL;
		pitch = 0;
	}
}

bool TextureWrapperStreaming::loadFromFile(string path) {
	free();

	SDL_Texture* newTexture = NULL;

	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if(loadedSurface == NULL) {
		cout << "Unable to load image %s! SDL_image Error: " << IMG_GetError();
	} else {
		SDL_Surface* formattedSurface = SDL_ConvertSurfaceFormat(loadedSurface, SDL_GetWindowPixelFormat(gameWindow), 0);
		if(formattedSurface == NULL) {
			cout << "Unable to convert loaded surface to display format! SDL Error: " << SDL_GetError() << endl;
		} else {
			newTexture = SDL_CreateTexture(gameRenderer, SDL_GetWindowPixelFormat(gameWindow), SDL_TEXTUREACCESS_STREAMING, formattedSurface->w, formattedSurface->h);
			if(newTexture == NULL) {
				cout << "Unable to create blank texture! SDL Error: " << SDL_GetError() << endl;
			} else {
				SDL_LockTexture(newTexture, NULL, &pixelsPtr, &pitch);

				memcpy(pixelsPtr, formattedSurface->pixels, formattedSurface->pitch * formattedSurface->h);

				SDL_UnlockTexture(newTexture);
				pixelsPtr = NULL;

				textureWidth = formattedSurface->w;
				textureHeight = formattedSurface->h;
			}

			SDL_FreeSurface(formattedSurface);
		}

		SDL_FreeSurface(loadedSurface);
	}

	texturePtr = newTexture;
	return texturePtr != NULL;
}

bool TextureWrapperStreaming::createBlank(int w, int h) {
	free();

	SDL_Texture* newTexture = NULL;

	newTexture = SDL_CreateTexture(gameRenderer, SDL_GetWindowPixelFormat(gameWindow), SDL_TEXTUREACCESS_STREAMING, w, h);
	if(newTexture == NULL) {
		cout << "Unable to create blank texture! SDL Error: " << SDL_GetError() << endl;
	} else {
		textureWidth = w;
		textureHeight = h;
	}

	texturePtr = newTexture;
	return texturePtr != NULL;
}

bool TextureWrapperStreaming::lockTexture() {
	bool success = true;

	if(pixelsPtr != NULL) {
		cout << "Texture is already locked!" << endl;
		success = false;
	} else {
		if(SDL_LockTexture(texturePtr, NULL, &pixelsPtr, &pitch) != 0) {
			cout << "Unable to lock texture! " << SDL_GetError() << endl;
			success = false;
		}
	}

	return success;
}

bool TextureWrapperStreaming::unlockTexture() {
	bool success = true;

	if(pixelsPtr == NULL) {
		cout << "Texture is not locked!" << endl;
		success = false;
	} else {
		SDL_UnlockTexture(texturePtr);
		pixelsPtr = NULL;
		pitch = 0;
	}

	return success;
}