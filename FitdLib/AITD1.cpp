#include "common.h"

// DEMO mapping
/*
#define PALETTE_JEU		0
#define ITDFONT			1
*/

int AITD1KnownCVars[] =
{
	SAMPLE_PAGE,
	BODY_FLAMME,
	MAX_WEIGHT_LOADABLE,
	TEXTE_CREDITS,
	SAMPLE_TONNERRE,
	INTRO_DETECTIVE,
	INTRO_HERITIERE,
	WORLD_NUM_PERSO,
	CHOOSE_PERSO,
	SAMPLE_CHOC,
	SAMPLE_PLOUF,
	REVERSE_OBJECT,
	KILLED_SORCERER,
	LIGHT_OBJECT,
	FOG_FLAG,
	DEAD_PERSO,
	-1
};

enumLifeMacro AITD1LifeMacroTable[] =
{
	LM_DO_MOVE,
	LM_ANIM_ONCE,
	LM_ANIM_ALL_ONCE,
	LM_BODY,
	LM_IF_EGAL, // If Equal
	LM_IF_DIFFERENT,
	LM_IF_SUP_EGAL,
	LM_IF_SUP,
	LM_IF_INF_EGAL,
	LM_IF_INF,
	LM_GOTO,
	LM_RETURN,
	LM_END,
	LM_ANIM_REPEAT,
	LM_ANIM_MOVE,
	LM_MOVE,
	LM_HIT,
	LM_MESSAGE,
	LM_MESSAGE_VALUE,
	LM_VAR,
	LM_INC,
	LM_DEC,
	LM_ADD,
	LM_SUB,
	LM_LIFE_MODE,
	LM_SWITCH,
	LM_CASE,
	LM_CAMERA,
	LM_START_CHRONO,
	LM_MULTI_CASE,
	LM_FOUND,
	LM_LIFE,
	LM_DELETE,
	LM_TAKE,
	LM_IN_HAND,
	LM_READ,
	LM_ANIM_SAMPLE,
	LM_SPECIAL,
	LM_DO_REAL_ZV,
	LM_SAMPLE,
	LM_TYPE,
	LM_GAME_OVER,
	LM_MANUAL_ROT,
	LM_RND_FREQ,
	LM_MUSIC,
	LM_SET_BETA,
	LM_DO_ROT_ZV,
	LM_STAGE,
	LM_FOUND_NAME,
	LM_FOUND_FLAG,
	LM_FOUND_LIFE,
	LM_CAMERA_TARGET,
	LM_DROP,
	LM_FIRE,
	LM_TEST_COL,
	LM_FOUND_BODY,
	LM_SET_ALPHA,
	LM_STOP_BETA,
	LM_DO_MAX_ZV,
	LM_PUT,
	LM_C_VAR,
	LM_DO_NORMAL_ZV,
	LM_DO_CARRE_ZV, // Square
	LM_SAMPLE_THEN,
	LM_LIGHT,
	LM_SHAKING,
	LM_INVENTORY,
	LM_FOUND_WEIGHT,
	LM_UP_COOR_Y,
	LM_SPEED,
	LM_PUT_AT,
	LM_DEF_ZV,
	LM_HIT_OBJECT,
	LM_GET_HARD_CLIP,
	LM_ANGLE,
	LM_REP_SAMPLE,
	LM_THROW,
	LM_WATER,
	LM_PICTURE,
	LM_STOP_SAMPLE,
	LM_NEXT_MUSIC,
	LM_FADE_MUSIC,
	LM_STOP_HIT_OBJECT,
	LM_COPY_ANGLE,
	LM_END_SEQUENCE,
	LM_SAMPLE_THEN_REPEAT,
	LM_WAIT_GAME_OVER,
};

int makeIntroScreens(void)
{
	char* data;
	unsigned int chrono;

	data = loadPak("ITD_RESS", AITD1_TITRE);
	FastCopyScreen(data + 770, frontBuffer);
	osystem_CopyBlockPhys(frontBuffer, 0, 0, _SCREEN_INTERNAL_WIDTH, _SCREEN_INTERNAL_HEIGHT);
	FadeInPhys(8, 0);
	memcpy(logicalScreen, frontBuffer, _SCREEN_INTERNAL_WIDTH * _SCREEN_INTERNAL_HEIGHT);
	osystem_flip(NULL);
	free(data);
	loadPakTo("ITD_RESS", AITD1_LIVRE, aux);
	startChrono(&chrono);

	osystem_drawBackground();

	do {
		int time;

		process_events();

		time = evalChrono(&chrono);

		if (time >= 0x30)
			break;

	} while (key == 0 && Click == 0);

	playSound(CVars[getCVarsIdx(SAMPLE_PAGE)]);
	/*  LastSample = -1;
	LastPriority = -1;
	LastSample = -1;
	LastPriority = 0; */
	turnPageFlag = 1;
	Lire(CVars[getCVarsIdx(TEXTE_CREDITS)] + 1, 48, 2, 260, 197, 1, 26, 0);

	return(0);
}

