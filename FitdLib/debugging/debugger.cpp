#include <vector>
#include "common.h"

#ifdef FITD_DEBUGGER

bool debuggerVar_debugMenuDisplayed = false;

////// debug var used in engine
bool debuggerVar_drawModelZv = false;
bool debuggerVar_drawCameraCoverZone = false;
bool debuggerVar_noHardClip = false;
bool debuggerVar_topCamera = false;
long int debufferVar_topCameraZoom = -4000;

bool debuggerVar_useBlackBG = false;
bool debuggerVar_fastForward = false;
///////////////////////////////

u8 debugger_mainDebugButtonVar_toggleDrawModelZv = 0;
u8 debugger_mainDebugButtonVar_toggleDrawCameraCoverZone = 0;

void SaveScene()
{
	std::vector<float> vertices;
	std::vector<int> indices;

	int numRooms = getNumberOfRoom();
	for (int i = 0; i < numRooms; i++) {
		roomDataStruct* pRoomData = &roomDataTable[i];

		for (int j = 0; j < pRoomData->numHardCol; j++) {
			hardColStruct* pHardCol = &pRoomData->hardColTable[j];

			float ZVX1 = ((pHardCol->zv.ZVX1 + pRoomData->worldX * 10));
			float ZVX2 = ((pHardCol->zv.ZVX2 + pRoomData->worldX * 10));
			float ZVY1 = -((pHardCol->zv.ZVY1 - pRoomData->worldY * 10));
			float ZVY2 = -((pHardCol->zv.ZVY2 - pRoomData->worldY * 10));
			float ZVZ1 = -((pHardCol->zv.ZVZ1 - pRoomData->worldZ * 10));
			float ZVZ2 = -((pHardCol->zv.ZVZ2 - pRoomData->worldZ * 10));

			int firstIndex = vertices.size() / 3 + 1;
			vertices.push_back(ZVX1); vertices.push_back(ZVY1); vertices.push_back(ZVZ1);
			vertices.push_back(ZVX2); vertices.push_back(ZVY1); vertices.push_back(ZVZ1);
			vertices.push_back(ZVX2); vertices.push_back(ZVY1); vertices.push_back(ZVZ2);
			vertices.push_back(ZVX1); vertices.push_back(ZVY1); vertices.push_back(ZVZ2);

			vertices.push_back(ZVX1); vertices.push_back(ZVY2); vertices.push_back(ZVZ1);
			vertices.push_back(ZVX1); vertices.push_back(ZVY2); vertices.push_back(ZVZ2);
			vertices.push_back(ZVX2); vertices.push_back(ZVY2); vertices.push_back(ZVZ2);
			vertices.push_back(ZVX2); vertices.push_back(ZVY2); vertices.push_back(ZVZ1);

			indices.push_back(firstIndex + 0);
			indices.push_back(firstIndex + 1);
			indices.push_back(firstIndex + 2);
			indices.push_back(firstIndex + 3);

			indices.push_back(firstIndex + 4);
			indices.push_back(firstIndex + 5);
			indices.push_back(firstIndex + 6);
			indices.push_back(firstIndex + 7);

			indices.push_back(firstIndex + 3);
			indices.push_back(firstIndex + 2);
			indices.push_back(firstIndex + 6);
			indices.push_back(firstIndex + 5);

			indices.push_back(firstIndex + 0);
			indices.push_back(firstIndex + 3);
			indices.push_back(firstIndex + 5);
			indices.push_back(firstIndex + 4);

			indices.push_back(firstIndex + 0);
			indices.push_back(firstIndex + 4);
			indices.push_back(firstIndex + 7);
			indices.push_back(firstIndex + 1);

			indices.push_back(firstIndex + 1);
			indices.push_back(firstIndex + 7);
			indices.push_back(firstIndex + 6);
			indices.push_back(firstIndex + 2);
		}
	}

	FILE* fhandle = fopen("output.obj", "w+");

	for (int i = 0; i < vertices.size(); i += 3) {
		fprintf(fhandle, "v %f %f %f\n", -vertices[i + 0], vertices[i + 2], vertices[i + 1]);
	}

	for (int i = 0; i < indices.size(); i += 4) {
		fprintf(fhandle, "f %d %d %d %d\n", indices[i + 0], indices[i + 1], indices[i + 2], indices[i + 3]);
	}

	fclose(fhandle);

}

#ifdef USE_IMGUI
void InputS16(const char* name, s16* value)
{
	int intValue = *value;

	ImGui::InputInt(name, &intValue);

	*value = intValue;
}
#endif

