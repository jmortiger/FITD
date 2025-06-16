// seg 4

#include "common.h"

#ifdef PCLIKE
#include "SDL.h"
#endif


//////////// stuff to move


void blitScreenTatou(void)
{
	// for (int i = 0; i < 45120; i++) { frontBuffer[i] = backBuffer[i]; }
}

// #region `comparePalettes`
bool comparePalettes(unsigned char* source, unsigned char* dest)
{
	for (int i = 0; i < BYTES_IN_PALETTE; i++) { if (dest[i] != source[i]) return false; }
	return true;
}
bool comparePalettes(PaletteColorRGB* source, PaletteColorRGB* dest)
{
	for (int i = 0; i < COLORS_IN_PALETTE; i++) { if (dest[i] != source[i]) return false; }
	return true;
}
bool comparePalettes(unsigned char* source, PaletteColorRGB* dest) { return comparePalettes((PaletteColorRGB*)source, dest); }
bool comparePalettes(PaletteColorRGB* source, unsigned char* dest) { return comparePalettes(source, (PaletteColorRGB*)dest); }
/* // !!!!
bool comparePalettes(unsigned char* source, PaletteColorRGB* dest)
{
	for (int i = 0; i < BYTES_IN_PALETTE; i++) {
		if (dest[i] != (PaletteColorRGB)((PaletteColorRGB*)source)[i]) return false;
	}
	return true;
}

bool comparePalettes(PaletteColorRGB* source, unsigned char* dest)
{
	for (int i = 0; i < COLORS_IN_PALETTE; i++) {
		if (((PaletteColorRGB*)dest)[i] != source[i]) return false;
	}
	return true;//{ ((PaletteColorRGB*)dest)[i] = source[i]; }
} */
// #endregion `comparePalettes`
// #region `copyPalette`
void copyPalette(unsigned char* source, unsigned char* dest)
{
	for (int i = 0; i < BYTES_IN_PALETTE; i++) { dest[i] = source[i]; }
}

void copyPalette(PaletteColorRGB* source, PaletteColorRGB* dest)
{
	for (int i = 0; i < COLORS_IN_PALETTE; i++) { dest[i] = source[i]; }
}

void copyPalette(unsigned char* source, PaletteColorRGB* dest) { copyPalette((PaletteColorRGB*)source, dest); }
void copyPalette(PaletteColorRGB* source, unsigned char* dest) { copyPalette(source, (PaletteColorRGB*)dest); }
/* void copyPalette(unsigned char* source, PaletteColorRGB* dest)
{
	for (int i = 0; i < COLORS_IN_PALETTE; i++) {
		dest[i] = (PaletteColorRGB)((PaletteColorRGB*)source)[i];
		// dest[i].update(source + (i * BYTES_PER_PALETTE_COLOR));
	}
	// assert(comparePalettes(source, dest));
}

void copyPalette(PaletteColorRGB* source, unsigned char* dest)
{
	for (int i = 0; i < COLORS_IN_PALETTE; i++) { ((PaletteColorRGB*)dest)[i] = source[i]; }
	// assert(comparePalettes(source, dest));
} */
// #endregion `copyPalette`

// #region paletteFill
void paletteFill(void* palette, unsigned char r, unsigned char g, unsigned char b)
{
	// TODO: Why?
	r <<= 1;
	g <<= 1;
	b <<= 1;

	u8* paletteLocal = (u8*)palette;
	for (int i = 0, offset = 0; i < COLORS_IN_PALETTE; i++) {
		paletteLocal[offset] = r;
		paletteLocal[offset + 1] = g;
		paletteLocal[offset + 2] = b;
		offset += 3;
	}
}

void paletteFill(void* palette, PaletteColorRGB color)
{
	// TODO: Why?
	color.r <<= 1;
	color.g <<= 1;
	color.b <<= 1;

	PaletteColorRGB* paletteLocal = (PaletteColorRGB*)palette;
	for (int i = 0; i < COLORS_IN_PALETTE; i++) { paletteLocal[i] = color; }
}
// #endregion paletteFill