void CopyBox_Aux_Log(int x1, int y1, int x2, int y2)
{
	int i;
	int j;

	for (i = y1; i < y2; i++) {
		for (j = x1; j < x2; j++) {
			*(screenSm3 + i * _SCREEN_INTERNAL_WIDTH + j) = *(screenSm1 + i * _SCREEN_INTERNAL_WIDTH + j);
		}
	}
}

int ChoosePerso(void)
{
	int choice = 0;
	int firsttime = 1;
	int choiceMade = 0;

	InitCopyBox(aux, logicalScreen);

	while (choiceMade == 0) {
		process_events();
		osystem_drawBackground();

		// TODO: missing code for music stop

		loadPakTo("ITD_RESS", 10, aux);
		FastCopyScreen(aux, logicalScreen);
		FastCopyScreen(logicalScreen, aux2);

		if (choice == 0) {
			// Draw a framed box taking half the width & the whole height
			AffBigCadre(80, 100, (_SCREEN_INTERNAL_WIDTH / 2), _SCREEN_INTERNAL_HEIGHT);
			CopyBox_Aux_Log(10, 10, 149, 190);
		} else {
			// Draw a framed box taking half the width & the whole height
			AffBigCadre(240, 100, 160, _SCREEN_INTERNAL_HEIGHT);
			CopyBox_Aux_Log(170, 10, 309, 190);
		}

		FastCopyScreen(logicalScreen, frontBuffer);
		osystem_CopyBlockPhys(frontBuffer, 0, 0, _SCREEN_INTERNAL_WIDTH, _SCREEN_INTERNAL_HEIGHT);

		if (firsttime != 0) {
			FadeInPhys(0x40, 0);

			do {
				process_events();
			} while (Click || key);

			firsttime = 0;
		}

		// process input
		while ((localKey = key) != 28 && Click == 0) {
			process_events();
			osystem_drawBackground();

			// left
			if (JoyD & 4) {
				choice = 0;
				FastCopyScreen(aux2, logicalScreen);
				// Draw a framed box taking half the width & the whole height
				AffBigCadre(80, 100, (_SCREEN_INTERNAL_WIDTH / 2), _SCREEN_INTERNAL_HEIGHT);
				CopyBox_Aux_Log(10, 10, 149, 190);
				osystem_CopyBlockPhys((unsigned char*)logicalScreen, 0, 0, _SCREEN_INTERNAL_WIDTH, _SCREEN_INTERNAL_HEIGHT);

				while (JoyD != 0) {
					process_events();
				}
			}

			// right
			if (JoyD & 8) {
				choice = 1;
				FastCopyScreen(aux2, logicalScreen);
				// Draw a framed box taking half the width & the whole height
				AffBigCadre(240, 100, (_SCREEN_INTERNAL_WIDTH / 2), _SCREEN_INTERNAL_HEIGHT);
				CopyBox_Aux_Log(170, 10, 309, 190);
				osystem_CopyBlockPhys((unsigned char*)logicalScreen, 0, 0, _SCREEN_INTERNAL_WIDTH, _SCREEN_INTERNAL_HEIGHT);

				while (JoyD != 0) {
					process_events();
				}
			}

			if (localKey == 1) {
				InitCopyBox(aux2, logicalScreen);
				FadeOutPhys(0x40, 0);
				return(-1);
			}
		}

		FadeOutPhys(0x40, 0);
		turnPageFlag = 0;

		switch (choice) {
			case 0:
			{
				FastCopyScreen(frontBuffer, logicalScreen);
				SetClip(0, 0, 319, 199);
				loadPakTo("ITD_RESS", AITD1_FOND_INTRO, aux);
				CopyBox_Aux_Log(160, 0, 319, 199);
				FastCopyScreen(logicalScreen, aux);
				Lire(CVars[getCVarsIdx(INTRO_HERITIERE)] + 1, 165, 5, 314, 194, 2, 15, 0);
				CVars[getCVarsIdx(CHOOSE_PERSO)] = 1;
				break;
			}
			case 1:
			{
				FastCopyScreen(frontBuffer, logicalScreen);
				SetClip(0, 0, 319, 199);
				loadPakTo("ITD_RESS", AITD1_FOND_INTRO, aux);
				CopyBox_Aux_Log(0, 0, 159, 199);
				FastCopyScreen(logicalScreen, aux);
				Lire(CVars[getCVarsIdx(INTRO_DETECTIVE)] + 1, 5, 5, 154, 194, 2, 15, 0);
				CVars[getCVarsIdx(CHOOSE_PERSO)] = 0;
				break;
			}
		}

		if (localKey && 0x1C) {
			choiceMade = 1;
		}

	}

	FadeOutPhys(64, 0);
	InitCopyBox(aux2, logicalScreen);
	return(choice);
}

