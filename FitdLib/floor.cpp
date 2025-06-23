#include "common.h"

// cspell:ignore CAMSAL

u32 g_currentFloorRoomRawDataSize = 0;
u32 g_currentFloorCameraRawDataSize;
u32 g_currentFloorNumCamera = 0;
std::vector<cameraDataStruct> g_currentFloorCameraData;

/// @brief Loads all the data for the specified floor into the appropiate global variables.
/// @param floorNumber 
void loadFloor(int floorNumber)
{
	DebugPrintfLn(DBO_L_INFO, "loadFloor(%i)", floorNumber);
	DebugBeginSection(DBO_FLOOR);

	// Free preexisting cam data
	if (g_currentFloorCameraRawData) {
		DebugPrintfLn(DBO_L_INFO1, "Freeing prior camera data from address %lx & prior floor data from address %lx...", g_currentFloorCameraRawData, g_currentFloorRoomRawData);
		free(g_currentFloorCameraRawData);
		free(g_currentFloorRoomRawData);
	}

	//stopSounds();

	HQR_Reset(listBody);
	HQR_Reset(listAnim);

	g_currentFloor = floorNumber;

	char floorFileName[256];
	// Load floor data from floor file.
	// TODO: Fix AITD3 case
	// NOTE: Document why this is & add debug line. Related to `room.cpp/getNumOfRooms()`.
	// if (g_gameId < AITD3) {
	if (g_gameId <= AITD3) {
		sprintf(floorFileName, "ETAGE%02d", floorNumber);
		DebugPrintfLn(DBO_L_INFO, "Loading raw data from %s...", floorFileName);

		g_currentFloorRoomRawDataSize = getPakSize(floorFileName, 0);
		DebugPrintfLn(DBO_L_INFO1, "floorRoomRawDataSize: %u", g_currentFloorRoomRawDataSize);
		g_currentFloorCameraRawDataSize = getPakSize(floorFileName, 1);
		DebugPrintfLn(DBO_L_INFO1, "floorCameraRawDataSize: %u", g_currentFloorCameraRawDataSize);

		g_currentFloorRoomRawData = CheckLoadMallocPak(floorFileName, 0);
		g_currentFloorCameraRawData = CheckLoadMallocPak(floorFileName, 1);
	}

	currentCamera = -1;
	needChangeRoom = 1;
	changeFloor = 0;

	// #region Load rooms //////////////////////////////////

	if (roomDataTable) { // Free prior room data table
		DebugPrintfLn(DBO_L_LOG, "Freeing prior room data table from address %lx...", roomDataTable);
		free(roomDataTable);
		roomDataTable = NULL;
	}

	int expectedNumberOfRoom = getNumberOfRoom();

	int i;
	DebugPrintfLn(DBO_L_INFO2, "Loading the expected %i rooms on floor %i...", expectedNumberOfRoom, floorNumber);
	for (i = 0; i < expectedNumberOfRoom; i++) {
		DebugPrintfLn(DBO_L_LOG, "Room %i (E%iR%i):", i, floorNumber, i);
		DebugBeginSection(DBO_FLOOR);
		u32 j;

		if (roomDataTable)
			roomDataTable = (roomDataStruct*)realloc(roomDataTable, sizeof(roomDataStruct) * (i + 1));
		else
			roomDataTable = (roomDataStruct*)malloc(sizeof(roomDataStruct));

		u8* roomData;
		// TODO: Fix AITD3 case
		// if (g_gameId >= AITD3) {
		if (g_gameId > AITD3) {
			char buffer[256];

			// NOTE: The Steam version of AITD3 DOESN'T use `SAL` files; it seems to use `ETAGE` files like the other games.
			// IDEA: Use static flag instead of checking every time?
			// if (g_gameId == AITD3) {
			// 	char _temp[100] = "";
			// 	sprintf(_temp, "SAL%02d.PAK", floorNumber);
			// 	if (fileExists(_temp))
			// 		sprintf(buffer, "SAL%02d", floorNumber);
			// }
			// if (buffer[0] != 'S') sprintf(buffer, "ETAGE%02d", floorNumber);
			getStageFile(buffer, floorNumber);

			DebugPrintfLn(DBO_L_LOG, "Loading room data from file %s...", buffer);
			roomData = (u8*)CheckLoadMallocPak(buffer, i);
		} else {
			DebugPrintfLn(DBO_L_LOG, "New offset for room data: %u (Raw: %u)", READ_LE_U32(g_currentFloorRoomRawData + i * 4), *(u32*)(g_currentFloorRoomRawData + i * 4));
			roomData = (u8*)(g_currentFloorRoomRawData + READ_LE_U32(g_currentFloorRoomRawData + i * 4));
		}
		roomDataStruct* currentRoomDataPtr = &roomDataTable[i];

		u8* roomData_backup = roomData;
		DebugPrintfLn(DBO_L_LOG, "hardColOffset: %hu", READ_LE_U16(roomData));
		int hardColOffset = readS16LE(roomData);
		DebugPrintfLn(DBO_L_LOG, "sceZoneOffset: %hu", READ_LE_U16(roomData));
		int sceZoneOffset = readS16LE(roomData);

		/* DebugBPrintf(DBO_L_DEBUG, "worldX: "); DebugBPrintRaw(DBO_L_DEBUG, PF_LE_S16(roomData + 4)); DebugBFlushLn();
		DebugBPrintf(DBO_L_DEBUG, "worldY: "); DebugBPrintRaw(DBO_L_DEBUG, PF_LE_S16(roomData + 6)); DebugBFlushLn();
		DebugBPrintf(DBO_L_DEBUG, "worldZ: "); DebugBPrintRaw(DBO_L_DEBUG, PF_LE_S16(roomData + 8)); DebugBFlushLn();
		currentRoomDataPtr->worldX = READ_LE_S16(roomData + 4);
		currentRoomDataPtr->worldY = READ_LE_S16(roomData + 6);
		currentRoomDataPtr->worldZ = READ_LE_S16(roomData + 8); */
		DebugBPrintf(DBO_L_DEBUG, "worldX: "); DebugBPrintRaw(DBO_L_DEBUG, PF_LE_S16(roomData)); DebugBFlushLn();
		currentRoomDataPtr->worldX = readS16LE(roomData);
		DebugBPrintf(DBO_L_DEBUG, "worldY: "); DebugBPrintRaw(DBO_L_DEBUG, PF_LE_S16(roomData)); DebugBFlushLn();
		currentRoomDataPtr->worldY = readS16LE(roomData);
		DebugBPrintf(DBO_L_DEBUG, "worldZ: "); DebugBPrintRaw(DBO_L_DEBUG, PF_LE_S16(roomData)); DebugBFlushLn();
		currentRoomDataPtr->worldZ = readS16LE(roomData);
		// roomData = roomData_backup;

		// #region Read cameras
		/* DebugBPrintf(DBO_L_DEBUG, "numCameraInRoom: "); DebugBPrintRaw(DBO_L_DEBUG, PF_LE_U16(roomData + 0xA)); DebugBFlushLn();
		currentRoomDataPtr->numCameraInRoom = READ_LE_U16(roomData + 0xA); */
		DebugBPrintf(DBO_L_DEBUG, "numCameraInRoom: "); DebugBPrintRaw(DBO_L_DEBUG, PF_LE_U16(roomData)); DebugBFlushLn();
		currentRoomDataPtr->numCameraInRoom = readU16LE(roomData);

		// NOTE: Unlike the other 2 tables, there should always be at least 1 cam, so no safety.
		currentRoomDataPtr->cameraIdxTable = (u16*)malloc(currentRoomDataPtr->numCameraInRoom * sizeof(u16));

		DebugPrintfLnCategory(DBO_L_LOG, DBO_CAMERA, "E%iR%i cameraIdxTable (length: %u)", floorNumber, i, currentRoomDataPtr->numCameraInRoom);
		DebugBeginSection(DBO_CAMERA);
		for (j = 0; j < currentRoomDataPtr->numCameraInRoom; j++) {
			// DebugBPrintf(DBO_L_DEBUG, "%i: ", j); DebugBPrintRaw(DBO_L_DEBUG, PF_LE_U16(roomData + 0xC + 2 * j)); DebugBFlushLn();
			// currentRoomDataPtr->cameraIdxTable[j] = READ_LE_U16(roomData + 0xC + 2 * j);
			DebugBPrintf(DBO_L_DEBUG, "%i: ", j); DebugBPrintRaw(DBO_L_DEBUG, PF_LE_U16(roomData)); DebugBFlushLn();
			currentRoomDataPtr->cameraIdxTable[j] = readU16LE(roomData);
		}
		DebugEndSection();
		// #endregion Read cameras

		// IDEA: hard col read & sce zone read are almost identical; combine?

		// #region hard col read
		/* DebugPrintfLn(DBO_L_LOG, "Loading hardColData from offset %hu...", READ_LE_U16(roomData));
		// u8* hardColData = roomData + READ_LE_U16(roomData);
		// DebugPrintfLn(DBO_L_LOG, "numHardCol: %hu", READ_LE_U16(hardColData));
		// currentRoomDataPtr->numHardCol = READ_LE_U16(hardColData);
		// hardColData += 2;
		AutoAdvancePtr hardColData = AutoAdvancePtr(roomData + READ_LE_U16(roomData)); */
		DebugPrintfLn(DBO_L_LOG, "Loading hardColData from offset %hu...", hardColOffset);
		if ((roomData - roomData_backup) != hardColOffset)
			DebugPrintfLn(DBO_L_WARN, "Expected offset of %hu, got %hu (0x%lX - 0x%lX)", hardColOffset, roomData - roomData_backup, (unsigned long)roomData, (unsigned long)roomData_backup);
		// u8* hardColData = roomData + READ_LE_U16(roomData);
		// DebugPrintfLn(DBO_L_LOG, "numHardCol: %hu", READ_LE_U16(hardColData));
		// currentRoomDataPtr->numHardCol = READ_LE_U16(hardColData);
		// hardColData += 2;
		DebugPrintfLn(DBO_L_LOG, "numHardCol: %hu", READ_LE_U16(roomData));
		currentRoomDataPtr->numHardCol = readU16LE(roomData);
		// AutoAdvancePtr hardColData = AutoAdvancePtr(roomData + hardColOffset);
		// DebugPrintfLn(DBO_L_LOG, "numHardCol: %hu", hardColData.readOnlyU16LE());
		// currentRoomDataPtr->numHardCol = hardColData.readU16LE();

		if (currentRoomDataPtr->numHardCol) {
			currentRoomDataPtr->hardColTable = (hardColStruct*)malloc(sizeof(hardColStruct) * currentRoomDataPtr->numHardCol);

			for (j = 0; j < currentRoomDataPtr->numHardCol; j++) {
				DebugPrintfLn(DBO_L_DEBUG, "hardColTable[%i]:", j);
				DebugBeginSection(DBO_FLOOR);
				ZVStruct* zvData = &currentRoomDataPtr->hardColTable[j].zv;

				/* DebugBPrintf(DBO_L_DEBUG, "ZVX1: "); DebugBPrintRaw(DBO_L_DEBUG, PF_LE_S16(hardColData)); DebugBFlushLn();
				zvData->ZVX1 = READ_LE_S16(advPtr<s16>(hardColData));
				DebugBPrintf(DBO_L_DEBUG, "ZVX2: "); DebugBPrintRaw(DBO_L_DEBUG, PF_LE_S16(hardColData)); DebugBFlushLn();
				zvData->ZVX2 = READ_LE_S16(advPtr<s16>(hardColData));
				DebugBPrintf(DBO_L_DEBUG, "ZVY1: "); DebugBPrintRaw(DBO_L_DEBUG, PF_LE_S16(hardColData)); DebugBFlushLn();
				zvData->ZVY1 = READ_LE_S16(advPtr<s16>(hardColData));
				DebugBPrintf(DBO_L_DEBUG, "ZVY2: "); DebugBPrintRaw(DBO_L_DEBUG, PF_LE_S16(hardColData)); DebugBFlushLn();
				zvData->ZVY2 = READ_LE_S16(advPtr<s16>(hardColData));
				DebugBPrintf(DBO_L_DEBUG, "ZVZ1: "); DebugBPrintRaw(DBO_L_DEBUG, PF_LE_S16(hardColData)); DebugBFlushLn();
				zvData->ZVZ1 = READ_LE_S16(advPtr<s16>(hardColData));
				DebugBPrintf(DBO_L_DEBUG, "ZVZ2: "); DebugBPrintRaw(DBO_L_DEBUG, PF_LE_S16(hardColData)); DebugBFlushLn();
				zvData->ZVZ2 = READ_LE_S16(advPtr<s16>(hardColData));

				DebugBPrintf(DBO_L_DEBUG, "param: "); DebugBPrintRaw(DBO_L_DEBUG, PF_LE_S16(hardColData)); DebugBFlushLn();
				currentRoomDataPtr->hardColTable[j].parameter = READ_LE_U16(advPtr<u16>(hardColData));
				DebugBPrintf(DBO_L_DEBUG, "type: "); DebugBPrintRaw(DBO_L_DEBUG, PF_LE_S16(hardColData)); DebugBFlushLn();
				currentRoomDataPtr->hardColTable[j].type = READ_LE_U16(advPtr<u16>(hardColData)); */
				/* DebugBPrintf(DBO_L_DEBUG, "ZVX1: "); DebugBPrintRaw(DBO_L_DEBUG, PF_LE_S16(hardColData.ptr)); DebugBFlushLn();
				zvData->ZVX1 = hardColData.readS16LE();
				DebugBPrintf(DBO_L_DEBUG, "ZVX2: "); DebugBPrintRaw(DBO_L_DEBUG, PF_LE_S16(hardColData.ptr)); DebugBFlushLn();
				zvData->ZVX2 = hardColData.readS16LE();
				DebugBPrintf(DBO_L_DEBUG, "ZVY1: "); DebugBPrintRaw(DBO_L_DEBUG, PF_LE_S16(hardColData.ptr)); DebugBFlushLn();
				zvData->ZVY1 = hardColData.readS16LE();
				DebugBPrintf(DBO_L_DEBUG, "ZVY2: "); DebugBPrintRaw(DBO_L_DEBUG, PF_LE_S16(hardColData.ptr)); DebugBFlushLn();
				zvData->ZVY2 = hardColData.readS16LE();
				DebugBPrintf(DBO_L_DEBUG, "ZVZ1: "); DebugBPrintRaw(DBO_L_DEBUG, PF_LE_S16(hardColData.ptr)); DebugBFlushLn();
				zvData->ZVZ1 = hardColData.readS16LE();
				DebugBPrintf(DBO_L_DEBUG, "ZVZ2: "); DebugBPrintRaw(DBO_L_DEBUG, PF_LE_S16(hardColData.ptr)); DebugBFlushLn();
				zvData->ZVZ2 = hardColData.readS16LE();

				DebugBPrintf(DBO_L_DEBUG, "param: "); DebugBPrintRaw(DBO_L_DEBUG, PF_LE_U16(hardColData.ptr)); DebugBFlushLn();
				currentRoomDataPtr->hardColTable[j].parameter = hardColData.readU16LE();
				DebugBPrintf(DBO_L_DEBUG, "type: "); DebugBPrintRaw(DBO_L_DEBUG, PF_LE_U16(hardColData.ptr)); DebugBFlushLn();
				currentRoomDataPtr->hardColTable[j].type = hardColData.readU16LE(); */
				DebugBPrintf(DBO_L_DEBUG, "ZVX1: "); DebugBPrintRaw(DBO_L_DEBUG, PF_LE_S16(roomData)); DebugBFlushLn();
				zvData->ZVX1 = readS16LE(roomData);
				DebugBPrintf(DBO_L_DEBUG, "ZVX2: "); DebugBPrintRaw(DBO_L_DEBUG, PF_LE_S16(roomData)); DebugBFlushLn();
				zvData->ZVX2 = readS16LE(roomData);
				DebugBPrintf(DBO_L_DEBUG, "ZVY1: "); DebugBPrintRaw(DBO_L_DEBUG, PF_LE_S16(roomData)); DebugBFlushLn();
				zvData->ZVY1 = readS16LE(roomData);
				DebugBPrintf(DBO_L_DEBUG, "ZVY2: "); DebugBPrintRaw(DBO_L_DEBUG, PF_LE_S16(roomData)); DebugBFlushLn();
				zvData->ZVY2 = readS16LE(roomData);
				DebugBPrintf(DBO_L_DEBUG, "ZVZ1: "); DebugBPrintRaw(DBO_L_DEBUG, PF_LE_S16(roomData)); DebugBFlushLn();
				zvData->ZVZ1 = readS16LE(roomData);
				DebugBPrintf(DBO_L_DEBUG, "ZVZ2: "); DebugBPrintRaw(DBO_L_DEBUG, PF_LE_S16(roomData)); DebugBFlushLn();
				zvData->ZVZ2 = readS16LE(roomData);

				DebugBPrintf(DBO_L_DEBUG, "param: "); DebugBPrintRaw(DBO_L_DEBUG, PF_LE_U16(roomData)); DebugBFlushLn();
				currentRoomDataPtr->hardColTable[j].parameter = readU16LE(roomData);
				DebugBPrintf(DBO_L_DEBUG, "type: "); DebugBPrintRaw(DBO_L_DEBUG, PF_LE_U16(roomData)); DebugBFlushLn();
				currentRoomDataPtr->hardColTable[j].type = readU16LE(roomData);

				DebugEndSection();
			}
		} else currentRoomDataPtr->hardColTable = NULL;
		// #endregion hard col read

		// #region sce zone read
		/* // DebugPrintfLn(DBO_L_LOG, "Loading sceZoneData from offset %hu...", READ_LE_U16(roomData + 2));
		DebugBPrintf(DBO_L_LOG, "Loading sceZoneData from offset "); DebugBPrintRaw(DBO_L_LOG, PF_LE_U16(roomData + 2)); DebugBPrintf(DBO_L_LOG, "..."); DebugBFlushLn();
		roomData = roomData_backup;
		u8* sceZoneData = roomData + READ_LE_U16(roomData + 2);
		DebugBPrintf(DBO_L_LOG, "numSceZone: "); DebugBPrintRaw(DBO_L_LOG, PF_LE_U16(sceZoneData)); DebugBFlushLn();
		// DebugPrintfLn(DBO_L_LOG, "numSceZone: %hu", READ_LE_U16(sceZoneData));
		currentRoomDataPtr->numSceZone = READ_LE_U16(sceZoneData);
		sceZoneData += 2;
		*/
		DebugPrintfLn(DBO_L_LOG, "Loading sceZoneData from offset %hu...", sceZoneOffset);
		if ((unsigned long)((unsigned long)roomData - (unsigned long)roomData_backup) != sceZoneOffset)
			DebugPrintfLn(DBO_L_WARN, "Expected offset of %hu, got %lu (0x%lX - 0x%lX)", sceZoneOffset, (unsigned long)roomData - (unsigned long)roomData_backup, (unsigned long)roomData, (unsigned long)roomData_backup);
		DebugBPrintf(DBO_L_LOG, "numSceZone: "); DebugBPrintRaw(DBO_L_LOG, PF_LE_U16(roomData)); DebugBFlushLn();
		currentRoomDataPtr->numSceZone = readU16LE(roomData);

		if (currentRoomDataPtr->numSceZone) {
			currentRoomDataPtr->sceZoneTable = (sceZoneStruct*)malloc(sizeof(sceZoneStruct) * currentRoomDataPtr->numSceZone);

			for (j = 0; j < currentRoomDataPtr->numSceZone; j++) {
				DebugPrintfLn(DBO_L_DEBUG, "sceZoneTable[%i]:", j);
				DebugBeginSection(DBO_FLOOR);
				ZVStruct* zvData = &currentRoomDataPtr->sceZoneTable[j].zv;

				/* DebugBPrintf(DBO_L_DEBUG, "ZVX1: "); DebugBPrintRaw(DBO_L_DEBUG, PF_LE_S16(sceZoneData + 0x00)); DebugBFlushLn();
				zvData->ZVX1 = READ_LE_S16(sceZoneData + 0x00);
				DebugBPrintf(DBO_L_DEBUG, "ZVX2: "); DebugBPrintRaw(DBO_L_DEBUG, PF_LE_S16(sceZoneData + 0x02)); DebugBFlushLn();
				zvData->ZVX2 = READ_LE_S16(sceZoneData + 0x02);
				DebugBPrintf(DBO_L_DEBUG, "ZVY1: "); DebugBPrintRaw(DBO_L_DEBUG, PF_LE_S16(sceZoneData + 0x04)); DebugBFlushLn();
				zvData->ZVY1 = READ_LE_S16(sceZoneData + 0x04);
				DebugBPrintf(DBO_L_DEBUG, "ZVY2: "); DebugBPrintRaw(DBO_L_DEBUG, PF_LE_S16(sceZoneData + 0x06)); DebugBFlushLn();
				zvData->ZVY2 = READ_LE_S16(sceZoneData + 0x06);
				DebugBPrintf(DBO_L_DEBUG, "ZVZ1: "); DebugBPrintRaw(DBO_L_DEBUG, PF_LE_S16(sceZoneData + 0x08)); DebugBFlushLn();
				zvData->ZVZ1 = READ_LE_S16(sceZoneData + 0x08);
				DebugBPrintf(DBO_L_DEBUG, "ZVZ2: "); DebugBPrintRaw(DBO_L_DEBUG, PF_LE_S16(sceZoneData + 0x0A)); DebugBFlushLn();
				zvData->ZVZ2 = READ_LE_S16(sceZoneData + 0x0A);

				DebugBPrintf(DBO_L_DEBUG, "param: "); DebugBPrintRaw(DBO_L_DEBUG, PF_LE_S16(sceZoneData + 0x0C)); DebugBFlushLn();
				currentRoomDataPtr->sceZoneTable[j].parameter = READ_LE_U16(sceZoneData + 0x0C);
				DebugBPrintf(DBO_L_DEBUG, "type: "); DebugBPrintRaw(DBO_L_DEBUG, PF_LE_S16(sceZoneData + 0x0E)); DebugBFlushLn();
				currentRoomDataPtr->sceZoneTable[j].type = READ_LE_U16(sceZoneData + 0x0E);

				sceZoneData += 0x10; */
				DebugBPrintf(DBO_L_DEBUG, "ZVX1: "); DebugBPrintRaw(DBO_L_DEBUG, PF_LE_S16(roomData)); DebugBFlushLn();
				zvData->ZVX1 = readS16LE(roomData);
				DebugBPrintf(DBO_L_DEBUG, "ZVX2: "); DebugBPrintRaw(DBO_L_DEBUG, PF_LE_S16(roomData)); DebugBFlushLn();
				zvData->ZVX2 = readS16LE(roomData);
				DebugBPrintf(DBO_L_DEBUG, "ZVY1: "); DebugBPrintRaw(DBO_L_DEBUG, PF_LE_S16(roomData)); DebugBFlushLn();
				zvData->ZVY1 = readS16LE(roomData);
				DebugBPrintf(DBO_L_DEBUG, "ZVY2: "); DebugBPrintRaw(DBO_L_DEBUG, PF_LE_S16(roomData)); DebugBFlushLn();
				zvData->ZVY2 = readS16LE(roomData);
				DebugBPrintf(DBO_L_DEBUG, "ZVZ1: "); DebugBPrintRaw(DBO_L_DEBUG, PF_LE_S16(roomData)); DebugBFlushLn();
				zvData->ZVZ1 = readS16LE(roomData);
				DebugBPrintf(DBO_L_DEBUG, "ZVZ2: "); DebugBPrintRaw(DBO_L_DEBUG, PF_LE_S16(roomData)); DebugBFlushLn();
				zvData->ZVZ2 = readS16LE(roomData);

				DebugBPrintf(DBO_L_DEBUG, "param: "); DebugBPrintRaw(DBO_L_DEBUG, PF_LE_U16(roomData)); DebugBFlushLn();
				currentRoomDataPtr->sceZoneTable[j].parameter = readU16LE(roomData);
				DebugBPrintf(DBO_L_DEBUG, "type: "); DebugBPrintRaw(DBO_L_DEBUG, PF_LE_U16(roomData)); DebugBFlushLn();
				currentRoomDataPtr->sceZoneTable[j].type = readU16LE(roomData);

				DebugEndSection();
			}
		} else currentRoomDataPtr->sceZoneTable = NULL;
		// #endregion sce zone read
		DebugEndSection();
	}
	// #endregion Load rooms //////////////////////////////////

	// #region camera stuff /////////////////////////////////////////////////
	int expectedNumberOfCamera;
	// TODO: Fix AITD3 case
	// if (g_gameId >= AITD3) {
	if (g_gameId > AITD3) {
		char buffer[256];

		sprintf(buffer, g_gameId == AITD3 ? "CAM%02d" : "CAMSAL%02d", floorNumber);

		expectedNumberOfCamera = PAK_getNumFiles(buffer);
	} else {
		int maxExpectedNumberOfCamera = ((READ_LE_U32(g_currentFloorCameraRawData)) / 4);

		expectedNumberOfCamera = 0;

		int minOffset = 0;

		for (int i = 0; i < maxExpectedNumberOfCamera; i++) {
			int offset = READ_LE_U32(g_currentFloorCameraRawData + i * 4);
			if (offset <= minOffset) break;
			minOffset = offset;
			expectedNumberOfCamera++;
		}
	}

	g_currentFloorCameraData.clear();
	g_currentFloorCameraData.resize(expectedNumberOfCamera);

	DebugPrintfLn(DBO_L_LOG, "Loading the %i expected cameras...", expectedNumberOfCamera);
	for (i = 0; i < expectedNumberOfCamera; i++) {
		DebugPrintfLn(DBO_L_DEBUG, "Camera %i:", i);
		DebugBeginSection(DBO_FLOOR);
		int k;
		uint offset;
		u8* currentCameraData;

		// if (g_gameId >= AITD3) {
		if (g_gameId > AITD3) {
			char buffer[256];

			// TODO: Fix AITD3 case
			sprintf(buffer, g_gameId == AITD3 ? "CAM%02d" : "CAMSAL%02d", floorNumber);

			offset = 0;
			g_currentFloorCameraRawDataSize = 1;
			currentCameraData = (u8*)CheckLoadMallocPak(buffer, i);
		} else offset = READ_LE_U32(g_currentFloorCameraRawData + i * 4);

		// load cameras
		if (offset < g_currentFloorCameraRawDataSize) {
			// TODO: Fix AITD3 case
			// if (g_gameId < AITD3)
			if (g_gameId <= AITD3) {
				DebugBPrintf(DBO_L_DEBUG, "Loading cam data from offset "); DebugBPrintRaw(DBO_L_DEBUG, PF_LE_U32(g_currentFloorCameraRawData + i * 4)); /* DebugBPrintf(DBO_L_DEBUG, "...");  */DebugBFlushLn();
				currentCameraData = (u8*)(g_currentFloorCameraRawData + READ_LE_U32(g_currentFloorCameraRawData + i * 4));
			}

			u8* backupDataPtr = currentCameraData;

			DebugBPrintf(DBO_L_DEBUG, "alpha: "); DebugBPrintRaw(DBO_L_DEBUG, PF_LE_U16(currentCameraData + 0x00)); DebugBFlushLn();
			g_currentFloorCameraData[i].alpha = READ_LE_U16(currentCameraData + 0x00);
			DebugBPrintf(DBO_L_DEBUG, "beta: "); DebugBPrintRaw(DBO_L_DEBUG, PF_LE_U16(currentCameraData + 0x02)); DebugBFlushLn();
			g_currentFloorCameraData[i].beta = READ_LE_U16(currentCameraData + 0x02);
			DebugBPrintf(DBO_L_DEBUG, "gamma: "); DebugBPrintRaw(DBO_L_DEBUG, PF_LE_U16(currentCameraData + 0x04)); DebugBFlushLn();
			g_currentFloorCameraData[i].gamma = READ_LE_U16(currentCameraData + 0x04);

			DebugBPrintf(DBO_L_DEBUG, "x: "); DebugBPrintRaw(DBO_L_DEBUG, PF_LE_S16(currentCameraData + 0x06)); DebugBFlushLn();
			g_currentFloorCameraData[i].x = READ_LE_S16(currentCameraData + 0x06);
			DebugBPrintf(DBO_L_DEBUG, "y: "); DebugBPrintRaw(DBO_L_DEBUG, PF_LE_S16(currentCameraData + 0x08)); DebugBFlushLn();
			g_currentFloorCameraData[i].y = READ_LE_S16(currentCameraData + 0x08);
			DebugBPrintf(DBO_L_DEBUG, "z: "); DebugBPrintRaw(DBO_L_DEBUG, PF_LE_S16(currentCameraData + 0x0A)); DebugBFlushLn();
			g_currentFloorCameraData[i].z = READ_LE_S16(currentCameraData + 0x0A);

			DebugBPrintf(DBO_L_DEBUG, "focal1: "); DebugBPrintRaw(DBO_L_DEBUG, PF_LE_U16(currentCameraData + 0x0C)); DebugBFlushLn();
			g_currentFloorCameraData[i].focal1 = READ_LE_U16(currentCameraData + 0x0C);
			DebugBPrintf(DBO_L_DEBUG, "focal2: "); DebugBPrintRaw(DBO_L_DEBUG, PF_LE_U16(currentCameraData + 0x0E)); DebugBFlushLn();
			g_currentFloorCameraData[i].focal2 = READ_LE_U16(currentCameraData + 0x0E);
			DebugBPrintf(DBO_L_DEBUG, "focal3: "); DebugBPrintRaw(DBO_L_DEBUG, PF_LE_U16(currentCameraData + 0x10)); DebugBFlushLn();
			g_currentFloorCameraData[i].focal3 = READ_LE_U16(currentCameraData + 0x10);

			DebugBPrintf(DBO_L_DEBUG, "numViewedRooms: "); DebugBPrintRaw(DBO_L_DEBUG, PF_LE_U16(currentCameraData + 0x12)); DebugBFlushLn();
			g_currentFloorCameraData[i].numViewedRooms = READ_LE_U16(currentCameraData + 0x12);

			currentCameraData += 0x14;

			g_currentFloorCameraData[i].viewedRoomTable = (cameraViewedRoomStruct*)malloc(sizeof(cameraViewedRoomStruct) * g_currentFloorCameraData[i].numViewedRooms);
			ASSERT(g_currentFloorCameraData[i].viewedRoomTable);
			memset(g_currentFloorCameraData[i].viewedRoomTable, 0, sizeof(cameraViewedRoomStruct) * g_currentFloorCameraData[i].numViewedRooms);

			// TODO: Add Debug Lines
			for (k = 0; k < g_currentFloorCameraData[i].numViewedRooms; k++) {
				cameraViewedRoomStruct* pCurrentCameraViewedRoom = &g_currentFloorCameraData[i].viewedRoomTable[k];

				pCurrentCameraViewedRoom->viewedRoomIdx = READ_LE_U16(currentCameraData + 0x00);
				pCurrentCameraViewedRoom->offsetToMask = READ_LE_U16(currentCameraData + 0x02);
				pCurrentCameraViewedRoom->offsetToCover = READ_LE_U16(currentCameraData + 0x04);

				if (g_gameId == AITD1) {
					pCurrentCameraViewedRoom->offsetToHybrids = 0;
					pCurrentCameraViewedRoom->offsetCamOptims = 0;
					pCurrentCameraViewedRoom->lightX = READ_LE_U16(currentCameraData + 0x06);
					pCurrentCameraViewedRoom->lightY = READ_LE_U16(currentCameraData + 0x08);
					pCurrentCameraViewedRoom->lightZ = READ_LE_U16(currentCameraData + 0x0A);
				} else {
					pCurrentCameraViewedRoom->offsetToHybrids = READ_LE_U16(currentCameraData + 0x06);
					pCurrentCameraViewedRoom->offsetCamOptims = READ_LE_U16(currentCameraData + 0x08);
					pCurrentCameraViewedRoom->lightX = READ_LE_U16(currentCameraData + 0x0A);
					pCurrentCameraViewedRoom->lightY = READ_LE_U16(currentCameraData + 0x0C);
					pCurrentCameraViewedRoom->lightZ = READ_LE_U16(currentCameraData + 0x0E);
				}

				// load camera mask
				u8* pMaskData = NULL;
				if (g_gameId >= JACK) {
					pMaskData = backupDataPtr + g_currentFloorCameraData[i].viewedRoomTable[k].offsetToMask;

					// for this camera, how many masks zone
					pCurrentCameraViewedRoom->numMask = READ_LE_U16(pMaskData);
					pMaskData += 2;

					pCurrentCameraViewedRoom->masks = (cameraMaskStruct*)malloc(sizeof(cameraMaskStruct) * pCurrentCameraViewedRoom->numMask);
					memset(pCurrentCameraViewedRoom->masks, 0, sizeof(cameraMaskStruct) * pCurrentCameraViewedRoom->numMask);

					for (int k = 0; k < pCurrentCameraViewedRoom->numMask; k++) {
						cameraMaskStruct* pCurrentCameraMask = &pCurrentCameraViewedRoom->masks[k];

						// for this overlay zone, how many 
						pCurrentCameraMask->numTestRect = READ_LE_U16(pMaskData);
						pMaskData += 2;

						pCurrentCameraMask->rectTests = (rectTestStruct*)malloc(sizeof(rectTestStruct) * pCurrentCameraMask->numTestRect);
						memset(pCurrentCameraMask->rectTests, 0, sizeof(rectTestStruct) * pCurrentCameraMask->numTestRect);

						for (int j = 0; j < pCurrentCameraMask->numTestRect; j++) {
							rectTestStruct* pCurrentRectTest = &pCurrentCameraMask->rectTests[j];

							pCurrentRectTest->zoneX1 = READ_LE_S16(pMaskData);
							pCurrentRectTest->zoneZ1 = READ_LE_S16(pMaskData + 2);
							pCurrentRectTest->zoneX2 = READ_LE_S16(pMaskData + 4);
							pCurrentRectTest->zoneZ2 = READ_LE_S16(pMaskData + 6);
							pMaskData += 8;
						}
					}
				}

				// load camera cover
				{
					u8* pZoneData = backupDataPtr + g_currentFloorCameraData[i].viewedRoomTable[k].offsetToCover;
					if (pMaskData) assert(pZoneData == pMaskData);
					//pZoneData = currentCameraData;

					int numZones = pCurrentCameraViewedRoom->numCoverZones = READ_LE_U16(pZoneData);
					pZoneData += 2;

					pCurrentCameraViewedRoom->coverZones = (cameraZoneEntryStruct*)malloc(sizeof(cameraZoneEntryStruct) * numZones);

					ASSERT(pCurrentCameraViewedRoom->coverZones);

					for (int j = 0; j < pCurrentCameraViewedRoom->numCoverZones; j++) {
						int numPoints;
						pCurrentCameraViewedRoom->coverZones[j].numPoints = numPoints = READ_LE_U16(pZoneData);
						pZoneData += 2;

						pCurrentCameraViewedRoom->coverZones[j].pointTable = (cameraZonePointStruct*)malloc(sizeof(cameraZonePointStruct) * (numPoints + 1));

						for (int pointIdx = 0; pointIdx < pCurrentCameraViewedRoom->coverZones[j].numPoints; pointIdx++) {
							pCurrentCameraViewedRoom->coverZones[j].pointTable[pointIdx].x = READ_LE_U16(pZoneData);
							pZoneData += 2;
							pCurrentCameraViewedRoom->coverZones[j].pointTable[pointIdx].y = READ_LE_U16(pZoneData);
							pZoneData += 2;
						}

						// copy first point to last position
						pCurrentCameraViewedRoom->coverZones[j].pointTable[numPoints].x = pCurrentCameraViewedRoom->coverZones[j].pointTable[0].x;
						pCurrentCameraViewedRoom->coverZones[j].pointTable[numPoints].y = pCurrentCameraViewedRoom->coverZones[j].pointTable[0].y;
					}
				}

				if (g_gameId == AITD1)
					currentCameraData += 0x0C;
				else
					currentCameraData += 0x10;

				if (g_gameId == TIMEGATE)
					currentCameraData += 6;
			}
		} else { // Early exit
			DebugPrintfLn(DBO_L_WARN, "Expected %i cameras, but exhausted data store at %i cameras; exiting early.", expectedNumberOfCamera, i);
			DebugEndSection();
			break;
		}
		DebugEndSection();
	}

	g_currentFloorNumCamera = i - 1;
	DebugPrintfLn(DBO_L_LOG, "g_currentFloorNumCamera set to last loaded camera (%u)", g_currentFloorNumCamera);

	// globalCameraDataTable = (cameraDataStruct*)realloc(globalCameraDataTable,sizeof(cameraDataStruct)*numGlobalCamera);

	/*    roomCameraData+=0x14;
	}*/
	// #endregion camera stuff /////////////////////////////////////////////////
	DebugEndSection();
}
