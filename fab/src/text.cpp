#include "text.h"

Text::Text(string pathToFont, int h, int w) {
	if(!bitmapFont.loadFromFile(pathToFont)) {
		cout << "Failed to load the player sprite!" << endl;
	}
	fontH = h;
	fontW = w;
}

void Text::render(int x, int y, string text, Uint8 red, Uint8 green, Uint8 blue) {
	bitmapFont.setColor(red, green, blue);
	SDL_Rect currentChar;
	currentChar.w = fontW;
	currentChar.h = fontH;
	currentChar.x = 0;
	currentChar.y = 0;
	for(int i = 0; text[i] != 0; i++) {
		currentChar.x = (text[i] - 32) * fontW;
		if(currentChar.x < 0) currentChar.x = 0;
		bitmapFont.render(x + fontW * i, y, &currentChar);
	}
}