void startAITD1()
{
	fontHeight = 16;
	g_gameUseCDA = true;
	setPalette(currentGamePalette);

#ifndef AITD_UE4
	if (!make3dTatou()) {
		makeIntroScreens();
	}
#endif

	while (1) {
#ifndef AITD_UE4
		int startupMenuResult = processStartupMenu();
#else
		int startupMenuResult = 0;
#endif
		switch (startupMenuResult) {
			case -1: // timeout
			{
				CVars[getCVarsIdx(CHOOSE_PERSO)] = rand() & 1;
				startGame(7, 1, 0);

				if (!make3dTatou()) {
					if (!makeIntroScreens()) {
						//makeSlideshow();
					}
				}

				break;
			}
			case 0: // new game
			{
				// here, original would ask for protection

#if !TARGET_OS_IOS && !AITD_UE4
				if (ChoosePerso() != -1)
#endif
				{
					process_events();
					while (key) {
						process_events();
					}

#if !TARGET_OS_IOS
					startGame(7, 1, 0);
#endif

					// here, original would quit if protection flag was false

					startGame(0, 0, 1);
				}

				break;
			}
			case 1: // continue
			{
				// here, original would ask for protection

				if (restoreSave(12, 0)) {
					// here, original would quit if protection flag was false

					// updateShaking();

					flagInitView = 2;

					setupCamera();

					mainLoop(1, 1);

					// freeScene();

					FadeOutPhys(8, 0);
				}

				break;
			}
			case 2: // exit
			{
				freeAll();
				exit(-1);

				break;
			}
		}
	}
}

void AITD1_ReadBook(int index, int type)
{
	switch (type) {
		case 0: // READ_MESSAGE
		{
			loadPakTo("ITD_RESS", AITD1_LETTRE, aux);
			turnPageFlag = 0;
			Lire(index, 60, 10, 245, 190, 0, 26, 0);
			break;
		}
		case 1: // READ_BOOK
		{
			loadPakTo("ITD_RESS", AITD1_LIVRE, aux);
			turnPageFlag = 1;
			Lire(index, 48, 2, 260, 197, 0, 26, 0);
			break;
		}
		case 2: // READ_CARNET
		{
			loadPakTo("ITD_RESS", AITD1_CARNET, aux);
			turnPageFlag = 0;
			Lire(index, 50, 20, 250, 199, 0, 26, 0);
			break;
		}
		default:
			FITD_throwFatal(); // assert(0);
	}
}

