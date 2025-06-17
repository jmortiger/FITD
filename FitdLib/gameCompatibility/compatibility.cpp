#include "common.h"

/// @brief Fills the given character buffer with the correct name of the
/// requested stage/floor file for this version of the current game (correctly
/// handles Steam AITD3).
/// @param buffer A currently empty string buffer.
/// @param floorNumber 
/// @details The Steam version of AITD3 doesn't use `SAL` files; it seems to use
/// `ETAGE` files like the other games.
void getStageFile(char* buffer, int floorNumber)
{
	static char stageFilePrefix[6] = "";
	if (stageFilePrefix[0] == '\000') {
		char _temp[14] = "";
		if (g_gameId == AITD3) {
			sprintf(_temp, "SAL%02d.PAK", floorNumber);
			if (fileExists(_temp)) strcpy(stageFilePrefix, "SAL");
			else { // In case the floor was invalid & not the prefix, don't assign unless file is found.
				_temp[0] = '\000';
				// strcpy(_temp, "ETAGE");
				sprintf(_temp, "ETAGE%02d.PAK", floorNumber);
				if (fileExists(_temp)) strcpy(stageFilePrefix, "ETAGE");
				else DebugPrintfLn(DBO_L_WARN, "No file found for stage/floor #%02d", floorNumber);
			}
		} else {
			strcpy(stageFilePrefix, "ETAGE");
		}
	}
	sprintf(buffer, "%s%02d", stageFilePrefix, floorNumber);
}
int getGameRessIndex(RessType name)
{
// #define insertNum(num, name) RESS#num##_#name##
#define __insertName(macro) macro
#define _insertName(macro) __insertName(macro)
#define insertName(name) g_gameId == AITD1 ? (_insertName(RESS1_## name)) : g_gameId == AITD2 ? (_insertName(RESS2_## name)) : g_gameId == AITD3 ? (_insertName(RESS3_## name)) : -1
#define insertNameJack(name) g_gameId == AITD1 ? (_insertName(RESS1_## name)) : g_gameId == AITD2 ? (_insertName(RESS2_## name)) : g_gameId == AITD3 ? (_insertName(RESS3_## name)) : g_gameId == JACK ? (_insertName(JACK_## name)) : -1
#define insertNameAll(name) g_gameId == AITD1 ? (_insertName(RESS1_## name)) : g_gameId == AITD2 ? (_insertName(RESS2_## name)) : g_gameId == AITD3 ? (_insertName(RESS3_## name)) : g_gameId == JACK ? (_insertName(JACK_## name)) : g_gameId == TIMEGATE ? (_insertName(GATE_## name)) : -1
	switch (name) {
		case CADRE_SPF: return insertNameJack(CADRE_SPF);
		case ITDFONT: return insertNameAll(ITDFONT);
		case LETTRE: return insertName(LETTRE);
		case LIVRE: return insertNameJack(LIVRE);
		case CARNET: return insertName(CARNET);
		case INVENTAIRE_OPT0: return RESS2_INVENTAIRE_OPT0; // The same in AITD2 & 3
		case INVENTAIRE_OPT1: return RESS2_INVENTAIRE_OPT1; // The same in AITD2 & 3
		case INVENTAIRE_OPT2: return RESS2_INVENTAIRE_OPT2; // The same in AITD2 & 3
		case OPTION_SCREEN: return RESS2_OPTION_SCREEN; // The same in AITD2 & 3
		case TATOU_3DO: return RESS1_TATOU_3DO;
		case TATOU_PAL: return RESS1_TATOU_PAL;
		case TATOU_MCG: return RESS1_TATOU_MCG;
		case PALETTE_JEU: return RESS1_PALETTE_JEU;
		case TEXT_GRAPH: return RESS1_TEXT_GRAPH;
		case PERSO_CHOICE: return RESS1_PERSO_CHOICE;
		case GRENOUILLE: return RESS1_GRENOUILLE;
		case DEAD_END: return RESS1_DEAD_END;
		case TITRE: return RESS1_TITRE;
		case FOND_INTRO: return RESS1_FOND_INTRO;
		case CAM07000: return RESS1_CAM07000;
		case CAM07001: return RESS1_CAM07001;
		case CAM06000: return RESS1_CAM06000;
		case CAM06005: return RESS1_CAM06005;
		case CAM06008: return RESS1_CAM06008;
		case IM_EXT_JACK: return JACK_IM_EXT_JACK;
		case CYM11001: return RESS2_CYM11001;
		case CYM11011: return RESS2_CYM11011;
		case CYM11012: return RESS2_CYM11012;
		case CYM11013: return RESS2_CYM11013;
		case PRO07011: return RESS2_PRO07011;
		case PRO07012: return RESS2_PRO07012;
		case PRO07013: return RESS2_PRO07013;
		case PRO08001: return RESS2_PRO08001;
		case PRO08008: return RESS2_PRO08008;
		case SPRITES_INVENTAIRE: return RESS2_SPRITES_INVENTAIRE;
		case CYM00001: return RESS3_CYM00001;
		case CYM00007: return RESS3_CYM00007;
		case CYM00013: return RESS3_CYM00013;
		case CYM00014: return RESS3_CYM00014;
		case CYM01017: return RESS3_CYM01017;
		case CYM03011: return RESS3_CYM03011;
		case CYM13005: return RESS3_CYM13005;
		case CYM13013: return RESS3_CYM13013;
		case MENU3: return RESS3_MENU3;
		case SPRITES_INVENTAIRE_CAVERNE: return RESS3_SPRITES_INVENTAIRE_CAVERNE;
		case SPRITES_INVENTAIRE_COWBOY: return RESS3_SPRITES_INVENTAIRE_COWBOY;
		case SPRITES_INVENTAIRE_COUGUAR: return RESS3_SPRITES_INVENTAIRE_COUGUAR;

		default: return -1; // TODO: Throw
	}

// #undef insertNum
#undef __insertName
#undef _insertName
#undef insertName
#undef insertNameJack
}