void debugger_draw(void)
{
#ifdef USE_IMGUI
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("Debug")) {
			ImGui::MenuItem("No Collisions", nullptr, &debuggerVar_noHardClip);
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	if (debuggerVar_debugMenuDisplayed) {
		cameraDataStruct* pCamera = cameraDataTable[currentCamera];

		ImGui::Begin("Camera");
		if (pCamera) {
			ImGui::BeginGroup();
			ImGui::PushID("Position");
			ImGui::InputInt("X", &translateX);
			ImGui::InputInt("Y", &translateY);
			ImGui::InputInt("Z", &translateZ);
			ImGui::PopID();
			ImGui::EndGroup();

			ImGui::BeginGroup();
			ImGui::PushID("Center");
			InputS16("Pitch", &pCamera->alpha);
			InputS16("Yaw", &pCamera->beta);
			InputS16("Roll", &pCamera->gamma);
			ImGui::PopID();
			ImGui::EndGroup();

			SetAngleCamera(pCamera->alpha, pCamera->beta, pCamera->gamma);

			ImGui::BeginGroup();
			ImGui::PushID("Center");
			ImGui::InputInt("HCenter", &cameraCenterX);
			ImGui::InputInt("VCenter", &cameraCenterY);
			ImGui::PopID();
			ImGui::EndGroup();

			ImGui::BeginGroup();
			ImGui::PushID("Projection");
			ImGui::InputInt("Perspective", &cameraPerspective);
			ImGui::InputInt("XFov", &cameraFovX);
			ImGui::InputInt("YFov", &cameraFovY);
			ImGui::PopID();
			ImGui::EndGroup();
		}

		if (ImGui::Button("DumpScene")) {
			SaveScene();
		}

		ImGui::End();

		if (ListWorldObjets.size()) {
			ImGui::Begin("World objects");

			static int selectedWorldObject = 0;
			ImGui::InputInt("Index", &selectedWorldObject);

			ImGui::Separator();

			if (selectedWorldObject > ListWorldObjets.size())
				selectedWorldObject = ListWorldObjets.size() - 1;

			tWorldObject* pWorldObject = &ListWorldObjets[selectedWorldObject];

			if (pWorldObject) {
				InputS16("objectIndex", &pWorldObject->objIndex);
				InputS16("body", &pWorldObject->body);
				InputS16("flags", &pWorldObject->flags);
				InputS16("typeZV", &pWorldObject->typeZV);
				InputS16("foundBody", &pWorldObject->foundBody);
				InputS16("foundName", &pWorldObject->foundName);
				InputS16("flags2", &pWorldObject->flags2);
				InputS16("foundLife", &pWorldObject->foundLife);
				InputS16("x", &pWorldObject->x);
				InputS16("y", &pWorldObject->y);
				InputS16("z", &pWorldObject->z);
				InputS16("alpha", &pWorldObject->alpha);
				InputS16("beta", &pWorldObject->beta);
				InputS16("gamma", &pWorldObject->gamma);
				InputS16("stage", &pWorldObject->stage);
				InputS16("room", &pWorldObject->room);
				InputS16("lifeMode", &pWorldObject->lifeMode);
				InputS16("life", &pWorldObject->life);
				InputS16("floorLife", &pWorldObject->floorLife);
				InputS16("anim", &pWorldObject->anim);
				InputS16("frame", &pWorldObject->frame);
				InputS16("animType", &pWorldObject->animType);
				InputS16("animInfo", &pWorldObject->animInfo);
				InputS16("trackMode", &pWorldObject->trackMode);
				InputS16("trackNumber", &pWorldObject->trackNumber);
				InputS16("positionInTrack", &pWorldObject->positionInTrack);
				InputS16("mark", &pWorldObject->mark);
			}

			ImGui::End();
		}

		{
			ImGui::Begin("Active objects");

			static int selectedObject = 0;
			ImGui::InputInt("Index", &selectedObject);

			//ImGui::Separator();

			if (selectedObject > NUM_MAX_OBJECT)
				selectedObject = NUM_MAX_OBJECT - 1;

			tObject* pObject = &objectTable[selectedObject];

			ImGui::PushItemWidth(100);

			InputS16("world index", &pObject->indexInWorld); ImGui::SameLine();
			InputS16("bodyNum", &pObject->bodyNum);
			//            InputS16("_flags", &pObject->_flags);
			InputS16("dynFlags", &pObject->dynFlags);
			//ZVStruct zv;
			InputS16("screenXMin", &pObject->screenXMin); ImGui::SameLine();
			InputS16("screenYMin", &pObject->screenYMin); ImGui::SameLine();
			InputS16("screenXMax", &pObject->screenXMax); ImGui::SameLine();
			InputS16("screenYMax", &pObject->screenYMax);

			InputS16("roomX", &pObject->roomX); ImGui::SameLine();
			InputS16("roomY", &pObject->roomY); ImGui::SameLine();
			InputS16("roomZ", &pObject->roomZ); ImGui::SameLine();

			InputS16("worldX", &pObject->worldX);
			InputS16("worldY", &pObject->worldY);
			InputS16("worldZ", &pObject->worldZ);

			InputS16("alpha", &pObject->alpha);
			InputS16("beta", &pObject->beta);
			InputS16("gamma", &pObject->gamma);

			InputS16("stage", &pObject->stage);
			InputS16("room", &pObject->room);

			InputS16("lifeMode", &pObject->lifeMode);
			InputS16("life", &pObject->life);
			//unsigned int CHRONO;
			//unsigned int ROOM_CHRONO;
			InputS16("ANIM", &pObject->ANIM);
			InputS16("animType", &pObject->animType);
			InputS16("animInfo", &pObject->animInfo);
			InputS16("newAnim", &pObject->newAnim);
			InputS16("newAnimType", &pObject->newAnimType);
			InputS16("newAnimInfo", &pObject->newAnimInfo);
			InputS16("FRAME", &pObject->FRAME);
			InputS16("numOfFrames", &pObject->numOfFrames);
			InputS16("END_FRAME", &pObject->END_FRAME);
			InputS16("END_ANIM", &pObject->END_ANIM);
			InputS16("trackMode", &pObject->trackMode);
			InputS16("trackNumber", &pObject->trackNumber);
			InputS16("MARK", &pObject->MARK);
			InputS16("positionInTrack", &pObject->positionInTrack);

			InputS16("stepX", &pObject->stepX);
			InputS16("stepY", &pObject->stepY);
			InputS16("stepZ", &pObject->stepZ);

			InputS16("animNegX", &pObject->animNegX);
			InputS16("animNegY", &pObject->animNegY);
			InputS16("animNegZ", &pObject->animNegZ);

			//interpolatedValue YHandler;
			InputS16("falling", &pObject->falling);
			//interpolatedValue rotate;
			InputS16("direction", &pObject->direction);
			InputS16("speed", &pObject->speed);
			//interpolatedValue speedChange;
			//s16 COL[3];
			InputS16("COL_BY", &pObject->COL_BY);
			InputS16("HARD_DEC", &pObject->HARD_DEC);
			InputS16("HARD_COL", &pObject->HARD_COL);
			InputS16("HIT", &pObject->HIT);
			InputS16("HIT_BY", &pObject->HIT_BY);
			InputS16("animActionType", &pObject->animActionType);
			InputS16("animActionANIM", &pObject->animActionANIM);
			InputS16("animActionFRAME", &pObject->animActionFRAME);
			InputS16("animActionParam", &pObject->animActionParam);
			InputS16("hitForce", &pObject->hitForce);
			InputS16("hotPointID", &pObject->hotPointID);
			//point3dStruct hotPoint;
			InputS16("hardMat", &pObject->hardMat);

			ImGui::PopItemWidth();

			ImGui::End();
		}
	}
#endif
}
#endif

