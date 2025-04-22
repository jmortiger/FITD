#include "common.h"

u32 g_currentFloorRoomRawDataSize = 0;
u32 g_currentFloorCameraRawDataSize;
u32 g_currentFloorNumCamera = 0;
std::vector<cameraDataStruct> g_currentFloorCameraData;

void loadFloor(int floorNumber)
{
    int i;
    int expectedNumberOfRoom;
    int expectedNumberOfCamera;
    char floorFileName[256];

    if(g_currentFloorCameraRawData)
    {
        free(g_currentFloorCameraRawData);
        free(g_currentFloorRoomRawData);
    }

    //stopSounds();

    HQR_Reset(listBody);
    HQR_Reset(listAnim);

    g_currentFloor = floorNumber;

    if(g_gameId < AITD3)
    {
        sprintf(floorFileName,"ETAGE%02d",floorNumber);
        printf("loadFloor: Loading floor %i...\n", floorNumber);

        g_currentFloorRoomRawDataSize = getPakSize(floorFileName,0);
        printf("\tfloorRoomRawDataSize: %lu...\n", g_currentFloorRoomRawDataSize);
        g_currentFloorCameraRawDataSize = getPakSize(floorFileName,1);
        printf("\tfloorCameraRawDataSize: %lu...\n", g_currentFloorCameraRawDataSize);

        g_currentFloorRoomRawData = CheckLoadMallocPak(floorFileName,0);
        g_currentFloorCameraRawData = CheckLoadMallocPak(floorFileName,1);
    }

    currentCamera = -1;
    needChangeRoom = 1;
    changeFloor = 0;

    //////////////////////////////////

    if(roomDataTable)
    {
        free(roomDataTable);
        roomDataTable = NULL;
    }

    expectedNumberOfRoom = getNumberOfRoom();
    printf("\tExpecting %i Rooms on Floor %i\n", expectedNumberOfRoom, floorNumber);

    for(i=0;i<expectedNumberOfRoom;i++)
    {
        printf("\tE%iR%i:\n", floorNumber, i);
        u32 j;
        u8* roomData;
        u8* hardColData;
        u8* sceZoneData;
        roomDataStruct* currentRoomDataPtr;

        if(roomDataTable)
        {
            roomDataTable = (roomDataStruct*)realloc(roomDataTable,sizeof(roomDataStruct)*(i+1));
        }
        else
        {
            roomDataTable = (roomDataStruct*)malloc(sizeof(roomDataStruct));
        }

        if(g_gameId >= AITD3)
        {
            char buffer[256];

            if(g_gameId == AITD3)
            {
                sprintf(buffer,"SAL%02d",floorNumber);
            }
            else
            {
                sprintf(buffer,"ETAGE%02d",floorNumber);
            }

            roomData = (u8*)CheckLoadMallocPak(buffer,i);
        }
        else
        {
            printf("\t\troomData Offset: %lu (Raw: %lu)", READ_LE_U32(g_currentFloorRoomRawData + i * 4), *(u32*)(g_currentFloorRoomRawData + i * 4));
            roomData = (u8*)(g_currentFloorRoomRawData + READ_LE_U32(g_currentFloorRoomRawData + i * 4));
        }
        currentRoomDataPtr = &roomDataTable[i];

        printf("\t\tworldX: %hi (Raw: %hi)\n", READ_LE_S16(roomData + 4), *(s16*)(roomData + 4));
        printf("\t\tworldY: %hi (Raw: %hi)\n", READ_LE_S16(roomData + 6), *(s16*)(roomData + 6));
        printf("\t\tworldZ: %hi (Raw: %hi)\n", READ_LE_S16(roomData + 8), *(s16*)(roomData + 8));
        currentRoomDataPtr->worldX = READ_LE_S16(roomData+4);
        currentRoomDataPtr->worldY = READ_LE_S16(roomData+6);
        currentRoomDataPtr->worldZ = READ_LE_S16(roomData+8);
        // printf("\t\tworldX: %li (Raw: %hi)\n", currentRoomDataPtr->worldX, *(s16 *)(roomData + 4));
        // printf("\t\tworldY: %li (Raw: %hi)\n", currentRoomDataPtr->worldY, *(s16 *)(roomData + 6));
        // printf("\t\tworldZ: %li (Raw: %hi)\n", currentRoomDataPtr->worldZ, *(s16 *)(roomData + 8));

        printf("\t\tnumCameraInRoom: %hu (Raw: %hu)\n", READ_LE_U16(roomData + 0xA), *(s16*)(roomData + 0xA));
        currentRoomDataPtr->numCameraInRoom = READ_LE_U16(roomData+0xA);
        // printf("\t\tnumCameraInRoom: %li (Raw: %hu)\n", currentRoomDataPtr->numCameraInRoom, *(s16 *)(roomData + 0xA));

        // TODO: Not that it matters, but use u16 for sizeof
        currentRoomDataPtr->cameraIdxTable = (u16*)malloc(currentRoomDataPtr->numCameraInRoom*sizeof(s16));

        for(j=0;j<currentRoomDataPtr->numCameraInRoom;j++)
        {
            printf("\t\t\tid[%lu]: %hi (Raw: %hu)\n", j, READ_LE_U16(roomData + 0xC + 2 * j), *(u16*)(roomData + 0xC + 2 * j));
            currentRoomDataPtr->cameraIdxTable[j] = READ_LE_U16(roomData+0xC+2*j);
            // printf("\t\t\tid[%lu]: %hi (Raw: %hu)\n", j, currentRoomDataPtr->cameraIdxTable[j], *(u16 *)(roomData + 0xC + 2 * j));
        }

        // hard col read

        printf("\t\thardColData Offset: %hu (Raw: %hu)\n", READ_LE_U16(roomData), *(u16*)(roomData));
        hardColData = roomData + READ_LE_U16(roomData);
        printf("\t\tnumHardCol: %hu (Raw: %hu)\n", READ_LE_U16(hardColData), *(u16*)(hardColData));
        currentRoomDataPtr->numHardCol = READ_LE_U16(hardColData);
        // printf("\t\tnumHardCol: %1u (Raw: %hu)\n", currentRoomDataPtr->numHardCol, *(u16 *)(hardColData));
        hardColData+=2;

        if(currentRoomDataPtr->numHardCol)
        {
            currentRoomDataPtr->hardColTable = (hardColStruct*)malloc(sizeof(hardColStruct)*currentRoomDataPtr->numHardCol);

            for(j=0;j<currentRoomDataPtr->numHardCol;j++)
            {
                ZVStruct* zvData;

                zvData = &currentRoomDataPtr->hardColTable[j].zv;

                printf("\t\t\tCol #%lu:\n", j);
                printf("\t\t\t\tZVX1: %hi (Raw: %hi)\n", READ_LE_S16(hardColData + 0x00), *(s16*)(hardColData + 0x00));
                printf("\t\t\t\tZVX2: %hi (Raw: %hi)\n", READ_LE_S16(hardColData + 0x02), *(s16*)(hardColData + 0x02));
                printf("\t\t\t\tZVY1: %hi (Raw: %hi)\n", READ_LE_S16(hardColData + 0x04), *(s16*)(hardColData + 0x04));
                printf("\t\t\t\tZVY2: %hi (Raw: %hi)\n", READ_LE_S16(hardColData + 0x06), *(s16*)(hardColData + 0x06));
                printf("\t\t\t\tZVZ1: %hi (Raw: %hi)\n", READ_LE_S16(hardColData + 0x08), *(s16*)(hardColData + 0x08));
                printf("\t\t\t\tZVZ2: %hi (Raw: %hi)\n", READ_LE_S16(hardColData + 0x0A), *(s16*)(hardColData + 0x0A));
                printf("\t\t\t\tparameter: %hu (Raw: %hu)\n", READ_LE_U16(hardColData + 0x0C), *(u16*)(hardColData + 0x0C));
                printf("\t\t\t\ttype: %hu (Raw: %hu)\n", READ_LE_U16(hardColData + 0x0E), *(u16*)(hardColData + 0x0E));
                // printf("\t\t\tCol #%lu:\n", j);
                zvData->ZVX1 = READ_LE_S16(hardColData+0x00);
                // printf("\t\t\t\tZVX1: %li (Raw: %hi)\n", zvData->ZVX1, *(s16 *)(hardColData + 0x00));
                zvData->ZVX2 = READ_LE_S16(hardColData+0x02);
                // printf("\t\t\t\tZVX2: %li (Raw: %hi)\n", zvData->ZVX2, *(s16 *)(hardColData + 0x02));
                zvData->ZVY1 = READ_LE_S16(hardColData+0x04);
                // printf("\t\t\t\tZVY1: %li (Raw: %hi)\n", zvData->ZVY1, *(s16 *)(hardColData + 0x04));
                zvData->ZVY2 = READ_LE_S16(hardColData+0x06);
                // printf("\t\t\t\tZVY2: %li (Raw: %hi)\n", zvData->ZVY2, *(s16 *)(hardColData + 0x06));
                zvData->ZVZ1 = READ_LE_S16(hardColData+0x08);
                // printf("\t\t\t\tZVZ1: %li (Raw: %hi)\n", zvData->ZVZ1, *(s16 *)(hardColData + 0x08));
                zvData->ZVZ2 = READ_LE_S16(hardColData+0x0A);
                // printf("\t\t\t\tZVZ2: %li (Raw: %hi)\n", zvData->ZVZ2, *(s16 *)(hardColData + 0x0A));

                currentRoomDataPtr->hardColTable[j].parameter = READ_LE_U16(hardColData+0x0C);
                // printf("\t\t\t\tparameter: %lu (Raw: %hu)\n", currentRoomDataPtr->hardColTable[j].parameter, *(u16 *)(hardColData + 0x0C));
                currentRoomDataPtr->hardColTable[j].type = READ_LE_U16(hardColData+0x0E);
                // printf("\t\t\t\ttype: %lu (Raw: %hu)\n", currentRoomDataPtr->hardColTable[j].type, *(u16 *)(hardColData + 0x0E));

                hardColData+=0x10;
            }
        }
        else
        {
            currentRoomDataPtr->hardColTable = NULL;
        }

        // sce zone read

        printf("\t\tsceZoneData Offset: %hu (Raw: %hu)\n", READ_LE_U16(roomData + 2), *(u16*)(roomData + 2));
        sceZoneData = roomData + READ_LE_U16(roomData+2);
        printf("\t\tnumSceZone: %hu (Raw: %hu)\n", READ_LE_U16(roomData + 2), *(u16*)(roomData + 2));
        currentRoomDataPtr->numSceZone = READ_LE_U16(sceZoneData);
        // printf("\t\tnumSceZone: %lu (Raw: %hu)\n", currentRoomDataPtr->numSceZone, *(u16 *)(roomData + 2));
        sceZoneData+=2;

        if(currentRoomDataPtr->numSceZone)
        {
            currentRoomDataPtr->sceZoneTable = (sceZoneStruct*)malloc(sizeof(sceZoneStruct)*currentRoomDataPtr->numSceZone);

            for(j=0;j<currentRoomDataPtr->numSceZone;j++)
            {
                ZVStruct* zvData;

                zvData = &currentRoomDataPtr->sceZoneTable[j].zv;

                printf("\t\t\tSce #%lu:\n", j);
                printf("\t\t\t\tZVX1: %hi (Raw: %hi)\n", READ_LE_S16(sceZoneData + 0x00), *(s16*)(sceZoneData + 0x00));
                printf("\t\t\t\tZVX2: %hi (Raw: %hi)\n", READ_LE_S16(sceZoneData + 0x02), *(s16*)(sceZoneData + 0x02));
                printf("\t\t\t\tZVY1: %hi (Raw: %hi)\n", READ_LE_S16(sceZoneData + 0x04), *(s16*)(sceZoneData + 0x04));
                printf("\t\t\t\tZVY2: %hi (Raw: %hi)\n", READ_LE_S16(sceZoneData + 0x06), *(s16*)(sceZoneData + 0x06));
                printf("\t\t\t\tZVZ1: %hi (Raw: %hi)\n", READ_LE_S16(sceZoneData + 0x08), *(s16*)(sceZoneData + 0x08));
                printf("\t\t\t\tZVZ2: %hi (Raw: %hi)\n", READ_LE_S16(sceZoneData + 0x0A), *(s16*)(sceZoneData + 0x0A));
                printf("\t\t\t\tparameter: %hu (Raw: %hu)\n", READ_LE_U16(sceZoneData + 0x0C), *(u16*)(sceZoneData + 0x0C));
                printf("\t\t\t\ttype: %hu (Raw: %hu)\n", READ_LE_U16(sceZoneData + 0x0E), *(u16*)(sceZoneData + 0x0E));
                // printf("\t\t\tSce #%lu:\n", j);
                zvData->ZVX1 = READ_LE_S16(sceZoneData+0x00);
                // printf("\t\t\t\tZVX1: %li (Raw: %hi)\n", zvData->ZVX1, *(s16 *)(sceZoneData + 0x00));
                zvData->ZVX2 = READ_LE_S16(sceZoneData+0x02);
                // printf("\t\t\t\tZVX2: %li (Raw: %hi)\n", zvData->ZVX2, *(s16 *)(sceZoneData + 0x02));
                zvData->ZVY1 = READ_LE_S16(sceZoneData+0x04);
                // printf("\t\t\t\tZVY1: %li (Raw: %hi)\n", zvData->ZVY1, *(s16 *)(sceZoneData + 0x04));
                zvData->ZVY2 = READ_LE_S16(sceZoneData+0x06);
                // printf("\t\t\t\tZVY2: %li (Raw: %hi)\n", zvData->ZVY2, *(s16 *)(sceZoneData + 0x06));
                zvData->ZVZ1 = READ_LE_S16(sceZoneData+0x08);
                // printf("\t\t\t\tZVZ1: %li (Raw: %hi)\n", zvData->ZVZ1, *(s16 *)(sceZoneData + 0x08));
                zvData->ZVZ2 = READ_LE_S16(sceZoneData+0x0A);
                // printf("\t\t\t\tZVZ2: %li (Raw: %hi)\n", zvData->ZVZ2, *(s16 *)(sceZoneData + 0x0A));

                currentRoomDataPtr->sceZoneTable[j].parameter = READ_LE_U16(sceZoneData+0x0C);
                // printf("\t\t\t\tparameter: %lu (Raw: %hu)\n", currentRoomDataPtr->sceZoneTable[j].parameter, *(u16 *)(sceZoneData + 0x0C));
                currentRoomDataPtr->sceZoneTable[j].type = READ_LE_U16(sceZoneData+0x0E);
                // printf("\t\t\t\ttype: %lu (Raw: %hu)\n", currentRoomDataPtr->sceZoneTable[j].type, *(u16 *)(sceZoneData + 0x0E));

                sceZoneData+=0x10;
            }
        }
        else
        {
            currentRoomDataPtr->sceZoneTable = NULL;
        }
    }
    printf("\t%i Rooms on Floor %i, expected %i\n", i, floorNumber, expectedNumberOfRoom);
    ///////////////////////////////////

    /////////////////////////////////////////////////
    // camera stuff

    if(g_gameId >= AITD3)
    {
        char buffer[256];

        if(g_gameId == AITD3)
        {
            sprintf(buffer,"CAM%02d",floorNumber);
        }
        else
        {
            sprintf(buffer,"CAMSAL%02d",floorNumber);
        }

        expectedNumberOfCamera = PAK_getNumFiles(buffer);
    }
    else
    {
        printf("\tCan't have more than %lu (Raw: %lu / 4 = %f) cameras on Floor %i\n", (READ_LE_U32(g_currentFloorCameraRawData)) / 4, *(u32*)g_currentFloorCameraRawData, (*(u32*)g_currentFloorCameraRawData) / 4.0, floorNumber);
        int maxExpectedNumberOfCamera = ((READ_LE_U32(g_currentFloorCameraRawData))/4);
        printf("\tCan't have more than %i (Raw: %lu / 4 = %f) cameras on Floor %i\n", maxExpectedNumberOfCamera, *(u32*)g_currentFloorCameraRawData, (*(u32*)g_currentFloorCameraRawData) / 4.0, floorNumber);

		expectedNumberOfCamera = 0;

		int minOffset = 0;

		for(int i=0; i<maxExpectedNumberOfCamera; i++)
		{
            printf("\t\tOffset #%i: %lu (Raw: %lu); Prior Offset = %i", i, READ_LE_U32(g_currentFloorCameraRawData + i * 4), *(u32*)(g_currentFloorCameraRawData + i * 4), minOffset);
			int offset = READ_LE_U32(g_currentFloorCameraRawData + i * 4);
            printf("\t\t\tOffset #%i: %i (Raw: %lu); Prior Offset = %i", i, offset, *(u32*)(g_currentFloorCameraRawData + i * 4), minOffset);
			if(offset > minOffset)
			{
				minOffset = offset;
				expectedNumberOfCamera++;
			}
			else
			{
				break;
			}
		}
    }

    printf("\tExpecting %i Cameras on Floor %i\n", expectedNumberOfCamera, floorNumber);
    g_currentFloorCameraData.clear();
    g_currentFloorCameraData.resize(expectedNumberOfCamera);

    for(i=0;i<expectedNumberOfCamera;i++)
    {
        int k;
        unsigned int offset;
        unsigned char* currentCameraData;

        // TODO: Improve this control flow
        if(g_gameId >= AITD3)
        {
            printf("\t\tCam #%i:\n", i);
            char buffer[256];

            if(g_gameId == AITD3)
            {
                sprintf(buffer,"CAM%02d",floorNumber);
            }
            else
            {
                sprintf(buffer,"CAMSAL%02d",floorNumber);
            }

            offset = 0;
            g_currentFloorCameraRawDataSize = 1;
            currentCameraData = (unsigned char*)CheckLoadMallocPak(buffer,i);
        }
        else
        {
            printf("\t\tCam #%i (w/ Offset: %lu (Raw: %lu)):\n", i, READ_LE_U32(g_currentFloorCameraRawData + i * 4), *(u32*)(g_currentFloorCameraRawData + i * 4));
            offset = READ_LE_U32(g_currentFloorCameraRawData + i * 4);
            printf("\t\tCam #%i (w/ Offset: %u (Raw: %lu)):\n", i, offset, *(u32*)(g_currentFloorCameraRawData + i * 4));
        }

        // load cameras
        if(offset<g_currentFloorCameraRawDataSize)
        {
            unsigned char* backupDataPtr;

            // TODO: Improve this control flow
            if(g_gameId<AITD3)
            {
                // TODO: Just recycle offset
                // currentCameraData = (unsigned char*)(g_currentFloorCameraRawData + offset);
                currentCameraData = (unsigned char*)(g_currentFloorCameraRawData + READ_LE_U32(g_currentFloorCameraRawData + i * 4));
            }

            backupDataPtr = currentCameraData;

            printf("\t\t\talpha:%hi (Raw: %hu)\n", READ_LE_S16(currentCameraData + 0x00), *(s16*)(currentCameraData + 0x00));
            g_currentFloorCameraData[i].alpha = READ_LE_U16(currentCameraData + 0x00);
            // printf("\t\t\talpha:%hi (Raw: %hu)\n", g_currentFloorCameraData[i].alpha, *(s16*)(currentCameraData + 0x00));
            printf("\t\t\tbeta:%hi (Raw: %hu)\n", READ_LE_S16(currentCameraData + 0x02), *(s16*)(currentCameraData + 0x02));
            g_currentFloorCameraData[i].beta = READ_LE_U16(currentCameraData + 0x02);
            // printf("\t\t\tbeta:%hi (Raw: %hu)\n", g_currentFloorCameraData[i].beta, *(s16*)(currentCameraData + 0x02));
            printf("\t\t\tgamma:%hi (Raw: %hu)\n", READ_LE_S16(currentCameraData + 0x04), *(s16*)(currentCameraData + 0x04));
            g_currentFloorCameraData[i].gamma = READ_LE_U16(currentCameraData + 0x04);
            // printf("\t\t\tgamma:%hi (Raw: %hu)\n", g_currentFloorCameraData[i].gamma, *(s16*)(currentCameraData + 0x04));

            printf("\t\t\tx:%hi (Raw: %hu)\n", READ_LE_S16(currentCameraData + 0x06), *(s16*)(currentCameraData + 0x06));
            g_currentFloorCameraData[i].x = READ_LE_U16(currentCameraData+0x06);
            // printf("\t\t\tx:%hi (Raw: %hu)\n", g_currentFloorCameraData[i].x, *(s16*)(currentCameraData + 0x06));
            printf("\t\t\ty:%hi (Raw: %hu)\n", READ_LE_S16(currentCameraData + 0x08), *(s16*)(currentCameraData + 0x08));
            g_currentFloorCameraData[i].y = READ_LE_U16(currentCameraData+0x08);
            // printf("\t\t\ty:%hi (Raw: %hu)\n", g_currentFloorCameraData[i].y, *(s16*)(currentCameraData + 0x08));
            printf("\t\t\tz:%hi (Raw: %hu)\n", READ_LE_S16(currentCameraData + 0x0A), *(s16*)(currentCameraData + 0x0A));
            g_currentFloorCameraData[i].z = READ_LE_U16(currentCameraData+0x0A);
            // printf("\t\t\tz:%hi (Raw: %hu)\n", g_currentFloorCameraData[i].z, *(s16*)(currentCameraData + 0x0A));

            printf("\t\t\tfocal1:%hi (Raw: %hu)\n", READ_LE_S16(currentCameraData + 0x0C), *(s16*)(currentCameraData + 0x0C));
            g_currentFloorCameraData[i].focal1 = READ_LE_U16(currentCameraData+0x0C);
            // printf("\t\t\tfocal1:%hi (Raw: %hu)\n", g_currentFloorCameraData[i].focal1, *(s16*)(currentCameraData + 0x0C));
            printf("\t\t\tfocal2:%hi (Raw: %hu)\n", READ_LE_S16(currentCameraData + 0x0E), *(s16*)(currentCameraData + 0x0E));
            g_currentFloorCameraData[i].focal2 = READ_LE_U16(currentCameraData+0x0E);
            // printf("\t\t\tfocal2:%hi (Raw: %hu)\n", g_currentFloorCameraData[i].focal2, *(s16*)(currentCameraData + 0x0E));
            printf("\t\t\tfocal3:%hi (Raw: %hu)\n", READ_LE_S16(currentCameraData + 0x10), *(s16*)(currentCameraData + 0x10));
            g_currentFloorCameraData[i].focal3 = READ_LE_U16(currentCameraData+0x10);
            // printf("\t\t\tfocal3:%hi (Raw: %hu)\n", g_currentFloorCameraData[i].focal3, *(s16*)(currentCameraData + 0x10));

            printf("\t\t\tnumViewedRooms:%hi (Raw: %hu)\n", READ_LE_S16(currentCameraData + 0x12), *(s16*)(currentCameraData + 0x12));
            g_currentFloorCameraData[i].numViewedRooms = READ_LE_U16(currentCameraData+0x12);
            // printf("\t\t\tnumViewedRooms:%hi (Raw: %hu)\n", g_currentFloorCameraData[i].numViewedRooms, *(s16*)(currentCameraData + 0x12));

            currentCameraData+=0x14;

            g_currentFloorCameraData[i].viewedRoomTable = (cameraViewedRoomStruct*)malloc(sizeof(cameraViewedRoomStruct)*g_currentFloorCameraData[i].numViewedRooms);
            ASSERT(g_currentFloorCameraData[i].viewedRoomTable);
			memset(g_currentFloorCameraData[i].viewedRoomTable, 0, sizeof(cameraViewedRoomStruct)*g_currentFloorCameraData[i].numViewedRooms);

            for(k=0;k<g_currentFloorCameraData[i].numViewedRooms;k++)
            {
                cameraViewedRoomStruct* pCurrentCameraViewedRoom;

                pCurrentCameraViewedRoom = &g_currentFloorCameraData[i].viewedRoomTable[k];

                printf("\n\t\t\t\tViewed Room #%i:\n", k);
                printf("\t\t\t\tviewedRoomIdx: %hi (Raw: %hu)\n", READ_LE_U16(currentCameraData + 0x00), *(u16*)(currentCameraData + 0x00));
                pCurrentCameraViewedRoom->viewedRoomIdx = READ_LE_U16(currentCameraData+0x00);
                // printf("\t\t\t\tviewedRoomIdx: %hi (Raw: %hu)\n", pCurrentCameraViewedRoom->viewedRoomIdx, *(u16*)(currentCameraData + 0x00));
                printf("\t\t\t\toffsetToMask: %hi (Raw: %hu)\n", READ_LE_U16(currentCameraData + 0x02), *(u16*)(currentCameraData + 0x02));
                pCurrentCameraViewedRoom->offsetToMask = READ_LE_U16(currentCameraData+0x02);
                // printf("\t\t\t\toffsetToMask: %hi (Raw: %hu)\n", pCurrentCameraViewedRoom->offsetToMask, *(u16*)(currentCameraData + 0x02));
                printf("\t\t\t\toffsetToCover: %hi (Raw: %hu)\n", READ_LE_U16(currentCameraData + 0x04), *(u16*)(currentCameraData + 0x04));
                pCurrentCameraViewedRoom->offsetToCover = READ_LE_U16(currentCameraData+0x04);
                // printf("\t\t\t\toffsetToCover: %hi (Raw: %hu)\n", pCurrentCameraViewedRoom->offsetToCover, *(u16*)(currentCameraData + 0x04));

				if(g_gameId == AITD1)
				{
					printf("\t\t\t\toffsetToHybrids: %hi (Raw: %hu)\n", 0, 0);
					pCurrentCameraViewedRoom->offsetToHybrids = 0;
					// printf("\t\t\t\toffsetToHybrids: %hi (Raw: %hu)\n", pCurrentCameraViewedRoom->offsetToHybrids, 0);
					printf("\t\t\t\toffsetCamOptims: %hi (Raw: %hu)\n", 0, 0);
					pCurrentCameraViewedRoom->offsetCamOptims = 0;
					// printf("\t\t\t\toffsetCamOptims: %hi (Raw: %hu)\n", pCurrentCameraViewedRoom->offsetCamOptims, 0);
					printf("\t\t\t\tlightX: %hi (Raw: %hu)\n", READ_LE_U16(currentCameraData + 0x06), *(u16*)(currentCameraData + 0x06));
					pCurrentCameraViewedRoom->lightX = READ_LE_U16(currentCameraData+0x06);
					// printf("\t\t\t\tlightX: %hi (Raw: %hu)\n", pCurrentCameraViewedRoom->lightX, *(u16 *)(currentCameraData + 0x06));
					printf("\t\t\t\tlightY: %hi (Raw: %hu)\n", READ_LE_U16(currentCameraData + 0x08), *(u16*)(currentCameraData + 0x08));
					pCurrentCameraViewedRoom->lightY = READ_LE_U16(currentCameraData+0x08);
					// printf("\t\t\t\tlightY: %hi (Raw: %hu)\n", pCurrentCameraViewedRoom->lightY, *(u16 *)(currentCameraData + 0x08));
					printf("\t\t\t\tlightZ: %hi (Raw: %hu)\n", READ_LE_U16(currentCameraData + 0x0A), *(u16*)(currentCameraData + 0x0A));
					pCurrentCameraViewedRoom->lightZ = READ_LE_U16(currentCameraData+0x0A);
					// printf("\t\t\t\tlightZ: %hi (Raw: %hu)\n", pCurrentCameraViewedRoom->lightZ, *(u16 *)(currentCameraData + 0x0A));
				}
				else
                {
                    printf("\t\t\t\toffsetToHybrids: %hi (Raw: %hu)\n", READ_LE_U16(currentCameraData + 0x06), *(u16*)(currentCameraData + 0x06));
                    pCurrentCameraViewedRoom->offsetToHybrids = READ_LE_U16(currentCameraData+0x06);
                    // printf("\t\t\t\toffsetToHybrids: %hi (Raw: %hu)\n", pCurrentCameraViewedRoom->offsetToHybrids, *(u16 *)(currentCameraData + 0x06));
                    printf("\t\t\t\toffsetCamOptims: %hi (Raw: %hu)\n", READ_LE_U16(currentCameraData + 0x08), *(u16*)(currentCameraData + 0x08));
                    pCurrentCameraViewedRoom->offsetCamOptims = READ_LE_U16(currentCameraData+0x08);
                    // printf("\t\t\t\toffsetCamOptims: %hi (Raw: %hu)\n", pCurrentCameraViewedRoom->offsetCamOptims, *(u16 *)(currentCameraData + 0x08));
                    printf("\t\t\t\tlightX: %hi (Raw: %hu)\n", READ_LE_U16(currentCameraData + 0x0A), *(u16*)(currentCameraData + 0x0A));
					pCurrentCameraViewedRoom->lightX = READ_LE_U16(currentCameraData+0x0A);
                    // printf("\t\t\t\tlightX: %hi (Raw: %hu)\n", pCurrentCameraViewedRoom->lightX, *(u16 *)(currentCameraData + 0x0A));
                    printf("\t\t\t\tlightY: %hi (Raw: %hu)\n", READ_LE_U16(currentCameraData + 0x0C), *(u16*)(currentCameraData + 0x0C));
					pCurrentCameraViewedRoom->lightY = READ_LE_U16(currentCameraData+0x0C);
					// printf("\t\t\t\tlightY: %hi (Raw: %hu)\n", pCurrentCameraViewedRoom->lightY, *(u16 *)(currentCameraData + 0x0C));
					printf("\t\t\t\tlightZ: %hi (Raw: %hu)\n", READ_LE_U16(currentCameraData + 0x0E), *(u16*)(currentCameraData + 0x0E));
					pCurrentCameraViewedRoom->lightZ = READ_LE_U16(currentCameraData+0x0E);
					// printf("\t\t\t\tlightZ: %hi (Raw: %hu)\n", pCurrentCameraViewedRoom->lightZ, *(u16 *)(currentCameraData + 0x0E));
                }

				// load camera mask
				unsigned char* pMaskData = NULL;
				// TODO: Only AITD1 doesn't use masks; use `g_gameId > AITD1` or `g_gameId != AITD1`
				if(g_gameId >= JACK)
				{
					pMaskData = backupDataPtr + g_currentFloorCameraData[i].viewedRoomTable[k].offsetToMask;

					// for this camera, how many masks zone
					printf("\t\t\t\tnumMask: %hu (Raw: %hu)\n", READ_LE_U16(pMaskData), *(u16*)(pMaskData));
					pCurrentCameraViewedRoom->numMask = READ_LE_U16(pMaskData);
					// printf("\t\t\t\tnumMask: %hu (Raw: %hu)\n", pCurrentCameraViewedRoom->numMask, *(u16 *)(pMaskData));
					pMaskData+=2;

					pCurrentCameraViewedRoom->masks = (cameraMaskStruct*)malloc(sizeof(cameraMaskStruct)*pCurrentCameraViewedRoom->numMask);
					memset(pCurrentCameraViewedRoom->masks, 0, sizeof(cameraMaskStruct)*pCurrentCameraViewedRoom->numMask);

					for(int k=0; k<pCurrentCameraViewedRoom->numMask; k++)
					{
						cameraMaskStruct* pCurrentCameraMask = &pCurrentCameraViewedRoom->masks[k];

						// for this overlay zone, how many 
						printf("\t\t\t\tMask #%i (%hu numTestRect (Raw: %hu)):\n", k, READ_LE_U16(pMaskData), *(u16*)pMaskData);
						pCurrentCameraMask->numTestRect = READ_LE_U16(pMaskData);
						// printf("\t\t\t\tMask #%i (%hu numTestRect (Raw: %hu)): \n", k, pCurrentCameraMask->numTestRect, *(u16 *)pMaskData);
						pMaskData+=2;

						pCurrentCameraMask->rectTests = (rectTestStruct*)malloc(sizeof(rectTestStruct)*pCurrentCameraMask->numTestRect);
						memset(pCurrentCameraMask->rectTests, 0, sizeof(rectTestStruct)*pCurrentCameraMask->numTestRect);

						for(int j=0; j<pCurrentCameraMask->numTestRect; j++)
						{
							rectTestStruct* pCurrentRectTest = &pCurrentCameraMask->rectTests[j];

							printf("\t\t\t\t\tzoneX1: %hi (Raw: %hi)\n", READ_LE_S16(pMaskData + 0), *(s16*)(pMaskData + 0));
							pCurrentRectTest->zoneX1 = READ_LE_S16(pMaskData);
							// printf("\t\t\t\t\tzoneX1: %hi (Raw: %hi)\n", pCurrentRectTest->zoneX1, *(s16 *)(pMaskData + 0));
							printf("\t\t\t\t\tzoneZ1: %hi (Raw: %hi)\n", READ_LE_S16(pMaskData + 2), *(s16*)(pMaskData + 2));
							pCurrentRectTest->zoneZ1 = READ_LE_S16(pMaskData+2);
							// printf("\t\t\t\t\tzoneZ1: %hi (Raw: %hi)\n", pCurrentRectTest->zoneZ1, *(s16 *)(pMaskData + 2));
							printf("\t\t\t\t\tzoneX2: %hi (Raw: %hi)\n", READ_LE_S16(pMaskData + 4), *(s16*)(pMaskData + 4));
							pCurrentRectTest->zoneX2 = READ_LE_S16(pMaskData+4);
							// printf("\t\t\t\t\tzoneX2: %hi (Raw: %hi)\n", pCurrentRectTest->zoneX2, *(s16 *)(pMaskData + 4));
							printf("\t\t\t\t\tzoneZ2: %hi (Raw: %hi)\n", READ_LE_S16(pMaskData + 6), *(s16*)(pMaskData + 6));
							pCurrentRectTest->zoneZ2 = READ_LE_S16(pMaskData+6);
							// printf("\t\t\t\t\tzoneZ2: %hi (Raw: %hi)\n", pCurrentRectTest->zoneZ2, *(s16 *)(pMaskData + 6));
							pMaskData+=8;
						}
					}
				}

                // load camera cover
                {
                    unsigned char* pZoneData;
                    int numZones;
                    int j;

                    pZoneData = backupDataPtr + g_currentFloorCameraData[i].viewedRoomTable[k].offsetToCover;
					if(pMaskData)
					{
						assert(pZoneData == pMaskData);
					}
                    //pZoneData = currentCameraData;

                    pCurrentCameraViewedRoom->numCoverZones = numZones =READ_LE_U16(pZoneData);
                    pZoneData+=2;

                    pCurrentCameraViewedRoom->coverZones = (cameraZoneEntryStruct*)malloc(sizeof(cameraZoneEntryStruct)*numZones);

                    ASSERT(pCurrentCameraViewedRoom->coverZones);

                    for(j=0;j<pCurrentCameraViewedRoom->numCoverZones;j++)
                    {
                        int pointIdx;
                        int numPoints;

                        printf("\t\t\t\t\tCover Zone #%i w/ %hu points (Raw: %hu):\n", j, READ_LE_U16(pZoneData), *(u16*)pZoneData);
                        pCurrentCameraViewedRoom->coverZones[j].numPoints = numPoints = READ_LE_U16(pZoneData);
                        // printf("\t\t\t\t\tCover Zone #%i w/ %hu points (Raw: %hu):\n", j, pCurrentCameraViewedRoom->coverZones[j].numPoints, *(u16*)pZoneData);
                        pZoneData+=2;

                        // We need to copy first point to last position, so oversize by 1
                        pCurrentCameraViewedRoom->coverZones[j].pointTable = (cameraZonePointStruct*)malloc(sizeof(cameraZonePointStruct)*(numPoints+1));

                        for(pointIdx = 0; pointIdx < pCurrentCameraViewedRoom->coverZones[j].numPoints; pointIdx++)
                        {
                            printf("\t\t\t\t\t\tx: %hi (Raw: %hi)\n", READ_LE_S16(pZoneData), *(s16*)pZoneData);
                            pCurrentCameraViewedRoom->coverZones[j].pointTable[pointIdx].x = READ_LE_U16(pZoneData);
                            // printf("\t\t\t\t\t\tx: %hi (Raw: %hi)\n", pCurrentCameraViewedRoom->coverZones[j].pointTable[pointIdx].x, *(s16*)pZoneData);
                            pZoneData+=2;
                            printf("\t\t\t\t\t\ty: %hi (Raw: %hi)\n", READ_LE_S16(pZoneData), *(s16*)pZoneData);
                            pCurrentCameraViewedRoom->coverZones[j].pointTable[pointIdx].y = READ_LE_U16(pZoneData);
                            // printf("\t\t\t\t\t\ty: %hi (Raw: %hi)\n", pCurrentCameraViewedRoom->coverZones[j].pointTable[pointIdx].y, *(s16*)pZoneData);
                            pZoneData+=2;
                        }

                        // copy first point to last position
                        pCurrentCameraViewedRoom->coverZones[j].pointTable[numPoints].x = pCurrentCameraViewedRoom->coverZones[j].pointTable[0].x;
                        pCurrentCameraViewedRoom->coverZones[j].pointTable[numPoints].y = pCurrentCameraViewedRoom->coverZones[j].pointTable[0].y;
                    }
                }

                if(g_gameId == AITD1)
                    currentCameraData+=0x0C;
                else
                    currentCameraData+=0x10;

                if(g_gameId == TIMEGATE)
                {
                    currentCameraData+=6;
                }
            }
        }
        else
        {
            break;
        }
    }
    printf("\t%i Cameras on Floor %i, expected %i\n", i, floorNumber, expectedNumberOfCamera);

    g_currentFloorNumCamera = i-1;

    // globalCameraDataTable = (cameraDataStruct*)realloc(globalCameraDataTable,sizeof(cameraDataStruct)*numGlobalCamera);

    /*    roomCameraData+=0x14;

    }*/
}
