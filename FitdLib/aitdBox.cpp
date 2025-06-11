#include "common.h"

// #region AffSpfI/afficheSprite
/* /// @brief
/// @param left
/// @param top
/// @param index
/// @param gfxData
/// @param forceUpdate
/// @details `afficheSprite` & `AffSpfI` are virtually identical, so this can be used to toggle which version is used by changing `forceUpdate`.
void afficheSpriteShell(int left, int top, int index, char* gfxData, bool forceUpdate = false)
{
	if (g_gameId >= AITD3)
		return;

	char* outPtr = logicalScreen + top * _SCREEN_INTERNAL_WIDTH + left;
	char* inPtr = gfxData + READ_LE_U16(index * 2 + gfxData); // alignement unsafe

	inPtr += 4;

	int width = READ_LE_U16(inPtr); // alignement unsafe
	inPtr += 2;
	int height = READ_LE_U16(inPtr); // alignement unsafe
	inPtr += 2;

	int offset = _SCREEN_INTERNAL_WIDTH - width;

	for (uint i = 0, j; i < height; i++, outPtr += offset) {
		for (j = 0; j < width; j++, inPtr++, outPtr++) {
			char color = *inPtr;
			if (forceUpdate || color) *(outPtr) = color;
		}
	}
} */

/// @brief private; Poster sprite; Plaster the given sprite from the given graphics data at the specified point on the screen.
/// @param left 
/// @param top 
/// @param index 
/// @param gfxData 
/// @note Seemingly had a legacy name of [AffSpf](<https://docs.google.com/spreadsheets/d/1cYRTP37v7Y11O38okNyHPg1YrZx549GG6z2vhY7QRok/edit?gid=2024760462#gid=2024760462&range=D297>)
void afficheSprite(int left, int top, int index, char* gfxData)
{
	if (g_gameId >= AITD3)
		return;

	char* outPtr = logicalScreen + top * _SCREEN_INTERNAL_WIDTH + left;
	char* inPtr = gfxData + READ_LE_U16(index * 2 + gfxData); // alignement unsafe

	inPtr += 4;

	int width = READ_LE_U16(inPtr); // alignement unsafe
	inPtr += 2;
	int height = READ_LE_U16(inPtr); // alignement unsafe
	inPtr += 2;

	int offset = _SCREEN_INTERNAL_WIDTH - width;

	for (uint i = 0, j; i < height; i++) {
		for (j = 0; j < width; j++) {
			*(outPtr++) = *(inPtr++);
		}

		outPtr += offset;
	}
}
// void afficheSprite(int left, int top, int index, char* gfxData) { afficheSpriteShell(left, top, index, gfxData, true); }

/// @brief Same as `afficheSprite`, but only updates colors when the resultant color != 0
/// @param left 
/// @param top 
/// @param index 
/// @param gfxData 
/// @remark [Legacy name](<https://docs.google.com/spreadsheets/d/1cYRTP37v7Y11O38okNyHPg1YrZx549GG6z2vhY7QRok/edit?gid=2024760462#gid=2024760462&range=D298>)
void AffSpfI(int left, int top, int index, char* gfxData)
{
	if (g_gameId >= AITD3)
		return;

	char* outPtr = logicalScreen + top * _SCREEN_INTERNAL_WIDTH + left;
	char* inPtr = gfxData + READ_LE_U16(index * 2 + gfxData); // alignement unsafe

	inPtr += 4;

	int width = READ_LE_U16(inPtr); // alignement unsafe
	inPtr += 2;
	int height = READ_LE_U16(inPtr); // alignement unsafe
	inPtr += 2;

	int offset = _SCREEN_INTERNAL_WIDTH - width;

	for (uint i = 0, j; i < height; i++) {
		for (j = 0; j < width; j++) {
			char color = *(inPtr++);
			if (color) *(outPtr) = color;
			outPtr++;
		}

		outPtr += offset;
	}
}
// void AffSpfI(int left, int top, int index, char* gfxData) { afficheSpriteShell(left, top, index, gfxData, false); }
// #endregion AffSpfI/afficheSprite

/// @brief Poster Big Frame; Plaster the frame graphics data in `PtrCadre` at the specified point on the screen with the specified size.
/// @param x The x coordinate of the frame's center.
/// @param y The y coordinate of the frame's center.
/// @param width 
/// @param height 
/// @details Has side effects:
/// * `WindowX1`, `WindowY1`, `WindowX2`, `WindowY2`: Changes to define the border of the drawn box (?)
/// @todo Causes off by 1 problems, make non-centered variant.
/// @remark [Legacy name](https://docs.google.com/spreadsheets/d/1cYRTP37v7Y11O38okNyHPg1YrZx549GG6z2vhY7QRok/edit?gid=2024760462#gid=2024760462&range=D255)
void AffBigCadre(int x, int y, int width, int height)
{
	int top;
	int right;
	int left;
	int bottom;

	int currentLeftPosition;
	int currentTopPosition;

	int halfWidth;
	int halfHeight;

	SetClip(0, 0, (_SCREEN_INTERNAL_WIDTH - 1), (_SCREEN_INTERNAL_HEIGHT - 1));

	halfWidth = width / 2;
	currentLeftPosition = left = x - halfWidth;

	halfHeight = height / 2;
	currentTopPosition = top = y - halfHeight;

	right = x + halfWidth;
	bottom = y + halfHeight;

	afficheSprite(currentLeftPosition, currentTopPosition, 0, PtrCadre); // draw top left corner

	while (1) // draw top bar
	{
		currentLeftPosition += 20;

		if (right - 20 <= currentLeftPosition)
			break;

		afficheSprite(currentLeftPosition, currentTopPosition, 4, PtrCadre);
	}

	afficheSprite(currentLeftPosition, currentTopPosition, 1, PtrCadre); // draw top right corner

	currentLeftPosition = left;

	while (1) // draw left bar
	{
		currentTopPosition += 20;

		if (bottom - 20 <= currentTopPosition)
			break;

		afficheSprite(currentLeftPosition, currentTopPosition, 6, PtrCadre);
	}

	currentLeftPosition = right - 8;
	currentTopPosition = top + 20;

	while (bottom - 20 > currentTopPosition) {
		afficheSprite(currentLeftPosition, currentTopPosition, 7, PtrCadre);

		currentTopPosition += 20;
	}

	currentLeftPosition = left;

	afficheSprite(currentLeftPosition, currentTopPosition, 2, PtrCadre); // draw bottom left corner

	while (1) // draw bottom bar
	{
		currentLeftPosition += 20;

		if (right - 20 <= currentLeftPosition)
			break;

		afficheSprite(currentLeftPosition, currentTopPosition + 12, 5, PtrCadre);
	}

	afficheSprite(currentLeftPosition, currentTopPosition, 3, PtrCadre); // draw bottom right corner

	afficheSprite(x - 20, currentTopPosition + 12, 8, PtrCadre); // draw "in the dark"

	WindowX1 = left + 8;
	WindowY1 = top + 8;
	WindowX2 = right - 9;
	WindowY2 = bottom - 9;

	fillBox(WindowX1, WindowY1, WindowX2, WindowY2, 0);
	SetClip(WindowX1, WindowY1, WindowX2, WindowY2);
}
