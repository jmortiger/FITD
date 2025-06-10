#ifndef _OSYSTEM_H_
#define _OSYSTEM_H_

#ifdef __cplusplus
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#ifdef FITD_DEBUGGER
#include "debugging/debugFont.h"
#endif

extern "C" {
#endif

#define byte char
	//#ifndef bool
	//#define bool char
	//#endif
#define u8 unsigned char
#define true 1
#define false 0

	enum e_rendererType
	{
		OPENGL_OLD,
		OPENGL_ES,
		OPENGL_3_2,
	};

	extern enum e_rendererType g_rendererType;

	void osystem_init();

	// TODO: Pull all input-related code, disabled & otherwise, into dedicated files (`input.h` & `input.cpp`?).
	// Unused w/o even a comment-out reference anywhere beyond declaration & init, so disabled.
	/* extern int osystem_mouseRight;
	extern int osystem_mouseLeft; */
	//    void getMouseStatus(mouseStatusStruct * mouseData);

	void osystem_delay(int ms);

	void osystem_drawBackground();
	u32 osystem_startOfFrame();
	void osystem_endOfFrame();

	void osystem_initGL(int screenWidth, int screenHeight);

	void osystem_updateImage();
	void osystem_initBuffer();
	void osystem_getPalette(void* palette);
	void osystem_setPalette(void* palette);
	void osystem_flip(u8* videoBuffer);
	void osystem_CopyBlockPhys(u8* videoBuffer, int left, int top, int right, int bottom);
	void osystem_refreshFrontTextureBuffer();
	/// @todo Unimplemented; Remove?
	void osystem_drawLine(int X1, int X2, int Y1, int Y2, u8 color, u8* palette);

	void osystem_createMask(const std::array<u8, _SCREEN_INTERNAL_PIXELS>& mask, int roomId, int maskId, u8* refImage, int maskX1, int maskY1, int maskX2, int maskY2);
	void osystem_drawMask(int roomId, int maskId);

	void osystem_startFrame();
	void osystem_stopFrame();
	void osystem_startModelRender();
	void osystem_stopModelRender();

	// clip
	void osystem_setClip(float left, float top, float right, float bottom);
	void osystem_clearClip();

	/// @todo Unimplemented, but commented-out reference in `processInventory`; Remove?
	void osystem_cleanScreenKeepZBuffer();

	void osystem_fillPoly(float* buffer, int numPoint, unsigned char color, u8 polyType);
	void osystem_draw3dLine(float x1, float y1, float z1, float x2, float y2, float z2, unsigned char color);
	void osystem_draw3dQuad(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float x4, float y4, float z4, unsigned char color, int transparency);
	void osystem_drawSphere(float X, float Y, float Z, u8 color, u8 material, float size);
	void osystem_drawPoint(float X, float Y, float Z, u8 color, u8 material, float size);
	void osystem_flushPendingPrimitives();

	/* // #region Unused w/o even a comment-out reference anywhere beyond declaration, so disabled.
	void osystem_fadeBlackToWhite();

	void osystem_initVideoBuffer(char* buffer, int width, int height);
	void osystem_putpixel(int x, int y, int pixel);
	void osystem_setColor(byte i, byte R, byte G, byte B);
	void osystem_setPalette320x200(byte* palette);
	void osystem_draw320x200BufferToScreen(unsigned char* videoBuffer);
	void osystem_drawText(int X, int Y, char* text);
	void osystem_drawTextColor(int X, int Y, char* string, unsigned char R, unsigned char G, unsigned char B);

	void osystem_startBgPoly();
	void osystem_endBgPoly();
	void osystem_addBgPolyPoint(int x, int y);
	// #endregion Unused w/o even a comment-out reference anywhere beyond declaration, so disabled. */

	// #region Audio
	/// @todo Move to AL?
	void osystem_playSample(char* samplePtr, int size);
	/// @todo Unimplemented; Remove/Implement?
	/// @todo Move to AL?
	void osystem_playSampleFromName(char* sampleName);

	/// @todo Move to AL?
	int osystem_playTrack(int trackId);
	/// @todo Move to AL?
	void osystem_playAdlib();
	// #endregion Audio

#ifdef __cplusplus
}
#endif

#endif
