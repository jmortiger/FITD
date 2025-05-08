#ifndef _SCREEN_
#define _SCREEN_

/// @brief 
/// @details Game is running in dos resolution mode 13h, ie 320x200x256, but is displayed in 4:3 (320x240), so pixels are not square (1.6:1)
void setupScreen(void);

#if 1 // Coordinate conversion
#define _CornerCoordToCenterCoord(x, y, w, h) (x) + ((w) / 2), (y) + ((h) / 2)
#define CornerCoordToCenterCoord(x, y, w, h) _CornerCoordToCenterCoord(x, y, w, h)
#define _CornerRectToCenterRect(x, y, w, h) (x) + ((w) / 2), (y) + ((h) / 2), w, h
#define CornerRectToCenterRect(x, y, w, h) _CornerRectToCenterRect(x, y, w, h)
#else
/// @brief Takes a rectangle defined by its top-left coordinate & dimensions and converts it to its center coordinate & dimensions.
/// @param x 
/// @param y 
/// @param w 
/// @param h 
void CornerRectToCenterRect(int x, int y, int w, int h) { exit(1); }
/// @brief Takes a rectangle defined by its top-left coordinate & dimensions and returns its center coordinate.
/// @param x 
/// @param y 
/// @param w 
/// @param h 
void CornerCoordToCenterCoord(int x, int y, int w, int h) { exit(1); }
#endif
#endif
