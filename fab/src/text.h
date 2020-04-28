/* File: text.h
 * Authors: David Butler and William Coar
 * Description: Declaration of text header
*/

#ifndef TEXT_H
#define TEXT_H

#include "common.h"
#include "constants.h"
#include "texture.h"

class Text {
	public:
	Text(string pathToFont, int h, int w);
	Text();
	void loadFont(string pathToFont, int h, int w);
	void render(int x, int y, string text, Uint8 red = 255, Uint8 green = 255, Uint8 blue = 255);
	int getFontH() const { return fontH; }
	int getFontW() const { return fontW; }

	private:
	int fontW;
	int fontH;
	TextureWrapper bitmapFont;
};

#endif