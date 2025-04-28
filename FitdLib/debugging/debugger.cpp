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

debugLevelEnum defaultLevels = (debugLevelEnum)(debugLevelEnum::DBO_L_ERROR | debugLevelEnum::DBO_L_WARN | debugLevelEnum::DBO_L_INFO);
debugOutputConfig debugOutput;
void parseDebugParam(int argc, char* argv[])
{
	printf("Count: %i\n", argc);
	printf("sizeof(debugCategoryEnum): %zi\n", sizeof(debugCategoryEnum::DBO_NONE));
	debugCategoryEnum prior = DBO_NONE;
	for (int i = 1; i < argc; i++) {
		printf("argv[%i]: %s\n", i, argv[i]);
		char* ptr = argv[i];
		if (ptr[0] == '-') { ptr++; }
		if (prior != DBO_NONE) {
			if (ptr[0] >= '0' && ptr[0] <= '9') {
				debugLevelEnum* relevantLevels = NULL;
				switch (prior) {
					case debugCategoryEnum::DBO_PAK:
					{
						printf("debugCategoryEnum::DBO_PAK\n");
						relevantLevels = (debugLevelEnum*)&(debugOutput.verbosity_pak);
						break;
					}
					case debugCategoryEnum::DBO_FLOOR:
					{
						printf("debugCategoryEnum::DBO_FLOOR\n");
						relevantLevels = (debugLevelEnum*)&(debugOutput.verbosity_floor);
						break;
					}
					case debugCategoryEnum::DBO_MASK:
					{
						printf("debugCategoryEnum::DBO_MASK\n");
						relevantLevels = (debugLevelEnum*)&(debugOutput.verbosity_mask);
						break;
					}
					case debugCategoryEnum::DBO_CAMERA:
					{
						printf("debugCategoryEnum::DBO_CAMERA\n");
						relevantLevels = (debugLevelEnum*)&(debugOutput.verbosity_camera);
						break;
					}
					case debugCategoryEnum::DBO_SOUND:
					{
						printf("debugCategoryEnum::DBO_SOUND\n");
						relevantLevels = (debugLevelEnum*)&(debugOutput.verbosity_sound);
						break;
					}
					case DBO_NONE:
					default:
						break;
				}
				if (relevantLevels != NULL) {
					*relevantLevels = (debugLevelEnum)(0b00000000'00000000'00000000'00000000 | DBO_L_ALL & atoi(ptr));
					printf("Value: %i\n", 0b00000000'00000000'00000000'00000000 | DBO_L_ALL & atoi(ptr));
					continue;
				}
			}
		}
		if (0 == strcmp("pak", ptr) || 0 == strcmp("PAK", ptr)) {
			prior = debugCategoryEnum::DBO_PAK;
			debugOutput.verbosity_pak = defaultLevels;
		} else if (0 == strcmp("floor", ptr) || 0 == strcmp("FLOOR", ptr)) {
			prior = debugCategoryEnum::DBO_FLOOR;
			debugOutput.verbosity_floor = defaultLevels;
		} else if (0 == strcmp("mask", ptr) || 0 == strcmp("MASK", ptr)) {
			prior = debugCategoryEnum::DBO_MASK;
			debugOutput.verbosity_mask = defaultLevels;
		} else if (0 == strcmp("camera", ptr) || 0 == strcmp("CAMERA", ptr)) {
			prior = debugCategoryEnum::DBO_CAMERA;
			debugOutput.verbosity_camera = defaultLevels;
		} else if (0 == strcmp("sound", ptr) || 0 == strcmp("SOUND", ptr)) {
			prior = debugCategoryEnum::DBO_SOUND;
			debugOutput.verbosity_sound = defaultLevels;
		} else {
			prior = DBO_NONE;
		}
		printf("Selected: %hhu\n", prior);
		printf("Levels: %hhu\n", ((debugLevelEnum*)&debugOutput)[prior * sizeof(debugCategoryEnum)]);
		printf("Before: %hhu\n", debugOutput.debugOutputEnabled);
		debugOutput.debugOutputEnabled = (debugCategoryEnum)(debugOutput.debugOutputEnabled | prior);
		printf("After: %hhu\n", debugOutput.debugOutputEnabled);
	}
	DebugAddCategory(debugCategoryEnum::DBO_PAK);
	DebugPrintfLn(debugLevelEnum::DBO_L_DEBUG, "DBO_L_DEBUG");
	DebugPrintfLn(debugLevelEnum::DBO_L_ERROR, "DBO_L_ERROR");
	DebugPrintfLn(debugLevelEnum::DBO_L_INFO, "DBO_L_INFO");
	DebugPrintfLn(debugLevelEnum::DBO_L_LOG, "DBO_L_LOG");
	DebugPrintfLn(debugLevelEnum::DBO_L_NONE, "DBO_L_NONE");
	DebugPrintfLn(debugLevelEnum::DBO_L_WARN, "DBO_L_WARN");
}
char noneLabel[]					 = "NONE";									// 0b0000'0000
char pakLabel[]						 = "PAK";									// 0b0000'0001
char floorLabel[]					 = "FLOOR";									// 0b0000'0010
char pakFloorLabel[]				 = "PAK & FLOOR";							// 0b0000'0011
char maskLabel[]					 = "MASK";									// 0b0000'0100
char pakMaskLabel[]					 = "PAK & MASK";							// 0b0000'0101
char floorMaskLabel[]				 = "FLOOR & MASK";							// 0b0000'0110
char pakFloorMaskLabel[]			 = "PAK & FLOOR & MASK";					// 0b0000'0111
char cameraLabel[]					 = "CAMERA";								// 0b0000'1000
char pakCameraLabel[]				 = "PAK & CAMERA";							// 0b0000'1001
char floorCameraLabel[]				 = "FLOOR & CAMERA";						// 0b0000'1010
char pakFloorCameraLabel[]			 = "PAK & FLOOR & CAMERA";					// 0b0000'1011
char maskCameraLabel[]				 = "MASK & CAMERA";							// 0b0000'1100
char pakMaskCameraLabel[]			 = "PAK & MASK & CAMERA";					// 0b0000'1101
char floorMaskCameraLabel[]			 = "FLOOR & MASK & CAMERA";					// 0b0000'1110
char pakFloorMaskCameraLabel[]		 = "PAK & FLOOR & MASK & CAMERA";			// 0b0000'1111
char soundLabel[]					 = "SOUND";									// 0b0001'0000
char soundPakLabel[]				 = "SOUND & PAK";							// 0b0001'0001
char soundFloorLabel[]				 = "SOUND & FLOOR";							// 0b0001'0010
char soundPakFloorLabel[]			 = "SOUND & PAK & FLOOR";					// 0b0001'0011
char soundMaskLabel[]				 = "SOUND & MASK";							// 0b0001'0100
char soundPakMaskLabel[]			 = "SOUND & PAK & MASK";					// 0b0001'0101
char soundFloorMaskLabel[]			 = "SOUND & FLOOR & MASK";					// 0b0001'0110
char soundPakFloorMaskLabel[]		 = "SOUND & PAK & FLOOR & MASK";			// 0b0001'0111
char soundCameraLabel[]				 = "SOUND & CAMERA";						// 0b0001'1000
char soundPakCameraLabel[]			 = "SOUND & PAK & CAMERA";					// 0b0001'1001
char soundFloorCameraLabel[]		 = "SOUND & FLOOR & CAMERA";				// 0b0001'1010
char soundPakFloorCameraLabel[]		 = "SOUND & PAK & FLOOR & CAMERA";			// 0b0001'1011
char soundMaskCameraLabel[]			 = "SOUND & MASK & CAMERA";					// 0b0001'1100
char soundPakMaskCameraLabel[]		 = "SOUND & PAK & MASK & CAMERA";			// 0b0001'1101
char soundFloorMaskCameraLabel[]	 = "SOUND & FLOOR & MASK & CAMERA";			// 0b0001'1110
char soundPakFloorMaskCameraLabel[]	 = "SOUND & PAK & FLOOR & MASK & CAMERA";	// 0b0001'1111
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
};
// Foreground: 3x
// Background: 4x
// 0: Black
// 1: Red
// 2: Green
// 3: Yellow
// 4: Blue
// 5: Magenta
// 6: Cyan
// 7: White
// char noneLabel[]	= "NONE";	// 0b0000'0000
char debugLabel[]	= "\033[7;32mDEBUG\033[0m";	// 0b0000'0001
char logLabel[]		= "\033[7;36mLOG\033[0m";	// 0b0000'0010
char infoLabel[]	= "\033[7;34mINFO\033[0m";	// 0b0000'0100
char warnLabel[]	= "\033[7;33mWARN\033[0m";	// 0b0000'1000
char errorLabel[]	= "\033[7;31mERROR\033[0m";	// 0b0001'0000
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

/// @brief 
/// @param category 
/// @param level 
/// @return true if 1 or more enabled categories are included, false otherwise.
/// @todo Support `level`
bool _shouldPrint(debugCategoryEnum category, debugLevelEnum level = (debugLevelEnum)DBO_L_ALL)
{
	if (debugOutput.debugOutputEnabled & category &&
		((debugLevelEnum*)&debugOutput)[(category & debugCategoryEnum::DBO_PAK) * sizeof(debugCategoryEnum)] & level ||
		((debugLevelEnum*)&debugOutput)[(category & debugCategoryEnum::DBO_FLOOR) * sizeof(debugCategoryEnum)] & level ||
		((debugLevelEnum*)&debugOutput)[(category & debugCategoryEnum::DBO_MASK) * sizeof(debugCategoryEnum)] & level ||
		((debugLevelEnum*)&debugOutput)[(category & debugCategoryEnum::DBO_CAMERA) * sizeof(debugCategoryEnum)] & level ||
		((debugLevelEnum*)&debugOutput)[(category & debugCategoryEnum::DBO_SOUND) * sizeof(debugCategoryEnum)] & level
		) {
		return true;
	} else {
		return false;
	}
}

#define MAX_DEBUG_OUTPUT_INDENTS 10

int section = 0;

char indent[MAX_DEBUG_OUTPUT_INDENTS + 1] = "";

debugCategoryEnum resultantCategory = DBO_NONE;
debugCategoryEnum categoryStack[10] = {};

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

void DebugRemoveCategory(debugCategoryEnum category)
{
	category = (debugCategoryEnum)(category & DBO_ALL); // Mask the unused bytes
	categoryStack[section - 1] = (debugCategoryEnum)(categoryStack[section - 1] ^ category);
	// TODO: Do this correctly by iterating through stack and &-ing together; currently will wipe repeated flags.
	resultantCategory = (debugCategoryEnum)(resultantCategory ^ category);
}
bool sectionBeganStack[50] = {};
int sectionBeganIdx = 0;
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
	if (sectionBeganIdx == 0) return false;
	bool v = sectionBeganStack[sectionBeganIdx];
	sectionBeganIdx--;
	if (section > 0 && (v || _shouldPrint(resultantCategory))) {
		// 1. Decrement section *count*
		--section;
		// 2. Clear indent at prior section *index*
		indent[section] = '\0';
		// 3. Remove category
		// 3A. Remove category from `resultantCategory`
		// TODO: Do this correctly by iterating through stack and &-ing together; currently will wipe repeated flags.
		resultantCategory = (debugCategoryEnum)(resultantCategory ^ categoryStack[section]);
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
#endif
