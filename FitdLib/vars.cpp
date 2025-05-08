#include "common.h"

//OSystem osystem;
char rgbaBuffer[_SCREEN_INTERNAL_WIDTH * _SCREEN_INTERNAL_HEIGHT * 4];

// #region Current Found Body
char* currentFoundBody;
int currentFoundBodyIdx;
// #endregion Current Found Body
int statusVar1;

gameTypeEnum g_gameId;

hqrEntryStruct* HQ_Memory;

/* #region Unused */
/// @brief Currently unused (set to 1 once and otherwise unreferenced).
/// @todo What was this for, and can it be removed?
int videoMode;
/// @brief A flag indicating if music has been configured; currently unused.
/// @todo Should this be removed?
int musicConfigured;
/// @brief A flag indicating if music is enabled; currently unused.
/// @todo Should this be removed?
int musicEnabled;

/// @brief Set to 64800 and otherwise unused.
/// @todo Should this be removed?
int screenBufferSize;
/// @brief Set to 3 and otherwise unused.
/// @todo What was this for, and can it be removed?
int unkScreenVar2;
/// @brief Currently unused (set to 1 and otherwise unreferenced).
/// @todo What was this for, and can it be removed?
int actorTurnedToObj = 0;
/* #endregion Unused */

char* aux;
char* aux2;
std::vector<std::vector<s16>> BufferAnim;

char* logicalScreen;

std::vector<s16> CVars;

char* PtrPrioritySample;

char* PtrFont;

/// @brief Border Graphics (loaded from [`ITD_RESS.PAK` index 4](https://kb.speeddemosarchive.com/Alone_in_the_Dark_(1-3)/Game_Mechanics_and_Glitches#GAME_FILES))
char* PtrCadre;

unsigned char currentGamePalette[256 * 3];

// #region Timers
unsigned int timer;
unsigned int timeGlobal;
// #endregion Timers

// #region Window Coords
/// @brief Something to do w/ `AffBigCadre`
/// @todo Document
int WindowX1;
/// @brief Something to do w/ `AffBigCadre`
/// @todo Document
int WindowY1;
/// @brief Something to do w/ `AffBigCadre`
/// @todo Document
int WindowX2;
/// @brief Something to do w/ `AffBigCadre`
/// @todo Document
int WindowY2;
// #endregion Window Coords

// #region Input Fields
char JoyD = 0;
char Click = 0;
char key = 0;
char localKey;
char localJoyD;
char localClick;
// #endregion Input Fields

// #region Languages
/// @brief The filenames of supported languages.
/// @todo Earlier entries take priority over later entries; change order with system locale or something.
const std::vector<std::string> languageNameTable =
{
	"ENGLISH",
	"FRANCAIS",
	"ITALIANO",
	"ESPAGNOL",
	"DEUTSCH",
};

/// @brief The selected language.
char languageNameString[20] = "";
// #endregion Languages

textEntryStruct* tabTextes;
u8* systemTextes;

regularTextEntryStruct textTable[NUM_MAX_TEXT];

int turnPageFlag;

int hqrKeyGen = 0;

char* screenSm1;
char* screenSm2;
char* screenSm3;
char* screenSm4;
char* screenSm5;

tObject objectTable[NUM_MAX_OBJECT];

s16 currentWorldTarget;

// #region Caches
hqrEntryStruct* listMus;
hqrEntryStruct* listSamp;
hqrEntryStruct* listBody;
hqrEntryStruct* listAnim;
hqrEntryStruct* listLife;
hqrEntryStruct* listTrack;
hqrEntryStruct* listMatrix;
// #endregion Caches

s16 maxObjects;

std::vector<tWorldObject> ListWorldObjets; // may be less

s16* vars;

/// @brief The amount of memory taken up by `vars` in bytes (each var is 2 bytes).
int varSize;

messageStruct messageTable[NUM_MAX_MESSAGE];

s16 currentMusic;
int action;

