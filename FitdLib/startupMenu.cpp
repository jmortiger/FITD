#include "common.h"

void drawStartupMenu(int selectedEntry)
{
	int currentY = 76;
	int currentTextNum = 0;

	// Draw a box in the center of the screen spanning the screen's width.
	AffBigCadre((_SCREEN_INTERNAL_WIDTH / 2), (_SCREEN_INTERNAL_HEIGHT / 2), _SCREEN_INTERNAL_WIDTH, 80);

	while (currentTextNum < 3) {
		// highlight selected entry
		if (currentTextNum == selectedEntry) {
			fillBox(10, currentY, 309, currentY + 16, 100);
			SelectedMessage(160, currentY, currentTextNum + 11, 15, 4);
		} else {
			SimpleMessage(160, currentY, currentTextNum + 11, 4);
		}

		currentY += 16; // next line
		currentTextNum++; // next text
	}
}

int processStartupMenu(void)
{
	int currentSelectedEntry = 0;
	unsigned int chrono;
	int selectedEntry = -1;

	flushScreen();

	drawStartupMenu(0);

	osystem_startFrame();
	osystem_stopFrame();
	osystem_CopyBlockPhys((unsigned char*)logicalScreen, 0, 0, _SCREEN_INTERNAL_WIDTH, _SCREEN_INTERNAL_HEIGHT);

	osystem_flip(NULL);
	FadeInPhys(16, 0);
	startChrono(&chrono);

	// exit loop only if time out or if choice made
	while (evalChrono(&chrono) <= 0x10000) {
		osystem_CopyBlockPhys((unsigned char*)logicalScreen, 0, 0, _SCREEN_INTERNAL_WIDTH, _SCREEN_INTERNAL_HEIGHT);
		osystem_startFrame();

		if (selectedEntry != -1 || evalChrono(&chrono) > 0x10000) {
			break;
		}

		process_events();
		osystem_drawBackground();

		// up key
		if (JoyD & 1) {
			currentSelectedEntry--;

			if (currentSelectedEntry < 0) {
				currentSelectedEntry = 2;
			}

			drawStartupMenu(currentSelectedEntry);
			osystem_flip(NULL);
			//      menuWaitVSync();

			startChrono(&chrono);

			while (JoyD) {
				process_events();
			}
		}


		// down key
		if (JoyD & 2) {
			currentSelectedEntry++;

			if (currentSelectedEntry > 2) {
				currentSelectedEntry = 0;
			}

			drawStartupMenu(currentSelectedEntry);
			//menuWaitVSync();
			osystem_flip(NULL);

			startChrono(&chrono);

			while (JoyD) {
				process_events();
			}
		}

		// select current entry
		if (key == 28 || (key != 28 && Click != 0)) {
			selectedEntry = currentSelectedEntry;
		}
		osystem_stopFrame();
		osystem_flip(NULL);
	}

	// if exit game, do not fade
	if (selectedEntry == 2) {
		FadeOutPhys(16, 0);
	}

	while (JoyD) {
		process_events();
	}

	return(selectedEntry);
}
