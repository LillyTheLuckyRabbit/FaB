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
		cout << "Unable to load image %s! SDL_image Error: " << IMG_GetError();
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