#ifdef _DEBUGGER_H_ // Vars table
char _lbl_var0[]	= "flag_playerHasControl"; // initVal = 1
char _lbl_var1[]	= "flag_playerPushing"; // initVal = 0
char _lbl_var2[]	= "atticHatchAttempts"; // initVal = 0
char _lbl_var3[]	= "flag_atticHatchOpen"; // initVal = 0
char _lbl_var4[]	= "SFX_step1"; // initVal = 30
char _lbl_var5[]	= "SFX_step2"; // initVal = 31
char _lbl_var6[]	= "flag_lampFueled"; // initVal = 0
char _lbl_var7[]	= "flag_lampLit"; // initVal = 0
char _lbl_var8[]	= "lampOilLevel"; // initVal = 0
char _lbl_var9[]	= "droppingObjIdx"; // initVal = 0
char _lbl_var10[]	= "flag_throwingOrJug"; // initVal = 0
char _lbl_var11[]	= "flag_usedOilCanE3"; // initVal = 0
char _lbl_var12[]	= "flag_usedOilCanE1"; // initVal = 0
char _lbl_var13[]	= "rifleShots"; // initVal = 4
char _lbl_var14[]	= "flag_rifleReady"; // initVal = 1
char _lbl_var15[]	= "SFX_rifleShot"; // initVal = 7
char _lbl_var16[]	= "flag_canPlaceRug"; // initVal = 0
char _lbl_var17[]	= "flag_rugPlaced"; // initVal = 0
char _lbl_var18[]	= "zombieHealthE0R0"; // initVal = 10
char _lbl_var19[]	= "flag_isAtticBirdDead"; // initVal = 0
char _lbl_var20[]	= "activeMonsters"; // initVal = 0
char _lbl_var21[]	= "playerHealth"; // initVal = 20
char _lbl_var22[]	= "birdHealthE0R0"; // initVal = 10
char _lbl_var23[]	= "flag_isDoorOpenE1R0_1"; // initVal = 0
char _lbl_var24[]	= "flag_isPlayerBackingUp"; // initVal = 0
char _lbl_var25[]	= "flag_isInIntro"; // initVal = 1
char _lbl_var26[]	= "arrow pointer"; // initVal = 0
char _lbl_var27[]	= "flag_E1R1-R3 door open"; // initVal = 0
char _lbl_var28[]	= "flag_inDanger"; // initVal = 0
char _lbl_var29[]	= "deathType"; // initVal = 0
char _lbl_var30[]	= "flag_E1R1-E2 door open"; // initVal = 0
char _lbl_var31[]	= "flag_E1R2 zombie spawned"; // initVal = 0
char _lbl_var32[]	= "flag_saber chest unlocked"; // initVal = 0
char _lbl_var33[]	= "flag_Pregzt dead"; // initVal = 0
char _lbl_var34[]	= "saber blade in place"; // initVal = 0
char _lbl_var35[]	= "flag_player standing in trigger 0 in E3R11"; // initVal = 0
char _lbl_var36[]	= "saber handle in place"; // initVal = 0
char _lbl_var37[]	= "saber durability"; // initVal = 5
char _lbl_var38[]	= "flag_saber broken"; // initVal = 0
char _lbl_var39[]	= "saber damage"; // initVal = 4
char _lbl_var40[]	= "E1R2 zombie hitpoints"; // initVal = 10
char _lbl_var41[]	= "searching cabinet in E1R3"; // initVal = 0
char _lbl_var42[]	= "opening cabinet in E1R3"; // initVal = 0
char _lbl_var43[]	= "flag_E1R1-R4 door open"; // initVal = 0
char _lbl_var44[]	= "dresser unlocked in E1R4"; // initVal = 0
char _lbl_var45[]	= "flag_E1R4 bird alerted"; // initVal = 0
char _lbl_var46[]	= "flag_vase thrown"; // initVal = 0
char _lbl_var47[]	= "flag_brokenMirror1"; // initVal = 0
char _lbl_var48[]	= "flag_canPlaceMirror1"; // initVal = 0
char _lbl_var49[]	= "flag_placedMirror1"; // initVal = 0
char _lbl_var50[]	= "flag_unbroken mirrors placed counter"; // initVal = 0
char _lbl_var51[]	= "flag_canPlaceMirror2"; // initVal = 0
char _lbl_var52[]	= "flag_placedMirror2"; // initVal = 0
char _lbl_var53[]	= "flag_brokenMirror2 "; // initVal = 0
char _lbl_var54[]	= "E1R4 bird hitpoints"; // initVal = 10
char _lbl_var55[]	= "E1R1-R5 door open"; // initVal = 0
char _lbl_var56[]	= "E1R0 zombie spawned"; // initVal = 0
char _lbl_var57[]	= "E1R0 zombie hitpoints"; // initVal = 10
char _lbl_var58[]	= "E1R1-R7 door open"; // initVal = 0
char _lbl_var59[]	= "nightgaunts left"; // initVal = 2
char _lbl_var60[]	= "scary sounds played in E1R7"; // initVal = 0
char _lbl_var61[]	= "north nightgaunt aggroed"; // initVal = 0
char _lbl_var62[]	= "south nightgaunt aggroed"; // initVal = 0
char _lbl_var63[]	= "E2R0-R2 S door open"; // initVal = 1
char _lbl_var64[]	= "E2R0 west doors unlocked"; // initVal = 0
char _lbl_var65[]	= "E2R0-R2 N door open"; // initVal = 1
char _lbl_var66[]	= "E2R0-R1 N door open"; // initVal = 0
char _lbl_var67[]	= "E2R0-R1 S door open"; // initVal = 0
char _lbl_var68[]	= "canPlaceLibraryBook"; // initVal = 0
char _lbl_var69[]	= "light source in E2R0"; // initVal = 0
char _lbl_var70[]	= "area lit (E2R0)"; // initVal = 1
char _lbl_var71[]	= "flag_unused_vagabondActive"; // initVal = 0
char _lbl_var72[]	= "playing scary sounds when entering E2R0 from E2R8"; // initVal = 0
char _lbl_var73[]	= "flag_isProtectedFromBooks"; // initVal = 0
char _lbl_var74[]	= "ready to spawn axe"; // initVal = 1
char _lbl_var75[]	= "ready to spawn arrow"; // initVal = 1
char _lbl_var76[]	= "flag_Indian portrait dead"; // initVal = 0
char _lbl_var77[]	= "flag_E2R3-R4 door open"; // initVal = 0
char _lbl_var78[]	= "flag_E2R3 (and E5R4) birds are both alive"; // initVal = 1
char _lbl_var79[]	= "flag_E2R3 bird spawned"; // initVal = 0
char _lbl_var80[]	= "E2R3 bird hitpoints"; // initVal = 10
char _lbl_var81[]	= "flag_E2R2-R4 door open"; // initVal = 0
char _lbl_var82[]	= "flag_E2R4-R1 door open"; // initVal = 0
char _lbl_var83[]	= "flag_player in E2R6"; // initVal = 0
char _lbl_var84[]	= "flag_player in E2R5"; // initVal = 0
char _lbl_var85[]	= "flag_area lit (E2R6)"; // initVal = 1
char _lbl_var86[]	= "light source in E2R6"; // initVal = 0
char _lbl_var87[]	= "flag_E2R4-R5 door open"; // initVal = 0
char _lbl_var88[]	= "flag_jug filled"; // initVal = 0
char _lbl_var89[]	= "flag_canExtinguishCigars"; // initVal = 0
char _lbl_var90[]	= "playerCurrentAction"; // initVal = 16
char _lbl_var91[]	= "flag_E2R4-R6 door open"; // initVal = 0
char _lbl_var92[]	= "bullets in revolver"; // initVal = 6
char _lbl_var93[]	= "flag_E2R1-R7 door open"; // initVal = 0
char _lbl_var94[]	= "flag_canPlaceTalisman"; // initVal = 0
char _lbl_var95[]	= "flag_talismanPlaced"; // initVal = 0
char _lbl_var96[]	= "flag_E2R2-R10 door open"; // initVal = 0
char _lbl_var97[]	= "record in gramophone"; // initVal = 0
char _lbl_var98[]	= "flag_E2R10 ghost aggroed"; // initVal = 0
char _lbl_var99[]	= "flag_N front door open"; // initVal = 0
char _lbl_var100[]	= "flag_S front door open"; // initVal = 0
char _lbl_var101[]	= "flag_jellyReady"; // initVal = 0
char _lbl_var102[]	= "flag_E3R1-R13 E door open"; // initVal = 0
char _lbl_var103[]	= "doors between E3R1 and E3R13 have been unlocked"; // initVal = 0
char _lbl_var104[]	= "E3R1-R13 W door open"; // initVal = 0
char _lbl_var105[]	= "flag_unusedJellyShouldAttack"; // initVal = 1
char _lbl_var106[]	= "E3R1-R2 E door open"; // initVal = 0
char _lbl_var107[]	= "E3R1-R2 W door open"; // initVal = 0
char _lbl_var108[]	= "E3R2-R3 S door open"; // initVal = 0
char _lbl_var109[]	= "doors between E3R2 and E3R3 unlocked"; // initVal = 0
char _lbl_var110[]	= "E3R2-R3 N door open"; // initVal = 0
char _lbl_var111[]	= "player in E3R3"; // initVal = 0
char _lbl_var112[]	= "dancer 1 transformed into vortex (1) or dead (2)"; // initVal = 0
char _lbl_var113[]	= "dancer 2 transformed into vortex (1) or dead (2)"; // initVal = 0
char _lbl_var114[]	= "dancer 3 transformed into vortex (1) or dead (2)"; // initVal = 0
char _lbl_var115[]	= "pirate hitpoints"; // initVal = 15
char _lbl_var116[]	= "flag_E3R3-R4 S door open"; // initVal = 0
char _lbl_var117[]	= "flag_E3R3-R4 N door open"; // initVal = 0
char _lbl_var118[]	= "dancer 1 transformed into vortex"; // initVal = 0
char _lbl_var119[]	= "dancer 2 transformed into vortex"; // initVal = 0
char _lbl_var120[]	= "dancer 3 transformed into vortex"; // initVal = 0
char _lbl_var121[]	= "flag_E3R4-R5 door open"; // initVal = 0
char _lbl_var122[]	= "flag_spidersReleased"; // initVal = 0
char _lbl_var123[]	= "flag_statueSearched"; // initVal = 0
char _lbl_var124[]	= "isCellarDoorOpen"; // initVal = 0
char _lbl_var125[]	= "isCellarUnlocked"; // initVal = 0
char _lbl_var126[]	= "E3R5-R8 door open"; // initVal = 0
char _lbl_var127[]	= "E3R7-R8 door open"; // initVal = 0
char _lbl_var128[]	= "E3R8-R10 door open"; // initVal = 0
char _lbl_var129[]	= "player standing in trigger 0 in E3R12"; // initVal = 0
char _lbl_var130[]	= "E3R12 solved"; // initVal = 0
char _lbl_var131[]	= "zombie 1 in E3R12 hitpoints"; // initVal = 8
char _lbl_var132[]	= "player standing in trigger 1 in E3R12"; // initVal = 0
char _lbl_var133[]	= "E3R8-R9 door open"; // initVal = 0
char _lbl_var134[]	= "E3R10 zombie spawned"; // initVal = 0
char _lbl_var135[]	= "revolverReady"; // initVal = 1
char _lbl_var136[]	= "E3R10 zombie hitpoints"; // initVal = 8
char _lbl_var137[]	= "flag_E3R11-R13 W door open"; // initVal = 0
char _lbl_var138[]	= "flag_E3R11 doors unlocked"; // initVal = 0
char _lbl_var139[]	= "flag_E3R11-R13 E door open"; // initVal = 0
char _lbl_var140[]	= "flag_E3R0-R12 W door open"; // initVal = 0
char _lbl_var141[]	= "flag_E3R0-R12 E door open"; // initVal = 0
char _lbl_var142[]	= "flag_E3R9-R12 door open"; // initVal = 0
char _lbl_var143[]	= "zombie 2 in E3R12 hitpoints"; // initVal = 8
char _lbl_var144[]	= "flag_zombie 2 in E3R12 dead"; // initVal = 0
char _lbl_var145[]	= "flag_zombie 2 in E3R12 stood up"; // initVal = 0
char _lbl_var146[]	= "next active zombie in E3R12"; // initVal = 0
char _lbl_var147[]	= "flag_there are zombies active in E3R12"; // initVal = 0
char _lbl_var148[]	= "zombie 3 in E3R12 hitpoints"; // initVal = 8
char _lbl_var149[]	= "flag_zombie 3 in E3R12 dead"; // initVal = 0
char _lbl_var150[]	= "flag_zombie 3 in E3R12 stood up"; // initVal = 0
char _lbl_var151[]	= "zombie 4 in E3R12 hitpoints"; // initVal = 8
char _lbl_var152[]	= "flag_zombie 4 in E3R12 dead"; // initVal = 0
char _lbl_var153[]	= "flag_zombie 4 in E3R12 stood up"; // initVal = 0
char _lbl_var154[]	= "zombie 5 in E3R12 hitpoints"; // initVal = 8
char _lbl_var155[]	= "flag_zombie 5 in E3R12 dead"; // initVal = 0
char _lbl_var156[]	= "flag_zombie 5 in E3R12 stood up"; // initVal = 0
char _lbl_var157[]	= "flag_doors in E3R12 have shut"; // initVal = 0
char _lbl_var158[]	= "flag_zombie 1 in E3R12 aggroed"; // initVal = 0
char _lbl_var159[]	= "flag_E3R12-R13 door open"; // initVal = 0
char _lbl_var160[]	= "flag_bridgeSegmentTouched1r0"; // initVal = 0
char _lbl_var161[]	= "flag_bridgeSegmentTouched2r0"; // initVal = 0
char _lbl_var162[]	= "flag_bridgeSegmentTouched3r0"; // initVal = 0
char _lbl_var163[]	= "flag_bridgeSegmentTouched4r0"; // initVal = 0
char _lbl_var164[]	= "flag_bridgeSegmentTouched5r0"; // initVal = 0
char _lbl_var165[]	= "flag_bridgeSegmentTouched6r0"; // initVal = 0
char _lbl_var166[]	= "flag_bridgeSegmentTouched7r0"; // initVal = 0
char _lbl_var167[]	= "flag_bridgeSegmentTouched8r0"; // initVal = 0
char _lbl_var168[]	= "flag_isPlayerOverBridgePit"; // initVal = 0
char _lbl_var169[]	= "E5 step sound switch"; // initVal = 0
char _lbl_var170[]	= "Cthonian state"; // initVal = 0
char _lbl_var171[]	= "Cthonian has stopped past the boulders"; // initVal = 0
char _lbl_var172[]	= "Cthonian has turned around the corner"; // initVal = 0
char _lbl_var173[]	= "E5R4 bird hitpoints"; // initVal = 10
char _lbl_var174[]	= "E5 Deep One should be spawned"; // initVal = 1
char _lbl_var175[]	= "E5 Deep One hitpoints"; // initVal = 10
char _lbl_var176[]	= "E5 Deep One should be despawned"; // initVal = 0
char _lbl_var177[]	= "E5R8 wasp hitpoints"; // initVal = 10
char _lbl_var178[]	= "not in the dark in E5-E6"; // initVal = 1
char _lbl_var179[]	= "beetle hitpoints"; // initVal = 10
char _lbl_var180[]	= "flag_bridgeSegmentTouched1r10"; // initVal = 0
char _lbl_var181[]	= "flag_bridgeSegmentTouched2r10"; // initVal = 0
char _lbl_var182[]	= "flag_bridgeSegmentTouched3r10"; // initVal = 0
char _lbl_var183[]	= "flag_bridgeSegmentTouched4r10"; // initVal = 0
char _lbl_var184[]	= "flag_bridgeSegmentTouched5r10"; // initVal = 0
char _lbl_var185[]	= "flag_bridgeSegmentTouched6r10"; // initVal = 0
char _lbl_var186[]	= "flag_bridgeSegmentTouched7r10"; // initVal = 0
char _lbl_var187[]	= "flag_bridgeSegmentTouched8r10"; // initVal = 0
char _lbl_var188[]	= "flag_bridgeSegmentTouched9r10"; // initVal = 0
char _lbl_var189[]	= "flag_bridgeSegmentTouched10r10"; // initVal = 0
char _lbl_var190[]	= "flag_bridgeSegmentTouched11r10"; // initVal = 0
char _lbl_var191[]	= "flag_bridgeSegmentTouched12r10"; // initVal = 0
char _lbl_var192[]	= "flag_bridgeSegmentTouched13r10"; // initVal = 0
char _lbl_var193[]	= "E5R10 wasp hitpoints"; // initVal = 10
char _lbl_var194[]	= "E5R11 chest unlocked"; // initVal = 0
char _lbl_var195[]	= "flag_canPlaceGem"; // initVal = 0
char _lbl_var196[]	= "E6 Deep One should be spawned"; // initVal = 1
char _lbl_var197[]	= "E6 Deep One hitpoints"; // initVal = 10
char _lbl_var198[]	= "E6 Deep One should be despawned"; // initVal = 0
char _lbl_var199[]	= "E6R6 fireball switch"; // initVal = 1
char _lbl_var200[]	= "SFX_introStep1"; // initVal = 32
char _lbl_var201[]	= "SFX_introStep2"; // initVal = 33
char _lbl_var202[]	= "flag_unused1"; // initVal = 0
char _lbl_var203[]	= "flag_unused2"; // initVal = 0
char _lbl_var204[]	= "temporary (used by LIFE 59 for returning previous BODY after drinking flask)"; // initVal = 0
char _lbl_var205[]	= "flag_unusedIsUnderground"; // initVal = 0
char _lbl_var206[]	= "fallHeight"; // initVal = 0

