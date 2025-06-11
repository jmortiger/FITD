#include "common.h"

/// @brief The height of on-screen messages & menu text I think. 16 for all but AITD2.
/// @todo Check conflict w/ `common.h`'s `MESSAGE_HEIGHT` & `systemMenu.cpp`'s `SIZE_FONT`.
int fontHeight = 16;

char* fontVar1 = NULL;
s16 fontSm1 = 0;
s16 fontSm2 = 0x1234;
char* fontVar4 = NULL;
extern char* fontVar4;
char* fontVar5 = NULL;
extern char* fontVar5;
s16 currentFontColor = 0;
extern s16 currentFontColor;
s16 g_fontInterWordSpace = 2;
s16 g_fontInterLetterSpace = 1;
/// @brief The color to draw the text in (?)
/// @todo Document & rename
s16 fontSm3 = 18;
/// @brief Seemingly the text's x position?
s16 fontVar6 = 0;
s16 fontSm7 = 0x1234;
s16 fontSm8 = 0x1234;
s16 fontSm9 = 0x80;

unsigned char flagTable[] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };

void ExtSetFont(char* fontData, int color)
{
	fontVar1 = fontData; // fontPtr

	s16 tempDx = READ_LE_S16(fontData); // alignement
	fontData += 2;

	fontSm1 = *(fontData++); // character height
	fontSm2 = *(unsigned char*)(fontData++); // character size

	if (!fontSm2) {
		fontSm2 = READ_LE_S16(fontData);
	}

	fontData += 2;

	s16 tempAxFlip = READ_LE_S16(fontData);
	fontData += 2;

	tempAxFlip = ((tempAxFlip & 0xFF) << 8) | ((tempAxFlip & 0xFF00) >> 8);

	fontVar4 = fontData;

	fontVar5 = fontVar1 + tempAxFlip - (tempDx & 0xFF) * 2;

	currentFontColor = color;

	fontSm3 = color;
}

void SetFontSpace(int interWordSpace, int interLetterSpace)
{
	g_fontInterWordSpace = interWordSpace;
	g_fontInterLetterSpace = interLetterSpace;
}

int ExtGetSizeFont(u8* string)
{
	int width = 0;
	u8 character;

	while ((character = *(string++))) {
		char* dataPtr = fontVar5 + character * 2;
		u16 data = READ_LE_S16(dataPtr);

		data >>= 4;

		data &= 0xF;

		if (!data) width += g_fontInterWordSpace;

		width += g_fontInterLetterSpace;
		width += data;
	}

	return(width);
}

void renderText(int x, int y, char* surface, u8* string)
{
	unsigned char character;

	fontVar6 = x;
	fontSm7 = y;

	// Until the null-terminator is reached...
	while ((character = *(string++))) {
		char* dataPtr = fontVar5 + character * 2;
		u16 data = READ_LE_U16(dataPtr);

		data = ((data & 0xFF) << 8) | ((data & 0xFF00) >> 8);

		u16 dx = data;

		data >>= 12;

		// If it's a real character (width != 0)...
		if (data & 0xF) {
			dx &= 0xFFF;

			char* characterPtr = (dx >> 3) + fontVar4;

			fontSm9 = flagTable[dx & 7];

			int bp = fontSm7;

			fontSm8 = fontVar6;

			for (int ch = fontSm1; ch > 0; ch--) {
				if (bp >= 200)
					return;
				char* outPtr = logicalScreen + bp * _SCREEN_INTERNAL_WIDTH + fontSm8;


				int dh = fontSm9;
				int cl = data & 0xF;

				int al = *characterPtr;

				bp++;

				for (int bx = 0; cl > 0; cl--) {
					if (dh & al) *(outPtr) = (char)fontSm3;

					outPtr++;

					dh = ((dh >> 1) & 0x7F) | ((dh << 7) & 0x80);

					if (dh & 0x80) {
						bx++;
						al = *(characterPtr + bx);
					}
				}

				characterPtr += fontSm2;
			}

			// Add the width
			fontVar6 += data & 0xF;
		} else { // Otherwise, it's a space character.
			fontVar6 += g_fontInterWordSpace;
		}

		fontVar6 += g_fontInterLetterSpace;
	}
}

/// @brief Draw given text message at given location with a drop-shadow
/// @param x 
/// @param y 
/// @param index 
/// @param color1 
/// @param color2 
/// @remark [Legacy name](https://docs.google.com/spreadsheets/d/1cYRTP37v7Y11O38okNyHPg1YrZx549GG6z2vhY7QRok/edit?gid=2024760462#gid=2024760462&range=D257)
void SelectedMessage(int x, int y, int index, int color1, int color2)
{
	textEntryStruct* entryPtr = getTextFromIdx(index);

	if (!entryPtr) return;

	x -= (entryPtr->width / 2); // center

	u8* textPtr = entryPtr->textPtr;

	ExtSetFont(PtrFont, color2);
	renderText(x, y + 1, logicalScreen, textPtr);

	ExtSetFont(PtrFont, color1);
	renderText(x, y, logicalScreen, textPtr);
}

/// @brief Draw given text message at given location
/// @param x 
/// @param y 
/// @param index 
/// @param color 
/// @remark [Legacy name](https://docs.google.com/spreadsheets/d/1cYRTP37v7Y11O38okNyHPg1YrZx549GG6z2vhY7QRok/edit?gid=2024760462#gid=2024760462&range=D256)
void SimpleMessage(int x, int y, int index, int color)
{
	textEntryStruct* entryPtr = getTextFromIdx(index);

	if (!entryPtr) return;

	x -= (entryPtr->width / 2); // center

	u8* textPtr = entryPtr->textPtr;

	ExtSetFont(PtrFont, color);

	renderText(x, y + 1, logicalScreen, textPtr);
}