#if 1 // Life Script logging
/// @brief Life script numbers to enable output for.
int loggedLifeScripts[_MAX_LOGGED_LIFE_SCRIPTS];
int numLoggedLifeScripts = 0;
#endif

// #if 1 // Replay
// bool enableReplayRecording;
// char* replayEvent[];
// #endif

/// @brief Finds the index of the most-significant bit flag.
/// @param flag The flag to find the index of.
/// @return -1 if more than 1 bit is set
int getBitFlagIndex(unsigned char flag)
{
	int iter = 0;
	while (flag != 0) {
		flag >>= 1;
		iter++;
	}
	return iter;
	// if (flag <= 2) { return (int)flag; }
	// if (flag % 2 == 1) return -1;
	// int iter = 0;
	// float val = flag;
	// while (val > 1) {
	// 	val /= 2;
	// 	iter++;
	// 	// if (val != (val = (int)val)) return -1;
	// }
	// if (val != 0) {
	// 	return -1;
	// } else {
	// 	return iter;
	// }
}

#define MAX_DEBUG_OUTPUT_INDENTS 10

int section = 0;

char indent[MAX_DEBUG_OUTPUT_INDENTS + 1] = "";

debugCategoryEnum resultantCategory = DBO_NONE;
debugCategoryEnum categoryStack[10];

debugLevelEnum defaultLevels = (debugLevelEnum)(debugLevelEnum::DBO_L_ERROR | debugLevelEnum::DBO_L_WARN | debugLevelEnum::DBO_L_INFO);
debugOutputConfig outputConfig;
// #define __DEBUG_parseDebugParam__ // Uncomment to turn on `parseDebugParam` output
void parseDebugParam(int argc, char* argv[])
{
#ifdef __DEBUG_parseDebugParam__
	printf("Count: %i\n", argc);
	printf("sizeof(debugCategoryEnum): %zi\n", sizeof(debugCategoryEnum)); // printf("sizeof(debugCategoryEnum): %zi\n", sizeof(debugCategoryEnum::DBO_NONE));
#endif
	debugCategoryEnum prior = DBO_NONE;
	bool shouldLogLives = false;
	for (int i = 1; i < argc; i++) {
		printf("argv[%i]: %s\n", i, argv[i]);
		char* ptr = argv[i];
		if (shouldLogLives) {
			shouldLogLives = false;
			char* backupLives = ptr;
			char* delimPtr;
			while (ptr != nullptr && *ptr != '\000' && numLoggedLifeScripts < _MAX_LOGGED_LIFE_SCRIPTS) {
				backupLives = ptr;
				ptr = strpbrk(ptr, ",");
				if (ptr != nullptr) *(ptr++) = '\000';
				loggedLifeScripts[numLoggedLifeScripts++] = atoi(backupLives);
			}
			continue;
		}
		if (ptr[0] == '-') { ptr++; }
		if (prior != DBO_NONE &&
			ptr[0] >= '0' && ptr[0] <= '9' &&
			getBitFlagIndex(prior) != 0) {
			((debugLevelEnum*)&outputConfig)[getBitFlagIndex(prior)] = (debugLevelEnum)(DBO_L_ALL & atoi(ptr));
			printf("Value: %i\n", DBO_L_ALL & atoi(ptr));
			continue;
		}
		if (0 == strcasecmp("pak", ptr)) {
			prior = debugCategoryEnum::DBO_PAK;
			outputConfig.verbosity_pak = defaultLevels;
		} else if (0 == strcasecmp("floor", ptr)) {
			prior = debugCategoryEnum::DBO_FLOOR;
			outputConfig.verbosity_floor = defaultLevels;
		} else if (0 == strcasecmp("mask", ptr)) {
			prior = debugCategoryEnum::DBO_MASK;
			outputConfig.verbosity_mask = defaultLevels;
		} else if (0 == strcasecmp("camera", ptr)) {
			prior = debugCategoryEnum::DBO_CAMERA;
			outputConfig.verbosity_camera = defaultLevels;
		} else if (0 == strcasecmp("sound", ptr)) {
			prior = debugCategoryEnum::DBO_SOUND;
			outputConfig.verbosity_sound = defaultLevels;
		} else if (0 == strcasecmp("itd", ptr)) {
			prior = debugCategoryEnum::DBO_ITD;
			outputConfig.verbosity_itd = defaultLevels;
		} else if (0 == strcasecmp("life", ptr)) {
			prior = debugCategoryEnum::DBO_LIFE;
			outputConfig.verbosity_life = defaultLevels;
		} else {
			prior = DBO_NONE;
			if (strcasecmp("logLife", ptr) == 0) {
				shouldLogLives = true;
				continue;
			}
		}
#ifdef __DEBUG_parseDebugParam__
		printf("Selected: %hhu\n", prior);
		// printf("Levels: %hhu\n", ((debugLevelEnum*)&outputConfig)[prior * sizeof(debugCategoryEnum)]);
		// printf("Before: %hhu\n", outputConfig.debugOutputEnabled);
#endif
		outputConfig.debugOutputEnabled = (debugCategoryEnum)(outputConfig.debugOutputEnabled | prior);
#ifdef __DEBUG_parseDebugParam__
		// printf("After: %hhu\n", outputConfig.debugOutputEnabled);
#endif
	}
#ifdef __DEBUG_parseDebugParam__
	printf("rc: %hhu, section: %i, categoryStack[section]: %hhi\n", (unsigned char)resultantCategory, section, (u8)categoryStack[section]);
	DebugBeginSection(debugCategoryEnum::DBO_FLOOR);
	printf("rc: %hhu, section: %i, categoryStack[section - 1]: %hhi\n", (unsigned char)resultantCategory, section, (u8)categoryStack[section - 1]);
	DebugPrintfLn(debugLevelEnum::DBO_L_DEBUG, "Sample Output for DBO_L_DEBUG");
	DebugPrintfLn(debugLevelEnum::DBO_L_ERROR, "Sample Output for DBO_L_ERROR");
	DebugPrintfLn(debugLevelEnum::DBO_L_INFO, "Sample Output for DBO_L_INFO");
	DebugPrintfLn(debugLevelEnum::DBO_L_LOG, "Sample Output for DBO_L_LOG");
	DebugPrintfLn(debugLevelEnum::DBO_L_NONE, "Sample Output for DBO_L_NONE");
	DebugPrintfLn(debugLevelEnum::DBO_L_WARN, "Sample Output for DBO_L_WARN");
	printf("rc: %hhu, section: %i, categoryStack[section - 1]: %hhi\n", (unsigned char)resultantCategory, section, (u8)categoryStack[section - 1]);
	DebugEndSection();
	printf("rc: %hhu, section: %i, categoryStack[section]: %hhi\n", (unsigned char)resultantCategory, section, (u8)categoryStack[section]);

	printf("rc: %hhu, section: %i, categoryStack[section]: %hhi\n", (unsigned char)resultantCategory, section, (u8)categoryStack[section]);
	DebugBeginSection(debugCategoryEnum::DBO_FLOOR);
	printf("rc: %hhu, section: %i, categoryStack[section - 1]: %hhi\n", (unsigned char)resultantCategory, section, (u8)categoryStack[section - 1]);
	DebugPrintfLn(debugLevelEnum::DBO_L_DEBUG, "Sample Output for DBO_L_DEBUG");
	DebugPrintfLn(debugLevelEnum::DBO_L_ERROR, "Sample Output for DBO_L_ERROR");
	DebugPrintfLn(debugLevelEnum::DBO_L_INFO, "Sample Output for DBO_L_INFO");
	DebugPrintfLn(debugLevelEnum::DBO_L_LOG, "Sample Output for DBO_L_LOG");
	DebugPrintfLn(debugLevelEnum::DBO_L_NONE, "Sample Output for DBO_L_NONE");
	DebugPrintfLn(debugLevelEnum::DBO_L_WARN, "Sample Output for DBO_L_WARN");
	DebugBeginSection(debugCategoryEnum::DBO_PAK);
	printf("rc: %hhu, section: %i, categoryStack[section - 1]: %hhi\n", (unsigned char)resultantCategory, section, (u8)categoryStack[section - 1]);
	DebugPrintfLn(debugLevelEnum::DBO_L_DEBUG, "Sample Output for DBO_L_DEBUG");
	DebugPrintfLn(debugLevelEnum::DBO_L_ERROR, "Sample Output for DBO_L_ERROR");
	DebugPrintfLn(debugLevelEnum::DBO_L_INFO, "Sample Output for DBO_L_INFO");
	DebugPrintfLn(debugLevelEnum::DBO_L_LOG, "Sample Output for DBO_L_LOG");
	DebugPrintfLn(debugLevelEnum::DBO_L_NONE, "Sample Output for DBO_L_NONE");
	DebugPrintfLn(debugLevelEnum::DBO_L_WARN, "Sample Output for DBO_L_WARN");
	printf("rc: %hhu, section: %i, categoryStack[section - 1]: %hhi\n", (unsigned char)resultantCategory, section, (u8)categoryStack[section - 1]);
	DebugEndSection();
	printf("rc: %hhu, section: %i, categoryStack[section - 1]: %hhi\n", (unsigned char)resultantCategory, section, (u8)categoryStack[section]);
	DebugPrintfLn(debugLevelEnum::DBO_L_DEBUG, "Sample Output for DBO_L_DEBUG");
	DebugPrintfLn(debugLevelEnum::DBO_L_ERROR, "Sample Output for DBO_L_ERROR");
	DebugPrintfLn(debugLevelEnum::DBO_L_INFO, "Sample Output for DBO_L_INFO");
	DebugPrintfLn(debugLevelEnum::DBO_L_LOG, "Sample Output for DBO_L_LOG");
	DebugPrintfLn(debugLevelEnum::DBO_L_NONE, "Sample Output for DBO_L_NONE");
	DebugPrintfLn(debugLevelEnum::DBO_L_WARN, "Sample Output for DBO_L_WARN");
	DebugEndSection();
	printf("rc: %hhu, section: %i, categoryStack[section]: %hhi\n", (unsigned char)resultantCategory, section, (u8)categoryStack[section]);
	DebugPrintfLn(debugLevelEnum::DBO_L_WARN, "Sample Output for DBO_L_WARN");
#endif
}

