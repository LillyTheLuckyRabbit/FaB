/* File: texture.cpp
 * Authors: David Butler and Lillian Coar
 * Description: Wrapper class for SDL texture
*/

#include "texture.h"

// Forward declarations of globals
extern SDL_Window* gameWindow;
extern SDL_Renderer* gameRenderer;

//Default contructor
TextureWrapper::TextureWrapper() {
	texturePtr = NULL;
	textureWidth = 0;
	textureHeight = 0;
}

//Calls the free member to deallocate the texture
TextureWrapper::~TextureWrapper() {
	free();
}

//Deallocates any existing texture, loads a sprite to a surface, and converts
//the surface to a texture
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

//Deallocates the texture
void TextureWrapper::free() {
	if(texturePtr != NULL) {
		SDL_DestroyTexture(texturePtr);
		texturePtr = NULL;
		textureWidth = 0;
		textureHeight = 0;
	}
}

//Basically a wrapper for SDL_RenderCopyEx
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

//Set the amount of red, green, and blue in a texture
void TextureWrapper::setColor(Uint8 red, Uint8 green, Uint8 blue) {
	SDL_SetTextureColorMod(texturePtr, red, green, blue);
}

//Blend modes can be used for more complex texture blending
//SDL's built-in modes are additive blending, color modulation, and alpha blending
void TextureWrapper::setBlendMode(SDL_BlendMode blending) {
	SDL_SetTextureBlendMode(texturePtr, blending);
}

void TextureWrapper::setAlpha(Uint8 alpha) {
	SDL_SetTextureAlphaMod(texturePtr, alpha);
}

//Streaming textures can be modified on a per-pixel basis

//Default contructor
TextureWrapperStreaming::TextureWrapperStreaming() {
	texturePtr = NULL;
	textureWidth = 0;
	textureHeight = 0;
	pixelsPtr = NULL;
	pitch = 0;
}

//Deallocate function
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

//Deallocates any existing texture, loads a sprite to a surface, and converts
//the surface to a texture
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

//Creates a blank texture
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

//Locks texture, allowing modification
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

//Unlocks texture, disallowing modification
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