#ifndef _TATOU_H_
#define _TATOU_H_

int make3dTatou(void);
// #region To move
// #region Palette (Direct)
void paletteFill(void* palette, unsigned char r, unsigned char g, unsigned char b);
void paletteFill(void* palette, PaletteColorRGB color);
void copyPalette(unsigned char* source, unsigned char* dest);
void copyPalette(PaletteColorRGB* source, PaletteColorRGB* dest);
void copyPalette(PaletteColorRGB* source, unsigned char* dest);
void copyPalette(unsigned char* source, PaletteColorRGB* dest);
void setPalette(void* palette);
// #endregion Palette (Direct)
// #region Time
void startChrono(unsigned int* chrono);
int evalChrono(unsigned int* chrono);
// #endregion Time
void process_events(void);
void FastCopyScreen(void* source, void* dest);
void Rotate(unsigned int x, unsigned int y, unsigned int z, int* xOut, int* yOut);
void FadeInPhys(int step, int start);
void FadeOutPhys(int step, int start);
void playSound(int num);
void setCameraTarget(int x, int y, int z, int alpha, int beta, int gamma, int time);
// #endregion To move
#endif