#if 1 // LABELS
char noneLabel[] = "NONE";												// 0b0000'0000
char pakLabel[] = "PAK";												// 0b0000'0001
char floorLabel[] = "FLOOR";												// 0b0000'0010
char pakFloorLabel[] = "PAK & FLOOR";										// 0b0000'0011
char maskLabel[] = "MASK";												// 0b0000'0100
char pakMaskLabel[] = "PAK & MASK";										// 0b0000'0101
char floorMaskLabel[] = "FLOOR & MASK";										// 0b0000'0110
char pakFloorMaskLabel[] = "PAK & FLOOR & MASK";								// 0b0000'0111
char cameraLabel[] = "CAMERA";											// 0b0000'1000
char pakCameraLabel[] = "PAK & CAMERA";										// 0b0000'1001
char floorCameraLabel[] = "FLOOR & CAMERA";									// 0b0000'1010
char pakFloorCameraLabel[] = "PAK & FLOOR & CAMERA";								// 0b0000'1011
char maskCameraLabel[] = "MASK & CAMERA";										// 0b0000'1100
char pakMaskCameraLabel[] = "PAK & MASK & CAMERA";								// 0b0000'1101
char floorMaskCameraLabel[] = "FLOOR & MASK & CAMERA";								// 0b0000'1110
char pakFloorMaskCameraLabel[] = "PAK & FLOOR & MASK & CAMERA";						// 0b0000'1111
char soundLabel[] = "SOUND";												// 0b0001'0000
char soundPakLabel[] = "SOUND & PAK";										// 0b0001'0001
char soundFloorLabel[] = "SOUND & FLOOR";										// 0b0001'0010
char soundPakFloorLabel[] = "SOUND & PAK & FLOOR";								// 0b0001'0011
char soundMaskLabel[] = "SOUND & MASK";										// 0b0001'0100
char soundPakMaskLabel[] = "SOUND & PAK & MASK";								// 0b0001'0101
char soundFloorMaskLabel[] = "SOUND & FLOOR & MASK";								// 0b0001'0110
char soundPakFloorMaskLabel[] = "SOUND & PAK & FLOOR & MASK";						// 0b0001'0111
char soundCameraLabel[] = "SOUND & CAMERA";									// 0b0001'1000
char soundPakCameraLabel[] = "SOUND & PAK & CAMERA";								// 0b0001'1001
char soundFloorCameraLabel[] = "SOUND & FLOOR & CAMERA";							// 0b0001'1010
char soundPakFloorCameraLabel[] = "SOUND & PAK & FLOOR & CAMERA";						// 0b0001'1011
char soundMaskCameraLabel[] = "SOUND & MASK & CAMERA";								// 0b0001'1100
char soundPakMaskCameraLabel[] = "SOUND & PAK & MASK & CAMERA";						// 0b0001'1101
char soundFloorMaskCameraLabel[] = "SOUND & FLOOR & MASK & CAMERA";						// 0b0001'1110
char soundPakFloorMaskCameraLabel[] = "SOUND & PAK & FLOOR & MASK & CAMERA";				// 0b0001'1111
char itdNoneLabel[] = "ITD & NONE";										// 0b0000'0000
char itdPakLabel[] = "ITD & PAK";											// 0b0000'0001
char itdFloorLabel[] = "ITD & FLOOR";										// 0b0000'0010
char itdPakFloorLabel[] = "ITD & PAK & FLOOR";									// 0b0000'0011
char itdMaskLabel[] = "ITD & MASK";										// 0b0000'0100
char itdPakMaskLabel[] = "ITD & PAK & MASK";									// 0b0000'0101
char itdFloorMaskLabel[] = "ITD & FLOOR & MASK";								// 0b0000'0110
char itdPakFloorMaskLabel[] = "ITD & PAK & FLOOR & MASK";							// 0b0000'0111
char itdCameraLabel[] = "ITD & CAMERA";										// 0b0000'1000
char itdPakCameraLabel[] = "ITD & PAK & CAMERA";								// 0b0000'1001
char itdFloorCameraLabel[] = "ITD & FLOOR & CAMERA";								// 0b0000'1010
char itdPakFloorCameraLabel[] = "ITD & PAK & FLOOR & CAMERA";						// 0b0000'1011
char itdMaskCameraLabel[] = "ITD & MASK & CAMERA";								// 0b0000'1100
char itdPakMaskCameraLabel[] = "ITD & PAK & MASK & CAMERA";							// 0b0000'1101
char itdFloorMaskCameraLabel[] = "ITD & FLOOR & MASK & CAMERA";						// 0b0000'1110
char itdPakFloorMaskCameraLabel[] = "ITD & PAK & FLOOR & MASK & CAMERA";					// 0b0000'1111
char itdSoundLabel[] = "ITD & SOUND";										// 0b0001'0000
char itdSoundPakLabel[] = "ITD & SOUND & PAK";									// 0b0001'0001
char itdSoundFloorLabel[] = "ITD & SOUND & FLOOR";								// 0b0001'0010
char itdSoundPakFloorLabel[] = "ITD & SOUND & PAK & FLOOR";							// 0b0001'0011
char itdSoundMaskLabel[] = "ITD & SOUND & MASK";								// 0b0001'0100
char itdSoundPakMaskLabel[] = "ITD & SOUND & PAK & MASK";							// 0b0001'0101
char itdSoundFloorMaskLabel[] = "ITD & SOUND & FLOOR & MASK";						// 0b0001'0110
char itdSoundPakFloorMaskLabel[] = "ITD & SOUND & PAK & FLOOR & MASK";					// 0b0001'0111
char itdSoundCameraLabel[] = "ITD & SOUND & CAMERA";								// 0b0001'1000
char itdSoundPakCameraLabel[] = "ITD & SOUND & PAK & CAMERA";						// 0b0001'1001
char itdSoundFloorCameraLabel[] = "ITD & SOUND & FLOOR & CAMERA";						// 0b0001'1010
char itdSoundPakFloorCameraLabel[] = "ITD & SOUND & PAK & FLOOR & CAMERA";				// 0b0001'1011
char itdSoundMaskCameraLabel[] = "ITD & SOUND & MASK & CAMERA";						// 0b0001'1100
char itdSoundPakMaskCameraLabel[] = "ITD & SOUND & PAK & MASK & CAMERA";					// 0b0001'1101
char itdSoundFloorMaskCameraLabel[] = "ITD & SOUND & FLOOR & MASK & CAMERA";				// 0b0001'1110
char itdSoundPakFloorMaskCameraLabel[] = "ITD & SOUND & PAK & FLOOR & MASK & CAMERA";			// 0b0001'1111
char lifeLabel[] = "LIFE";												// 0b0010'0000
char lifePakLabel[] = "LIFE & PAK";										// 0b0010'0001
char lifeFloorLabel[] = "LIFE & FLOOR";										// 0b0010'0010
char lifePakFloorLabel[] = "LIFE & PAK & FLOOR";								// 0b0010'0011
char lifeMaskLabel[] = "LIFE & MASK";										// 0b0010'0100
char lifePakMaskLabel[] = "LIFE & PAK & MASK";									// 0b0010'0101
char lifeFloorMaskLabel[] = "LIFE & FLOOR & MASK";								// 0b0010'0110
char lifePakFloorMaskLabel[] = "LIFE & PAK & FLOOR & MASK";							// 0b0010'0111
char lifeCameraLabel[] = "LIFE & CAMERA";										// 0b0010'1000
char lifePakCameraLabel[] = "LIFE & PAK & CAMERA";								// 0b0010'1001
char lifeFloorCameraLabel[] = "LIFE & FLOOR & CAMERA";								// 0b0010'1010
char lifePakFloorCameraLabel[] = "LIFE & PAK & FLOOR & CAMERA";						// 0b0010'1011
char lifeMaskCameraLabel[] = "LIFE & MASK & CAMERA";								// 0b0010'1100
char lifePakMaskCameraLabel[] = "LIFE & PAK & MASK & CAMERA";						// 0b0010'1101
char lifeFloorMaskCameraLabel[] = "LIFE & FLOOR & MASK & CAMERA";						// 0b0010'1110
char lifePakFloorMaskCameraLabel[] = "LIFE & PAK & FLOOR & MASK & CAMERA";				// 0b0010'1111
char lifeSoundLabel[] = "LIFE & SOUND";										// 0b0011'0000
char lifeSoundPakLabel[] = "LIFE & SOUND & PAK";								// 0b0011'0001
char lifeSoundFloorLabel[] = "LIFE & SOUND & FLOOR";								// 0b0011'0010
char lifeSoundPakFloorLabel[] = "LIFE & SOUND & PAK & FLOOR";						// 0b0011'0011
char lifeSoundMaskLabel[] = "LIFE & SOUND & MASK";								// 0b0011'0100
char lifeSoundPakMaskLabel[] = "LIFE & SOUND & PAK & MASK";							// 0b0011'0101
char lifeSoundFloorMaskLabel[] = "LIFE & SOUND & FLOOR & MASK";						// 0b0011'0110
char lifeSoundPakFloorMaskLabel[] = "LIFE & SOUND & PAK & FLOOR & MASK";					// 0b0011'0111
char lifeSoundCameraLabel[] = "LIFE & SOUND & CAMERA";								// 0b0011'1000
char lifeSoundPakCameraLabel[] = "LIFE & SOUND & PAK & CAMERA";						// 0b0011'1001
char lifeSoundFloorCameraLabel[] = "LIFE & SOUND & FLOOR & CAMERA";						// 0b0011'1010
char lifeSoundPakFloorCameraLabel[] = "LIFE & SOUND & PAK & FLOOR & CAMERA";				// 0b0011'1011
char lifeSoundMaskCameraLabel[] = "LIFE & SOUND & MASK & CAMERA";						// 0b0011'1100
char lifeSoundPakMaskCameraLabel[] = "LIFE & SOUND & PAK & MASK & CAMERA";				// 0b0011'1101
char lifeSoundFloorMaskCameraLabel[] = "LIFE & SOUND & FLOOR & MASK & CAMERA";				// 0b0011'1110
char lifeSoundPakFloorMaskCameraLabel[] = "LIFE & SOUND & PAK & FLOOR & MASK & CAMERA";		// 0b0011'1111
char lifeItdNoneLabel[] = "LIFE & ITD & NONE";									// 0b0010'0000
char lifeItdPakLabel[] = "LIFE & ITD & PAK";									// 0b0010'0001
char lifeItdFloorLabel[] = "LIFE & ITD & FLOOR";								// 0b0010'0010
char lifeItdPakFloorLabel[] = "LIFE & ITD & PAK & FLOOR";							// 0b0010'0011
char lifeItdMaskLabel[] = "LIFE & ITD & MASK";									// 0b0010'0100
char lifeItdPakMaskLabel[] = "LIFE & ITD & PAK & MASK";							// 0b0010'0101
char lifeItdFloorMaskLabel[] = "LIFE & ITD & FLOOR & MASK";							// 0b0010'0110
char lifeItdPakFloorMaskLabel[] = "LIFE & ITD & PAK & FLOOR & MASK";					// 0b0010'0111
char lifeItdCameraLabel[] = "LIFE & ITD & CAMERA";								// 0b0010'1000
char lifeItdPakCameraLabel[] = "LIFE & ITD & PAK & CAMERA";							// 0b0010'1001
char lifeItdFloorCameraLabel[] = "LIFE & ITD & FLOOR & CAMERA";						// 0b0010'1010
char lifeItdPakFloorCameraLabel[] = "LIFE & ITD & PAK & FLOOR & CAMERA";					// 0b0010'1011
char lifeItdMaskCameraLabel[] = "LIFE & ITD & MASK & CAMERA";						// 0b0010'1100
char lifeItdPakMaskCameraLabel[] = "LIFE & ITD & PAK & MASK & CAMERA";					// 0b0010'1101
char lifeItdFloorMaskCameraLabel[] = "LIFE & ITD & FLOOR & MASK & CAMERA";				// 0b0010'1110
char lifeItdPakFloorMaskCameraLabel[] = "LIFE & ITD & PAK & FLOOR & MASK & CAMERA";			// 0b0010'1111
char lifeItdSoundLabel[] = "LIFE & ITD & SOUND";								// 0b0011'0000
char lifeItdSoundPakLabel[] = "LIFE & ITD & SOUND & PAK";							// 0b0011'0001
char lifeItdSoundFloorLabel[] = "LIFE & ITD & SOUND & FLOOR";						// 0b0011'0010
char lifeItdSoundPakFloorLabel[] = "LIFE & ITD & SOUND & PAK & FLOOR";					// 0b0011'0011
char lifeItdSoundMaskLabel[] = "LIFE & ITD & SOUND & MASK";							// 0b0011'0100
char lifeItdSoundPakMaskLabel[] = "LIFE & ITD & SOUND & PAK & MASK";					// 0b0011'0101
char lifeItdSoundFloorMaskLabel[] = "LIFE & ITD & SOUND & FLOOR & MASK";					// 0b0011'0110
char lifeItdSoundPakFloorMaskLabel[] = "LIFE & ITD & SOUND & PAK & FLOOR & MASK";			// 0b0011'0111
char lifeItdSoundCameraLabel[] = "LIFE & ITD & SOUND & CAMERA";						// 0b0011'1000
char lifeItdSoundPakCameraLabel[] = "LIFE & ITD & SOUND & PAK & CAMERA";					// 0b0011'1001
char lifeItdSoundFloorCameraLabel[] = "LIFE & ITD & SOUND & FLOOR & CAMERA";				// 0b0011'1010
char lifeItdSoundPakFloorCameraLabel[] = "LIFE & ITD & SOUND & PAK & FLOOR & CAMERA";			// 0b0011'1011
char lifeItdSoundMaskCameraLabel[] = "LIFE & ITD & SOUND & MASK & CAMERA";				// 0b0011'1100
char lifeItdSoundPakMaskCameraLabel[] = "LIFE & ITD & SOUND & PAK & MASK & CAMERA";			// 0b0011'1101
char lifeItdSoundFloorMaskCameraLabel[] = "LIFE & ITD & SOUND & FLOOR & MASK & CAMERA";		// 0b0011'1110
char lifeItdSoundPakFloorMaskCameraLabel[] = "LIFE & ITD & SOUND & PAK & FLOOR & MASK & CAMERA";	// 0b0011'1111
char* debugCategoryLabels[] = {
	noneLabel,
	pakLabel,
	floorLabel,
	pakFloorLabel,
	maskLabel,
	pakMaskLabel,
	floorMaskLabel,
	pakFloorMaskLabel,
	cameraLabel,
	pakCameraLabel,
	floorCameraLabel,
	pakFloorCameraLabel,
	maskCameraLabel,
	pakMaskCameraLabel,
	floorMaskCameraLabel,
	pakFloorMaskCameraLabel,
	soundLabel,
	soundPakLabel,
	soundFloorLabel,
	soundPakFloorLabel,
	soundMaskLabel,
	soundPakMaskLabel,
	soundFloorMaskLabel,
	soundPakFloorMaskLabel,
	soundCameraLabel,
	soundPakCameraLabel,
	soundFloorCameraLabel,
	soundPakFloorCameraLabel,
	soundMaskCameraLabel,
	soundPakMaskCameraLabel,
	soundFloorMaskCameraLabel,
	soundPakFloorMaskCameraLabel,
	itdNoneLabel,
	itdPakLabel,
	itdFloorLabel,
	itdPakFloorLabel,
	itdMaskLabel,
	itdPakMaskLabel,
	itdFloorMaskLabel,
	itdPakFloorMaskLabel,
	itdCameraLabel,
	itdPakCameraLabel,
	itdFloorCameraLabel,
	itdPakFloorCameraLabel,
	itdMaskCameraLabel,
	itdPakMaskCameraLabel,
	itdFloorMaskCameraLabel,
	itdPakFloorMaskCameraLabel,
	itdSoundLabel,
	itdSoundPakLabel,
	itdSoundFloorLabel,
	itdSoundPakFloorLabel,
	itdSoundMaskLabel,
	itdSoundPakMaskLabel,
	itdSoundFloorMaskLabel,
	itdSoundPakFloorMaskLabel,
	itdSoundCameraLabel,
	itdSoundPakCameraLabel,
	itdSoundFloorCameraLabel,
	itdSoundPakFloorCameraLabel,
	itdSoundMaskCameraLabel,
	itdSoundPakMaskCameraLabel,
	itdSoundFloorMaskCameraLabel,
	itdSoundPakFloorMaskCameraLabel,
};

