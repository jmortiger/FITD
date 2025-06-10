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

/// @brief Currently Unused
/// @todo Determine what this was for.
boxStruct genVar2[15]; // recheckSize
/// @brief Currently Unused
/// @todo Determine what this was for.
boxStruct genVar4[50];
/// @brief Currently Unused
/// @todo Determine what this was for.
boxStruct* genVar1;
/// @brief Currently Unused
/// @todo Determine what this was for.
boxStruct* genVar3;

/// @brief Currently Unused
/// @todo Determine what this was for.
int genVar5;
/// @brief Currently Unused
/// @todo Determine what this was for.
int genVar6;

/// @brief Currently Unused
/// @todo Determine what this was for.
int overlaySize1;
/// @brief Currently Unused
/// @todo Determine what this was for.
int overlaySize2;

/// @brief Currently Unused
/// @todo Determine what this was for.
int bgOverlayVar1;
/* #endregion Unused */

/// @brief BACKGROUND1: A direct copy of a 2D background from the PAK file. Never overwritten after uncompressed. This is the third one in memory. Every time the camera changes, a new image is loaded into BACKGROUND1. [Link](https://kb.speeddemosarchive.com/Alone_in_the_Dark_(1-3)/Game_Mechanics_and_Glitches#:~:text=BACKGROUND1%3A,loaded%20into%20BACKGROUND1%2E)
/// @todo Change name
char* aux;
/// @brief BACKGROUND2: The same but with non-animated actors rendered onto it. When a static actor starts moving (gets the animated flag 0001 or redraw flag 0004), it gets removed from BACKGROUND2 by copying BACKGROUND1 and re-rendering all non-moving actors. The idea is to avoid having to render things that aren't currently moving. Similarly, when an actor becomes static, it is drawn into BACKGROUND2. This image is the second one in memory. [Link](https://kb.speeddemosarchive.com/Alone_in_the_Dark_(1-3)/Game_Mechanics_and_Glitches#:~:text=BACKGROUND2%3A,second%20one%20in%20memory%2E)
/// @todo Change name
char* aux2;
std::vector<std::vector<s16>> BufferAnim;

/// @brief BACKBUFFER (offscreen): The same as BACKGROUND2 but with moving actors rendered also. When animating actors have to be re-rendered (every frame), the previous polygons are erased by copying small 2D rectangles from BACKGROUND2 to BACKBUFFER. Without the BACKBUFFER you would see all rendered objects flickering all the time. The BACKBUFFER has a size of 64320 (slightly bigger than 320 x 200) and it's the first one in memory. [Link](https://kb.speeddemosarchive.com/Alone_in_the_Dark_(1-3)/Game_Mechanics_and_Glitches#:~:text=BACKBUFFER%20%28offscreen%29%3A,the%20first%20one%20in%20memory%2E)
char* logicalScreen;

std::vector<s16> CVars;

char* PtrPrioritySample;

char* PtrFont;

/// @brief Border Graphics (loaded from [`ITD_RESS.PAK` index 4](https://kb.speeddemosarchive.com/Alone_in_the_Dark_(1-3)/Game_Mechanics_and_Glitches#GAME_FILES))
char* PtrCadre;

/**
 * @brief The game's current color palette defined by 256 RGB byte triplets.
 * @todo Convert to an array of color structs.
 */
PaletteColorRGB currentGamePalette[COLORS_IN_PALETTE];
// unsigned char currentGamePalette[BYTES_IN_PALETTE];

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

// #region Text
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

/// @brief Text entries pointers
/// @details https://docs.google.com/spreadsheets/d/1cYRTP37v7Y11O38okNyHPg1YrZx549GG6z2vhY7QRok/edit?gid=0#gid=0&range=27:27
textEntryStruct* tabTextes;
/// @brief The raw text entries loaded from the language pak file.
u8* systemTextes;

regularTextEntryStruct textTable[NUM_MAX_TEXT];

int turnPageFlag;

/// @brief The message queue; these are rendered across the bottom of the screen.
messageStruct messageTable[NUM_MAX_MESSAGE];
// #endregion Text

int hqrKeyGen = 0;

char* screenSm1;
char* screenSm2;
char* screenSm3;
char* screenSm4;
char* screenSm5;

tObject objectTable[NUM_MAX_OBJECT];

s16 currentWorldTarget;

// #region HQR Caches
hqrEntryStruct* listMus;
hqrEntryStruct* listSamp;
hqrEntryStruct* listBody;
hqrEntryStruct* listAnim;
hqrEntryStruct* listLife;
hqrEntryStruct* listTrack;
hqrEntryStruct* listMatrix;
// #endregion HQR Caches

s16 maxObjects;

std::vector<tWorldObject> ListWorldObjets; // may be less

// #region VARS
/// @brief Used to persist game state across multiple life scripts.
s16* vars;

/// @brief The amount of memory taken up by `vars` in bytes (each var is 2 bytes, so `varSize / 2` should be the number of `vars`).
/// @details * Derived from the filesize (in bytes) of `VARS.ITD`.
/// * This largely goes unused
/// 	* It's included in save data, and is assigned & used when loading save data to correctly load `vars`, but this could break things if a save file w/ a different number of vars was loaded, & could be determined at the time of saving.
/// 	* Other mechanisms are used for bounding `vars`.
int varSize;
// #endregion VARS

/// @brief The current selected action (e.g. Fight, Open/Search, Jump) (I believe).
int action;

// #region Music & Sound
s16 currentMusic;

int nextSample;
int nextMusic;
int LastPriority;
int LastSample;
// #endregion Music & Sound

s16 currentCameraTargetActor;
/// @brief A flag indicating the player died; used to stop the main loop & transfer back to the start menu. Is (unnecessarily) included in save file (thus the s16).
s16 fIsGameOver;
s16 lightOff;
int lightVar2;
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
/// @todo Determine & describe specific difference with `flagRedraw`
/// @brief 
/// @details Not a true flag, but a mode switch.
/// 0: No BG change
/// 1: Static actors changed (update BG2)
/// 2: Cam/Background image changed (update BG1)
/// @todo Determine & describe specific difference with `flagRedraw`
int flagInitView;
/// @brief 
/// @details Not a true flag, but a mode switch.
/// 0: No BG change
/// 1: Static actors changed (update BG2)
/// 2: Cam/Background image changed (update BG1)
/// @todo Determine & describe specific difference with `flagInitView`
int flagRedraw;
/// @brief Seemingly replaces `flagRedraw`
bool cameraBackgroundChanged = false;
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
/// @brief Should the point be transformed __between coordinate spaces (?)__ using `transformYCos` & `transformYSin`? Changed for each camera.
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

// #region Life Script
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
// #endregion Life Script

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
int clipRight = _SCREEN_INTERNAL_WIDTH - 1;
/// @brief 
/// @todo This was previously initialized to 119; ensure this was done in error.
int clipBottom = _SCREEN_INTERNAL_HEIGHT - 1;

unsigned char* g_MaskPtr = NULL;

#if defined(FITD_DEBUGGER)
backgroundModeEnum backgroundMode = backgroundModeEnum_2D;
#endif
