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
