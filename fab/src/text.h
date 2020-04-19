#ifndef TEXT_H
#define TEXT_H

#include "common.h"
#include "constants.h"
#include "texture.h"

class Text {
	public:
	Text(string pathToFont, int h, int w);
	void render(int x, int y, string text, Uint8 red = 255, Uint8 green = 255, Uint8 blue = 255);
	int getFontH() const { return fontH; }
	int getFontW() const { return fontW; }

	private:
	int fontW;
	int fontH;
	TextureWrapper bitmapFont;
};

#endif