boxStruct genVar2[15]; // recheckSize
boxStruct genVar4[50];
boxStruct* genVar1;
boxStruct* genVar3;

int genVar5;
int genVar6;
int nextSample;
int nextMusic;
s16 currentCameraTargetActor;
/// @brief A flag indicating the player died; used to stop the main loop & transfer back to the start menu. Is (unnecessarily) included in save file (thus the s16).
s16 fIsGameOver;
s16 lightOff;
int lightVar2;
int LastPriority;
int LastSample;
s16 statusScreenAllowed;

// #region Floor, Room, & Camera
char* g_currentFloorRoomRawData = NULL;
char* g_currentFloorCameraRawData = NULL;

int changeFloor;
s16 currentCamera;
s16 g_currentFloor;
int needChangeRoom;

char* cameraPtr;
roomDefStruct* pCurrentRoomData;

s16 currentRoom;
int flagInitView;
int numCameraInRoom;
int numCameraZone;
char* cameraZoneData;
int numRoomZone;
char* roomZoneData;
char* room_PtrCamera[NUM_MAX_CAMERA_IN_ROOM];
/// @brief Somehow related to camera indicies.
/// @todo Rename
/// @todo Document
int startGameVar1;

int transformX;
int transformY;
int transformZ;
int transformXCos;
int transformXSin;
int transformYCos;
int transformYSin;
int transformZCos;
int transformZSin;
bool transformUseX;
bool transformUseY;
bool transformUseZ;

int translateX;
int translateY;
int translateZ;

int cameraCenterX;
int cameraCenterY;
int cameraPerspective;
int cameraFovX;
int cameraFovY;

char currentCameraVisibilityList[30];
// #endregion Floor, Room, & Camera

int currentProcessedActorIdx;
tObject* currentProcessedActorPtr;

int currentLifeActorIdx;
tObject* currentLifeActorPtr;
int currentLifeNum;

char* currentLifePtr;

s16 readNextArgument(const char* name)
{
	s16 value = *(s16*)(currentLifePtr);
	currentLifePtr += 2;

	if (name) {
		appendFormatted("%s:%d, ", name, value);
	} else {
		appendFormatted("%d, ", value);
	}

	return value;
}

bool cameraBackgroundChanged = false;
int flagRedraw;

float renderPointList[6400];

int numActorInList;
int sortedActorTable[NUM_MAX_OBJECT];

int angleCompX;
int angleCompZ;
int angleCompBeta;

int bufferAnimCounter = 0;

int animCurrentTime;
int animKeyframeLength;

int animMoveX;
int animMoveY;
int animMoveZ;

int animStepX;
int animStepZ;
int animStepY;

char* animVar1;
char* animVar3;
char* animVar4;

s16 newFloor;

int fadeState;

char cameraBuffer[256];
char cameraBuffer2[256];
char cameraBuffer3[400];
char cameraBuffer4[400];

char* cameraBufferPtr = cameraBuffer;
char* cameraBuffer2Ptr = cameraBuffer2;
char* cameraBuffer3Ptr = cameraBuffer3;

int overlaySize1;
int overlaySize2;

int bgOverlayVar1;

s16 newRoom;

s16 shakeVar1;
s16 shakingAmplitude;
unsigned int timerFreeze1;

hardColStruct* hardColTable[10];

s16 hardColStepX;
s16 hardColStepZ;

ZVStruct hardClip;

const char* listBodySelect[] = {
	"LISTBODY",
	"LISTBOD2",
};

const char* listAnimSelect[] = {
	"LISTANIM",
	"LISTANI2",
};

saveEntry saveTable[40];

int HQ_Load = 0;
int lightX = 4000;
int lightY = -2000;

int clipLeft = 0;
int clipTop = 0;
int clipRight = 319;
/// @brief 
/// @todo This was previously initialized to 119; ensure this was done in error.
int clipBottom = 199;

unsigned char* g_MaskPtr = NULL;

#if defined(FITD_DEBUGGER)
backgroundModeEnum backgroundMode = backgroundModeEnum_2D;
#endif
