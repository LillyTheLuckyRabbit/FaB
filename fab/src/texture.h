#ifndef TEXTURE_H
#define TEXTURE_H

#include "common.h"

class TextureWrapper {
	public:
	TextureWrapper();

	~TextureWrapper();

	bool loadFromFile(string path);

	void free();

	void setColor(Uint8 red, Uint8 green, Uint8 blue);

	void setBlendMode(SDL_BlendMode blending);

	void setAlpha(Uint8 alpha);

	void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

	int getWidth();
	int getHeight();

	private:
	SDL_Texture* texturePtr;

	int textureWidth;
	int textureHeight;
};

#endif