char debugLabel[] = FormatDleLabel(DLE_C_DEBUG, DEBUG);	// 0b0000'0001
char logLabel[] = FormatDleLabel(DLE_C_LOG, LOG);		// 0b0000'0010
char infoLabel[] = FormatDleLabel(DLE_C_INFO, INFO);		// 0b0000'0100
char warnLabel[] = FormatDleLabel(DLE_C_WARN, WARN);		// 0b0000'1000
char errorLabel[] = FormatDleLabel(DLE_C_ERROR, ERROR);	// 0b0001'0000
char* debugLevelLabels[] = {
	noneLabel,	// 0b0000'0000
	debugLabel,	// 0b0000'0001
	logLabel,	// 0b0000'0010
	logLabel,	// 0b0000'0011
	infoLabel,	// 0b0000'0100
	infoLabel,	// 0b0000'0101
	infoLabel,	// 0b0000'0110
	infoLabel,	// 0b0000'0111
	warnLabel,	// 0b0000'1000
	warnLabel,	// 0b0000'1001
	warnLabel,	// 0b0000'1010
	warnLabel,	// 0b0000'1011
	warnLabel,	// 0b0000'1100
	warnLabel,	// 0b0000'1101
	warnLabel,	// 0b0000'1110
	warnLabel,	// 0b0000'1111
	errorLabel,	// 0b0001'0000
	errorLabel,	// 0b0001'0001
	errorLabel,	// 0b0001'0010
	errorLabel,	// 0b0001'0011
	errorLabel,	// 0b0001'0100
	errorLabel,	// 0b0001'0101
	errorLabel,	// 0b0001'0110
	errorLabel,	// 0b0001'0111
	errorLabel,	// 0b0001'1000
	errorLabel,	// 0b0001'1001
	errorLabel,	// 0b0001'1010
	errorLabel,	// 0b0001'1011
	errorLabel,	// 0b0001'1100
	errorLabel,	// 0b0001'1101
	errorLabel,	// 0b0001'1110
	errorLabel,	// 0b0001'1111
};
#endif

