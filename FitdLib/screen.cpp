// seg 048

#include "common.h"

void setupScreen(void)
{
	logicalScreen = (char*)malloc(_SCREEN_BUFFER_SIZE);

	screenBufferSize = _SCREEN_BUFFER_SIZE;

	unkScreenVar2 = 3;

	// TODO: remain of screen init

}

void flushScreen(void)
{
	int i;
	int j;

	for (i = 0; i < _SCREEN_INTERNAL_HEIGHT; i++) {
		for (j = 0; j < _SCREEN_INTERNAL_WIDTH; j++) {
			*(logicalScreen + i * _SCREEN_INTERNAL_WIDTH + j) = 0;
		}
	}
}
