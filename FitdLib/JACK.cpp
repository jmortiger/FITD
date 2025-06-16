#include "common.h"

void startJACK()
{
	fontHeight = 16; // TODO: check
	startGame(16, 1, 1);
}

void JACK_ReadBook(int index, int type)
{
	switch (type) {
		case 1: // READ_BOOK
		{
			u8* pImage = (u8*)loadPak("ITD_RESS", JACK_LIVRE);
			memcpy(aux, pImage, _SCREEN_INTERNAL_WIDTH * _SCREEN_INTERNAL_HEIGHT);
			u8* lPalette = pImage + (_SCREEN_INTERNAL_WIDTH * _SCREEN_INTERNAL_HEIGHT);
			convertPaletteIfRequired(lPalette);
			copyPalette(lPalette, currentGamePalette);
			setPalette(lPalette);
			free(pImage);
			turnPageFlag = 1;
			Lire(index, 60, 10, 245, 190, 0, 124, 124);
			break;
		}
		default:
			FITD_throwFatal("Failed to read document; Expected 1; got %i", type);
	}
}
