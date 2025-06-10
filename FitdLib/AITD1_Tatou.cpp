#include "common.h"

#include "AITD1.h"

/// @brief 
/// @todo What's with the magic number `45120`? 
void clearScreenTatou(void)
{
	for (int i = 0; i < 45120; i++) { frontBuffer[i] = 0; }
}

/// @brief Handles the 3D armadillo from the intro to AITD1.
/// @param  
/// @return 1 if the sequence was canceled, otherwise 0.
int make3dTatou(void)
{
/// @brief The time before the 3d spinning armadillo pops up in AITD1's intro.
#define AITD1_TIME_BEFORE_3D_TATOU 180
	unsigned char paletteBackup[BYTES_IN_PALETTE];
	unsigned int localChrono;
	
	char* tatou2d = CheckLoadMallocPak("ITD_RESS", RESS1_TATOU_MCG);
	char* tatou3d = CheckLoadMallocPak("ITD_RESS", RESS1_TATOU_3DO);
	unsigned char* tatouPal = (unsigned char*)CheckLoadMallocPak("ITD_RESS", RESS1_TATOU_PAL);
	
	int time = 8920;
	int deltaTime = 50;
	int rotation = 256;
	int unk1 = 8;

	setupCameraProjection(160, 100, 128, 500, 490);

	copyPalette(currentGamePalette, paletteBackup);

	paletteFill(currentGamePalette, 0, 0, 0);

	setPalette(currentGamePalette);

	copyPalette(tatouPal, currentGamePalette);
	FastCopyScreen(tatou2d + 770, frontBuffer);
	FastCopyScreen(frontBuffer, aux2);

	osystem_CopyBlockPhys(frontBuffer, 0, 0, _SCREEN_INTERNAL_WIDTH, _SCREEN_INTERNAL_HEIGHT);

	FadeInPhys(8, 0);

	startChrono(&localChrono);

	do {
		process_events();

		//timeGlobal++;
		timer = timeGlobal;

		// avant eclair (before lightning)
		if (evalChrono(&localChrono) <= AITD1_TIME_BEFORE_3D_TATOU) {
			if (key || Click || JoyD) {
				break;
			}
		} else { // eclair (lightning)
			/* LastSample = LastPriority = -1; */

			playSound(CVars[getCVarsIdx(SAMPLE_TONNERRE)]);

			/* LastSample = LastPriority = -1; */

			paletteFill(currentGamePalette, 63, 63, 63);
			setPalette(currentGamePalette);
			/* setClipSize(0,0,_SCREEN_INTERNAL_WIDTH - 1,_SCREEN_INTERNAL_HEIGHT - 1); */

			clearScreenTatou();

			setCameraTarget(0, 0, 0, unk1, rotation, 0, time);

			AffObjet(0, 0, 0, 0, 0, 0, tatou3d);

			//blitScreenTatou();
			osystem_CopyBlockPhys((unsigned char*)frontBuffer, 0, 0, _SCREEN_INTERNAL_WIDTH, _SCREEN_INTERNAL_HEIGHT);

			process_events();

			copyPalette(tatouPal, currentGamePalette);
			setPalette(currentGamePalette);
			osystem_CopyBlockPhys((unsigned char*)frontBuffer, 0, 0, _SCREEN_INTERNAL_WIDTH, _SCREEN_INTERNAL_HEIGHT);

			// boucle de rotation du tatou (armadillo rotation loop)
			while (key == 0 && Click == 0 && JoyD == 0) {
				process_events();

				time += deltaTime - 25;

				if (time > 16000)
					break;

				rotation -= 8;

				clearScreenTatou();

				setCameraTarget(0, 0, 0, unk1, rotation, 0, time);

				AffObjet(0, 0, 0, 0, 0, 0, tatou3d);

				//blitScreenTatou();

				osystem_stopFrame();
			}

			break;
		}
	} while (1);

	free(tatouPal);
	free(tatou3d);
	free(tatou2d);

	// If there was an input event...
	if (key || Click || JoyD) {
		// ...handle it and return 1 to skip the title sequence.
		while (key) { process_events(); }

		FadeOutPhys(32, 0);
		copyPalette((unsigned char*)paletteBackup, currentGamePalette);
		return(true);
	} else {
		// ...otherwise, fade out slower & don't skip it.
		FadeOutPhys(16, 0);
		copyPalette((unsigned char*)paletteBackup, currentGamePalette);
		return(false);
	}

	return(false); // TODO: Redundant return?
#undef AITD1_TIME_BEFORE_3D_TATOU
}