const char* varsNameTable[] = {
	_lbl_var0,
	_lbl_var1,
	_lbl_var2,
	_lbl_var3,
	_lbl_var4,
	_lbl_var5,
	_lbl_var6,
	_lbl_var7,
	_lbl_var8,
	_lbl_var9,
	_lbl_var10,
	_lbl_var11,
	_lbl_var12,
	_lbl_var13,
	_lbl_var14,
	_lbl_var15,
	_lbl_var16,
	_lbl_var17,
	_lbl_var18,
	_lbl_var19,
	_lbl_var20,
	_lbl_var21,
	_lbl_var22,
	_lbl_var23,
	_lbl_var24,
	_lbl_var25,
	_lbl_var26,
	_lbl_var27,
	_lbl_var28,
	_lbl_var29,
	_lbl_var30,
	_lbl_var31,
	_lbl_var32,
	_lbl_var33,
	_lbl_var34,
	_lbl_var35,
	_lbl_var36,
	_lbl_var37,
	_lbl_var38,
	_lbl_var39,
	_lbl_var40,
	_lbl_var41,
	_lbl_var42,
	_lbl_var43,
	_lbl_var44,
	_lbl_var45,
	_lbl_var46,
	_lbl_var47,
	_lbl_var48,
	_lbl_var49,
	_lbl_var50,
	_lbl_var51,
	_lbl_var52,
	_lbl_var53,
	_lbl_var54,
	_lbl_var55,
	_lbl_var56,
	_lbl_var57,
	_lbl_var58,
	_lbl_var59,
	_lbl_var60,
	_lbl_var61,
	_lbl_var62,
	_lbl_var63,
	_lbl_var64,
	_lbl_var65,
	_lbl_var66,
	_lbl_var67,
	_lbl_var68,
	_lbl_var69,
	_lbl_var70,
	_lbl_var71,
	_lbl_var72,
	_lbl_var73,
	_lbl_var74,
	_lbl_var75,
	_lbl_var76,
	_lbl_var77,
	_lbl_var78,
	_lbl_var79,
	_lbl_var80,
	_lbl_var81,
	_lbl_var82,
	_lbl_var83,
	_lbl_var84,
	_lbl_var85,
	_lbl_var86,
	_lbl_var87,
	_lbl_var88,
	_lbl_var89,
	_lbl_var90,
	_lbl_var91,
	_lbl_var92,
	_lbl_var93,
	_lbl_var94,
	_lbl_var95,
	_lbl_var96,
	_lbl_var97,
	_lbl_var98,
	_lbl_var99,
	_lbl_var100,
	_lbl_var101,
	_lbl_var102,
	_lbl_var103,
	_lbl_var104,
	_lbl_var105,
	_lbl_var106,
	_lbl_var107,
	_lbl_var108,
	_lbl_var109,
	_lbl_var110,
	_lbl_var111,
	_lbl_var112,
	_lbl_var113,
	_lbl_var114,
	_lbl_var115,
	_lbl_var116,
	_lbl_var117,
	_lbl_var118,
	_lbl_var119,
	_lbl_var120,
	_lbl_var121,
	_lbl_var122,
	_lbl_var123,
	_lbl_var124,
	_lbl_var125,
	_lbl_var126,
	_lbl_var127,
	_lbl_var128,
	_lbl_var129,
	_lbl_var130,
	_lbl_var131,
	_lbl_var132,
	_lbl_var133,
	_lbl_var134,
	_lbl_var135,
	_lbl_var136,
	_lbl_var137,
	_lbl_var138,
	_lbl_var139,
	_lbl_var140,
	_lbl_var141,
	_lbl_var142,
	_lbl_var143,
	_lbl_var144,
	_lbl_var145,
	_lbl_var146,
	_lbl_var147,
	_lbl_var148,
	_lbl_var149,
	_lbl_var150,
	_lbl_var151,
	_lbl_var152,
	_lbl_var153,
	_lbl_var154,
	_lbl_var155,
	_lbl_var156,
	_lbl_var157,
	_lbl_var158,
	_lbl_var159,
	_lbl_var160,
	_lbl_var161,
	_lbl_var162,
	_lbl_var163,
	_lbl_var164,
	_lbl_var165,
	_lbl_var166,
	_lbl_var167,
	_lbl_var168,
	_lbl_var169,
	_lbl_var170,
	_lbl_var171,
	_lbl_var172,
	_lbl_var173,
	_lbl_var174,
	_lbl_var175,
	_lbl_var176,
	_lbl_var177,
	_lbl_var178,
	_lbl_var179,
	_lbl_var180,
	_lbl_var181,
	_lbl_var182,
	_lbl_var183,
	_lbl_var184,
	_lbl_var185,
	_lbl_var186,
	_lbl_var187,
	_lbl_var188,
	_lbl_var189,
	_lbl_var190,
	_lbl_var191,
	_lbl_var192,
	_lbl_var193,
	_lbl_var194,
	_lbl_var195,
	_lbl_var196,
	_lbl_var197,
	_lbl_var198,
	_lbl_var199,
	_lbl_var200,
	_lbl_var201,
	_lbl_var202,
	_lbl_var203,
	_lbl_var204,
	_lbl_var205,
	_lbl_var206,
};
#endif