// #region `computePalette`
void computePalette(unsigned char* inPalette, unsigned char* outPalette, int coefficient)
{
	for (int i = 0; i < COLORS_IN_PALETTE; i++) {
		*(outPalette++) = ((*(inPalette++)) * coefficient) >> 8;
		*(outPalette++) = ((*(inPalette++)) * coefficient) >> 8;
		*(outPalette++) = ((*(inPalette++)) * coefficient) >> 8;
	}
}
void computePalette(unsigned char* inPalette, PaletteColorRGB* outPalette, int coefficient)
{
	computePalette((PaletteColorRGB*)inPalette, outPalette, coefficient);
}
void computePalette(PaletteColorRGB* inPalette, unsigned char* outPalette, int coefficient)
{
	computePalette(inPalette, (PaletteColorRGB*)outPalette, coefficient);
}
/* void computePalette(unsigned char* inPalette, PaletteColorRGB* outPalette, int coefficient)
{
	for (int i = 0; i < COLORS_IN_PALETTE; i++, outPalette++) {
		(*outPalette).r = ((*(inPalette++)) * coefficient) >> 8;
		(*outPalette).g = ((*(inPalette++)) * coefficient) >> 8;
		(*outPalette).b = ((*(inPalette++)) * coefficient) >> 8;
	}
}
void computePalette(PaletteColorRGB* inPalette, unsigned char* outPalette, int coefficient)
{
	for (int i = 0; i < COLORS_IN_PALETTE; i++, inPalette++) {
		*(outPalette++) = ((*(inPalette)).r * coefficient) >> 8;
		*(outPalette++) = ((*(inPalette)).g * coefficient) >> 8;
		*(outPalette++) = ((*(inPalette)).b * coefficient) >> 8;
	}
} */
void computePalette(PaletteColorRGB* inPalette, PaletteColorRGB* outPalette, int coefficient)
{
	for (int i = 0; i < COLORS_IN_PALETTE; i++, inPalette++, outPalette++) {
		(*outPalette).r = ((*(inPalette)).r * coefficient) >> 8;
		(*outPalette).g = ((*(inPalette)).g * coefficient) >> 8;
		(*outPalette).b = ((*(inPalette)).b * coefficient) >> 8;
	}
}
// #endregion `computePalette`

void setPalette(void* sourcePal) { osystem_setPalette((PaletteColorRGB*)sourcePal); }

void FastCopyScreen(void* source, void* dest) { memcpy(dest, source, 64000); }

/// @brief 
/// @param step 
/// @param start ALWAYS ZERO; UNUSED
/// @remark [Legacy name](https://docs.google.com/spreadsheets/d/1cYRTP37v7Y11O38okNyHPg1YrZx549GG6z2vhY7QRok/edit?gid=2024760462#gid=2024760462&range=D171)
/// @todo Move to module AMBIANCE?
void FadeInPhys(int step, int start)
{
	freezeTime();
	
	u8 localPalette[BYTES_IN_PALETTE];

	// only used for the ending ?
	if (fadeState == 2) {} else {
		for (int i = 0; i < 256; i += step) {
			process_events();
			computePalette(currentGamePalette, localPalette, i);
			setPalette(localPalette);
			osystem_refreshFrontTextureBuffer();
			osystem_drawBackground();
		}
	}

	fadeState = 1;

	unfreezeTime();
}

/// @brief 
/// @param step 
/// @param start ALWAYS ZERO; UNUSED
/// @remark [Legacy name](https://docs.google.com/spreadsheets/d/1cYRTP37v7Y11O38okNyHPg1YrZx549GG6z2vhY7QRok/edit?gid=2024760462#gid=2024760462&range=D172)
/// @todo Move to module AMBIANCE?
void FadeOutPhys(int step, int start)
{
	freezeTime();
	
	u8 localPalette[BYTES_IN_PALETTE];

	for (int i = 256; i >= 0; i -= step) {
		process_events();
		computePalette(currentGamePalette, localPalette, i);
		setPalette(localPalette);
		osystem_refreshFrontTextureBuffer();
		osystem_drawBackground();
	}

	unfreezeTime();
}