/// @brief 
/// @param category 
/// @param level 
/// @return true if 1 or more enabled categories are included & 1 or more of the included categories have the debug level enabled, false otherwise.
bool _shouldPrint(debugCategoryEnum category, debugLevelEnum level = (debugLevelEnum)DBO_L_ALL)
{
	if (outputConfig.debugOutputEnabled & category) {
		unsigned int flag = DBO_NONE + 1;
		while (flag < DBO_ALL) {
			if (category & flag &&
				((debugLevelEnum*)&outputConfig)[getBitFlagIndex(flag) * sizeof(debugLevelEnum)] & level) {
				return true;
			} else { flag <<= 1; }
		}
	}
	return false;
}
// TODO: Update sectionBeganStack?
debugCategoryEnum DebugAddCategory(debugCategoryEnum category)
{
	category = (debugCategoryEnum)(category & DBO_ALL); // Mask the unused bytes
	if (categoryStack[section - 1] & category == category) {
		return debugCategoryEnum::DBO_NONE;
	}
	// Exclude all values in both
	category = (debugCategoryEnum)(categoryStack[section - 1] & category ^ category);
	categoryStack[section - 1] = (debugCategoryEnum)(categoryStack[section - 1] | category);
	resultantCategory = (debugCategoryEnum)(resultantCategory | category);
	return category;
}
// TODO: Update sectionBeganStack?
void DebugRemoveCategory(debugCategoryEnum category)
{
	category = (debugCategoryEnum)(category & DBO_ALL); // Mask the unused bytes
	categoryStack[section - 1] = (debugCategoryEnum)(categoryStack[section - 1] ^ category);
	// TODO: Do this correctly by iterating through stack and &-ing together; currently will wipe repeated flags.
	// resultantCategory = (debugCategoryEnum)(resultantCategory ^ category);
	resultantCategory = (debugCategoryEnum)(resultantCategory ^ category);
}
bool sectionBeganStack[50] = {};
int sectionBeganIdx = -1;
bool DebugBeginSection(debugCategoryEnum category)
{
	if (sectionBeganIdx == 49) return false;
	category = (debugCategoryEnum)(category & DBO_ALL); // Mask the unused bytes
	sectionBeganIdx++;
	sectionBeganStack[sectionBeganIdx] = section < MAX_DEBUG_OUTPUT_INDENTS && _shouldPrint((debugCategoryEnum)(resultantCategory | category));
	if (sectionBeganStack[sectionBeganIdx]) {
		categoryStack[section] = category;
		resultantCategory = (debugCategoryEnum)(resultantCategory | category);
		indent[section] = '\t';
		++section;
		indent[section] = '\0';
		return true;
	}
	return false;
}

bool DebugEndSection()
{
	// if (sectionBeganIdx == 0) return false;
	if (sectionBeganIdx == -1) return false;
	bool v = sectionBeganStack[sectionBeganIdx];
	sectionBeganStack[sectionBeganIdx] = false; // TODO: Unnecessary?
	sectionBeganIdx--;
	if (section > 0 && (v || _shouldPrint(resultantCategory))) {
		// 1. Decrement section *count*
		--section;
		// 2. Clear indent at prior section *index*
		indent[section] = '\0';
		// 3. Remove category
		// 3A. Remove category from `resultantCategory`
		// TODO: Do this correctly by iterating through stack and &-ing together; currently will wipe repeated flags.
		// resultantCategory = (debugCategoryEnum)(resultantCategory ^ categoryStack[section]);
		resultantCategory = DBO_NONE;
		for (int i = 0; i < section; i++) {
			resultantCategory = (debugCategoryEnum)(resultantCategory | categoryStack[i]);
		}
		// 3B. Remove category from `categoryStack`
		categoryStack[section] = DBO_NONE;
		return true;
	}
	return false;
}

void DebugPrintfLn(debugLevelEnum level, const char* format, ...)
{
	if (_shouldPrint(resultantCategory, level)) {
		va_list argList;
		va_start(argList, format);

		char buff[256];
		vsprintf(buff, format, argList);

		va_end(argList);

		printf("[%s]\t[%s]: %s%s\n", debugLevelLabels[level], debugCategoryLabels[resultantCategory], indent, buff);
	}
}
/// @brief Makes sure the initial line of a function is printed with the proper indent.
/// @param level 
/// @param category 
/// @param format 
/// @param  
void DebugPrintfLnCategory(debugLevelEnum level, debugCategoryEnum category, const char* format, ...)
{
	debugCategoryEnum t = (debugCategoryEnum)(resultantCategory | category);
	if (_shouldPrint(t, level)) {
		va_list argList;
		va_start(argList, format);

		char buff[256];
		vsprintf(buff, format, argList);

		va_end(argList);

		printf("[%s]\t[%s]: %s%s\n", debugLevelLabels[level], debugCategoryLabels[t], indent, buff);
	}
}
char buffer[256];
void DebugBPrintf(debugLevelEnum level, const char* format, ...)
{
	if (_shouldPrint(resultantCategory, level)) {
		va_list argList;
		va_start(argList, format);

		if (buffer[0] != '\000') {
			vsprintf(buffer, format, argList);

			va_end(argList);
			return;
		}
		vsprintf(buffer, format, argList);

		va_end(argList);

		printf("[%s]\t[%s]: %s%s", debugLevelLabels[level], debugCategoryLabels[resultantCategory], indent, buffer);
	}
}
void DebugBFlushLn()
{
	if (buffer[0] != '\000') {
		printf("%s\n", buffer);
		buffer[0] = '\000';
	}
}
#if 1 // DebugBPrintRaw definition
#define ___DEBUG_B_PRINT_RAW {\
	if (_shouldPrint(resultantCategory, level)) {\
		char buff[256];\
		sprintf(buff, "%s (Raw: %s)", typeSpecifier, typeSpecifier);\
		DebugBPrintf(level, buff, result, raw);\
	}\
}
#define ____DEBUG_B_PRINT_RAW__SIGNATURE(T, trailing) template <> void DebugBPrintRaw(debugLevelEnum level, const char* typeSpecifier, T result, T raw) trailing
#define ___DEBUG_B_PRINT_RAW__SIGNATURE(T) ____DEBUG_B_PRINT_RAW__SIGNATURE(T, ___DEBUG_B_PRINT_RAW)
template <typename T> void DebugBPrintRaw(debugLevelEnum level, const char* typeSpecifier, T result, T raw) ___DEBUG_B_PRINT_RAW
___DEBUG_B_PRINT_RAW__SIGNATURE(s8)
___DEBUG_B_PRINT_RAW__SIGNATURE(u8)
___DEBUG_B_PRINT_RAW__SIGNATURE(s16)
___DEBUG_B_PRINT_RAW__SIGNATURE(u16)
___DEBUG_B_PRINT_RAW__SIGNATURE(s32)
___DEBUG_B_PRINT_RAW__SIGNATURE(u32)
#undef ___DEBUG_B_PRINT_RAW
#undef ___DEBUG_B_PRINT_RAW__SIGNATURE
#undef ____DEBUG_B_PRINT_RAW__SIGNATURE
#endif
void DebugSPrintZVStruct(char* destination, ZVStruct& zv)
{
	sprintf(destination, "{ ZVX1: %i, ZVX2: %i, ZVY1: %i, ZVY2: %i, ZVZ1: %i, ZVZ2: %i }", zv.ZVX1, zv.ZVX2, zv.ZVY1, zv.ZVY2, zv.ZVZ1, zv.ZVZ2);
}
// void DebugSPrintZVStruct(char* destination, ZVStruct* zv) {
// 	sprintf(destination, "{ ZVX1: %i, ZVX2: %i, ZVY1: %i, ZVY2: %i, ZVZ1: %i, ZVZ2: %i }", zv->ZVX1, zv->ZVX2, zv->ZVY1, zv->ZVY2, zv->ZVZ1, zv->ZVZ2);
// }