/// @brief Advance tick (move time forward & prep rendering?)
/// @param  
void process_events(void)
{
#ifdef PCLIKE
	/* Our SDL event placeholder. */
	SDL_Event event;

	/* Grab all the events off the queue. */
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_KEYDOWN:
				/* Handle key presses. */
				// handle_key_down( &event.key.keysym );
				break;
			case SDL_QUIT:
				/* Handle quit requests (like Ctrl-c). */
				exit(0);
				break;
		}
	}
#else
	osystem_endOfFrame();
	u32 timeIncrease = osystem_startOfFrame();
	assert(timeIncrease);
#ifdef FITD_DEBUGGER
	if (debuggerVar_fastForward) timeIncrease = 8;
#endif
	timeGlobal += timeIncrease;
	timer = timeGlobal;
#endif
}

void startChrono(unsigned int* chrono) { *chrono = timer; }

int evalChrono(unsigned int* chrono) { return(timer - *chrono); }

// bp = x, bx = y, cx = z
// out
// xOut = dx, yOut = ax
void Rotate(unsigned int x, unsigned int y, unsigned int z, int* xOut, int* yOut)
{
	// TODO: Document the math going on here
	if (x) {
		int var1 = (((cosTable[(x + 0x100) & 0x3FF] * y) << 1) & 0xFFFF0000) - (((cosTable[x & 0x3FF] * z) << 1) & 0xFFFF0000);
		int var2 = (((cosTable[x & 0x3FF] * y) << 1) & 0xFFFF0000) + (((cosTable[(x + 0x100) & 0x3FF] * z) << 1) & 0xFFFF0000);

		*yOut = var1 >> 16;
		*xOut = var2 >> 16;
	} else {
		*xOut = z;
		*yOut = y;
	}
}

void setCameraTarget(int x, int y, int z, int alpha, int beta, int gamma, int time)
{
	int x1;
	int y1;
	int x2;
	int y2;

	Rotate(alpha + 0x200, -time, 0, &x1, &y1);
	Rotate(beta + 0x200, y1, 0, &x2, &y2);

	SetPosCamera(x2 + x, -x1 + y, y2 + z);
	SetAngleCamera(alpha, beta, gamma);
}

#include <filesystem>
extern "C" { extern char homePath[512]; }

/// @brief 
/// @param num 
void playSound(int num)
{
	DebugPrintfLnCategory(DBO_L_INFO, DBO_SOUND, "playSound(%i):%s", num, num == -1 ? " early exit." : "");
	if (num == -1) return;
	DebugBeginSection(DBO_SOUND);

	/* char sampleFileName[9] = ""; // char sampleFileName[256] = "";
	if (g_gameId == TIMEGATE) {
		strcpy(sampleFileName, "SAMPLES");
	} else {
		strcpy(sampleFileName, "LISTSAMP");
	}

	int size = getPakSize(sampleFileName, num); */
	// std::filesystem::path path = std::filesystem::path(homePath) / "SFX" / (g_gameId == TIMEGATE ? "SAMPLES" : "LISTSAMP");
	// path += ".PAK.";
	// path += num;
	// path += ".wav";
	// if (std::filesystem::exists(path)) {
	// 	osystem_playSampleFromName(path.c_str());
	// }
	int size = getPakSize(g_gameId == TIMEGATE ? "SAMPLES" : "LISTSAMP", num);
	DebugPrintfLn(size == 0 ? DBO_L_WARN : DBO_L_INFO, "Sample %i has size %i%s", num, size, size == 0 ? "; exiting early" : "");
	assert(size);

	char* ptr = HQR_Get(listSamp, num);
	DebugPrintfLn(ptr == 0 ? DBO_L_WARN : DBO_L_INFO, "Sample %i is at address %lx%s", num, ptr, ptr == 0 ? "; exiting early" : "");
	assert(ptr);

	osystem_playSample(ptr, size);
	DebugEndSection();
}

////////////////////////

