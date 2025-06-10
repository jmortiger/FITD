#pragma once

#include "osystem.h"

#define NB_BUFFER_ANIM 25 // AITD1 was  20
#define SIZE_BUFFER_ANIM (8*41) // AITD1 was 4*31

extern unsigned char frontBuffer[_SCREEN_INTERNAL_WIDTH * _SCREEN_INTERNAL_HEIGHT];

//extern OSystem osystem;
extern char rgbaBuffer[_SCREEN_INTERNAL_WIDTH * _SCREEN_INTERNAL_HEIGHT * 4];

/* #region Current Found Body */
extern char* currentFoundBody;
extern int currentFoundBodyIdx;
/* #endregion Current Found Body */
extern int statusVar1;

// #region Game Type
/// @brief The games supported.
enum gameTypeEnum
{
	AITD1,
	JACK,
	AITD2,
	AITD3,
	TIMEGATE,
}; typedef enum gameTypeEnum gameTypeEnum;

/// @brief Which game are we running?
extern enum gameTypeEnum g_gameId;
// #endregion Game Type

/// @brief [Link](https://kb.speeddemosarchive.com/Alone_in_the_Dark_(1-3)/Game_Mechanics_and_Glitches#ACTOR_FLAGS)
/// @details It doesn't *really* matter if it's signed; it's just easier b/c entries in the save data/arguments in life scripts are all signed 16 bit integers.
enum actorFlags : s16
{
	/// @brief Tells the renderer the area around the actor (in its 2D bbox) has to be redrawn every frame (e.g. isn't a static actor to be drawn into the [BACKGROUND2 buffer](https://kb.speeddemosarchive.com/Alone_in_the_Dark_(1-3)/Game_Mechanics_and_Glitches#:~:text=BACKGROUND2%3A,second%20one%20in%20memory%2E))
	///
	/// `0b0000'0000'0001`/`0x001`/`0001`/`1`
	AF_ANIMATED = 0b0000'0000'0001,
	/* /// @brief unused
	///
	/// `0b0000'0000'0010`/`0x002`/`0002`/`2`
	AF_UNUSED = 	0b0000'0000'0010, */
	/// @brief redraw – redraw once, then set this flag off (not shown in the RV since it often flickers constantly)
	///
	/// `0b0000'0000'0100`/`0x004`/`0004`/`4`
	AF_DRAWABLE = 0b0000'0000'0100,
	/// @brief not currently animating (replaces flag 0001 if the actor has completed its current ANIM without entering a different ANIM)
	///
	/// `0b0000'0000'1000`/`0x008`/`0010`/`8`
	AF_BOXIFY = 0b0000'0000'1000,
	/// @brief pushable
	///
	/// `0b0000'0001'0000`/`0x010`/`0020`/`16`
	AF_MOVABLE = 0b0000'0001'0000,
	/// @brief collisions – game will check for collisions between this actor and other actors and colliders
	///
	/// `0b0000'0010'0000`/`0x020`/`0040`/`32`
	AF_SPECIAL = 0b0000'0010'0000,
	/// @brief triggers – game will check for collisions between this actor and any triggers
	///
	/// `0b0000'0100'0000`/`0x040`/`0100`/`64`
	AF_TRIGGER = 0b0000'0100'0000,
	/// @brief collectible – PC colliding with this actor issues a FOUND call
	///
	/// `0b0000'1000'0000`/`0x080`/`0200`/`128`
	AF_FOUNDABLE = 0b0000'1000'0000,
	/// @brief gravity – one of the requirements for falling
	///
	/// `0b0001'0000'0000`/`0x100`/`0400`/`256`
	AF_FALLABLE = 0b0001'0000'0000,

	/// @brief Used to remove invalid flags from values.
	AF_MASK = AF_ANIMATED + AF_MOVABLE + AF_TRIGGER + AF_FOUNDABLE + AF_FALLABLE,
}; typedef enum actorFlags actorFlags;

// #region Misc. Structs
/// @brief A point in 3d space stored in signed 16 bit integers.
struct point3dStruct
{
	s16 x;
	s16 y;
	s16 z;
}; typedef struct point3dStruct point3dStruct;

// #region Text/Messages
struct regularTextEntryStruct
{
	/// @brief The address of the actual characters in `systemTextes`.
	u8* textPtr;
	s16 width;
}; typedef struct regularTextEntryStruct regularTextEntryStruct;

// IDEA: Merge w/ `regularTextEntry`?
struct textEntryStruct
{
	s16 index;
	/// @brief The address of the actual characters in `systemTextes`.
	u8* textPtr;
	s16 width;
}; typedef struct textEntryStruct textEntryStruct;

struct messageStruct
{
	textEntryStruct* string;
	/// @brief How many frames has the message been displayed?
	/// @details Only used in `drawTextOverlay` & `makeMessage`
	s16 time;
}; typedef struct messageStruct messageStruct;
// #endregion Text/Messages

struct saveEntry
{
	void* ptr;
	unsigned int size;
}; typedef struct saveEntry saveEntry;

struct hqrSubEntryStruct
{
	s16 key;
	s16 size;
	unsigned int lastTimeUsed;
	char* ptr;
}; typedef struct hqrSubEntryStruct hqrSubEntryStruct;

struct hqrEntryStruct
{
	char string[10];
	u16 maxFreeData;
	u16 sizeFreeData;
	u16 numMaxEntry;
	u16 numUsedEntry;
	hqrSubEntryStruct* entries;
}; typedef struct hqrEntryStruct hqrEntryStruct;

/// @brief Axis-Aligned Bounding Box; 32 bit signed integers.
struct ZVStruct
{
	/// @brief X min
	s32 ZVX1;
	/// @brief X max
	s32 ZVX2;
	/// @brief Y min
	s32 ZVY1;
	/// @brief Y max
	s32 ZVY2;
	/// @brief Z min
	s32 ZVZ1;
	/// @brief Z max
	s32 ZVZ2;
}; typedef struct ZVStruct ZVStruct;

struct interpolatedValue
{
	s16 oldAngle;
	s16 newAngle;
	s16 param;
	unsigned int timeOfRotate;
}; typedef struct interpolatedValue interpolatedValue;

struct tObject // used to read data from file too
{
	s16 indexInWorld;
	s16 bodyNum;
	u16 _flags;
	s16 dynFlags;
	ZVStruct zv;
	s16 screenXMin;
	s16 screenYMin;
	s16 screenXMax;
	s16 screenYMax;
	s16 roomX;
	s16 roomY;
	s16 roomZ;
	s16 worldX;
	s16 worldY;
	s16 worldZ;
	s16 alpha;
	s16 beta;
	s16 gamma;
	s16 stage;
	s16 room;
	s16 lifeMode;
	s16 life;
	unsigned int CHRONO;
	unsigned int ROOM_CHRONO;
	s16 ANIM;
	s16 animType;
	s16 animInfo;
	s16 newAnim;
	s16 newAnimType;
	s16 newAnimInfo;
	s16 FRAME;
	s16 numOfFrames;
	s16 END_FRAME;
	s16 END_ANIM;
	s16 trackMode;
	s16 trackNumber;
	s16 MARK;
	s16 positionInTrack;

	s16 stepX;
	s16 stepY;
	s16 stepZ;

	s16 animNegX;
	s16 animNegY;
	s16 animNegZ;

	interpolatedValue YHandler;
	s16 falling;
	interpolatedValue rotate;
	s16 direction;
	s16 speed;
	interpolatedValue speedChange;
	s16 COL[3];
	s16 COL_BY;
	s16 HARD_DEC;
	s16 HARD_COL;
	s16 HIT;
	s16 HIT_BY;
	s16 animActionType;
	s16 animActionANIM;
	s16 animActionFRAME;
	s16 animActionParam;
	s16 hitForce;
	s16 hotPointID;
	point3dStruct hotPoint;

	// aitd2
	s16 hardMat;
}; typedef struct tObject tObject;

struct tWorldObject
{
	s16 objIndex;
	s16 body;
	union
	{
		s16 flags;
		actorFlags bitField;
	};
	s16 typeZV;
	s16 foundBody;
	s16 foundName;
	s16 flags2;
	s16 foundLife;
	s16 x;
	s16 y;
	s16 z;
	s16 alpha;
	s16 beta;
	s16 gamma;
	s16 stage;
	s16 room;
	s16 lifeMode;
	s16 life;
	s16 floorLife;
	s16 anim;
	s16 frame;
	s16 animType;
	s16 animInfo;
	s16 trackMode;
	s16 trackNumber;
	s16 positionInTrack;

	// AITD2
	s16 mark;
}; typedef struct tWorldObject tWorldObject;

/// @brief 
/// @todo Document
/// @todo Rename fields
struct boxStruct
{
	s16 var0;
	s16 var1;
	s16 var2;
	s16 var3;
}; typedef struct boxStruct boxStruct;

struct roomDefStruct
{
	/// @brief Offset: 0
	s16 offsetToCameraDef;
	/// @brief Offset: 2
	s16 offsetToPosDef;
	/// @brief Offset: 4
	s16 worldX;
	/// @brief Offset: 6
	s16 worldY;
	/// @brief Offset: 8
	s16 worldZ;
	/// @brief Offset: 0xA
	s16 numCameraInRoom;
}; typedef struct roomDefStruct roomDefStruct;
// #endregion Misc. Structs

extern hqrEntryStruct* HQ_Memory;

/* #region Unused */
extern int videoMode;
extern int musicConfigured;
extern int musicEnabled;

extern int screenBufferSize;
extern int unkScreenVar2;
extern int actorTurnedToObj;

extern boxStruct genVar2[15]; // recheckSize
extern boxStruct genVar4[50];
extern boxStruct* genVar1;
extern boxStruct* genVar3;

extern int genVar5;
extern int genVar6;

extern int overlaySize1;
extern int overlaySize2;

extern int bgOverlayVar1;
/* #endregion */

// #region Screen Video Buffers
extern char* aux;
extern char* aux2;
extern char* logicalScreen;
// #endregion Screen Video Buffers

extern std::vector<std::vector<s16>> BufferAnim;

extern std::vector<s16> CVars;

extern char* PtrPrioritySample;

extern char* PtrFont;

extern char* PtrCadre;

struct PaletteColorRGB {
	// #region Fields
	unsigned char r;
	unsigned char g;
	unsigned char b;
	// #endregion Fields
	// #region Operator
	bool operator==(const PaletteColorRGB& other) {
		return (
			r == other.r &&
			g == other.g &&
			b == other.b
		);
	}
	bool operator==(unsigned char* other) {
		return (
			r == other[0] &&
			g == other[1] &&
			b == other[2]
		);
	}
	bool operator!=(const PaletteColorRGB& other) { return !(*this == other); }
	bool operator!=(unsigned char* other) { return !(*this == other); }
	// #endregion Operator
	// #region Update
	void updateOther(unsigned char* rgb) {
		rgb[0] = r;
		rgb[1] = g;
		rgb[2] = b;
	}
	void update(unsigned char* rgb) {
		r = rgb[0];
		g = rgb[1];
		b = rgb[2];
	}
	void update(unsigned char r, unsigned char g, unsigned char b) {
		this->r = r;
		this->g = g;
		this->b = b;
	}
	void update(PaletteColorRGB& color) {
		r = color.r;
		g = color.g;
		b = color.b;
	}
	void update(PaletteColorRGB* color) {
		r = color->r;
		g = color->g;
		b = color->b;
	}
	// #endregion Update
}; typedef struct PaletteColorRGB PaletteColorRGB;

extern PaletteColorRGB currentGamePalette[COLORS_IN_PALETTE];

// #region Timers
extern unsigned int timer;
extern unsigned int timeGlobal;
// #endregion Timers

// #region Window Coords
extern int WindowX1;
extern int WindowY1;
extern int WindowX2;
extern int WindowY2;
// #endregion Window Coords

// #region Input Fields
extern "C" {
	extern char JoyD;
};
extern char Click;
extern char key;
extern char localKey;
extern char localJoyD;
extern char localClick;
// #endregion Input Fields

// #region Languages
extern const std::vector<std::string> languageNameTable;
extern char languageNameString[];
// #endregion Languages

// #region Text/Messages
extern textEntryStruct* tabTextes;
extern u8* systemTextes;

extern regularTextEntryStruct textTable[40];

extern int turnPageFlag;
// #endregion Text/Messages

extern int hqrKeyGen;

extern char* screenSm1;
extern char* screenSm2;
extern char* screenSm3;
extern char* screenSm4;
extern char* screenSm5;

extern tObject objectTable[NUM_MAX_OBJECT];

extern s16 currentWorldTarget;

// #region Caches
extern hqrEntryStruct* listMus;
extern hqrEntryStruct* listSamp;
extern hqrEntryStruct* listBody;
extern hqrEntryStruct* listAnim;
extern hqrEntryStruct* listLife;
extern hqrEntryStruct* listTrack;
extern hqrEntryStruct* listMatrix;
// #endregion Caches

extern s16 maxObjects;

extern std::vector<tWorldObject> ListWorldObjets; // may be less

extern s16* vars;

extern int varSize;

extern messageStruct messageTable[5];

extern s16 currentMusic;
extern int action;

extern int nextSample;
extern int nextMusic;
extern s16 currentCameraTargetActor;
extern s16 fIsGameOver;
extern s16 lightOff;
extern int lightVar2;
extern int LastPriority;
extern int LastSample;
extern s16 statusScreenAllowed;

// #region Floor, Room, & Camera
extern char* g_currentFloorRoomRawData;
extern char* g_currentFloorCameraRawData;

extern int changeFloor;
extern s16 currentCamera;
extern s16 g_currentFloor;
extern int needChangeRoom;

extern char* cameraPtr;
extern roomDefStruct* pCurrentRoomData;

extern s16 currentRoom;
extern int flagInitView;
extern int flagRedraw;
extern bool cameraBackgroundChanged;
extern int numCameraInRoom;
extern int numCameraZone;
extern char* cameraZoneData;
extern int numRoomZone;
extern char* roomZoneData;
extern char* room_PtrCamera[NUM_MAX_CAMERA_IN_ROOM];
extern int startGameVar1;

extern int transformX;
extern int transformY;
extern int transformZ;
extern int transformXCos;
extern int transformXSin;
extern int transformYCos;
extern int transformYSin;
extern int transformZCos;
extern int transformZSin;
extern bool transformUseX;
extern bool transformUseY;
extern bool transformUseZ;

extern int translateX;
extern int translateY;
extern int translateZ;

extern int cameraCenterX;
extern int cameraCenterY;
extern int cameraPerspective;
extern int cameraFovX;
extern int cameraFovY;

extern char currentCameraVisibilityList[30];
// #endregion Floor, Room, & Camera

// #region Life Script Fields
extern int currentProcessedActorIdx;
extern tObject* currentProcessedActorPtr;

extern int currentLifeActorIdx;
extern tObject* currentLifeActorPtr;
extern int currentLifeNum;

extern char* currentLifePtr;

s16 readNextArgument(const char* name = NULL);
// #endregion Life Script Fields

extern float renderPointList[6400];

extern int numActorInList;
extern int sortedActorTable[NUM_MAX_OBJECT];

extern int angleCompX;
extern int angleCompZ;
extern int angleCompBeta;

extern int bufferAnimCounter;

extern int animCurrentTime;
extern int animKeyframeLength;
extern int animMoveX;
extern int animMoveY;
extern int animMoveZ;
extern int animStepZ;
extern int animStepX;
extern int animStepY;
extern char* animVar1;
extern char* animVar3;
extern char* animVar4;

extern s16 newFloor;

extern int fadeState;

extern char cameraBuffer[256];
extern char cameraBuffer2[256];
extern char cameraBuffer3[400];
extern char cameraBuffer4[400];

extern char* cameraBufferPtr;
extern char* cameraBuffer2Ptr;
extern char* cameraBuffer3Ptr;

extern s16 newRoom;

extern const char* listBodySelect[];
extern const char* listAnimSelect[];

extern s16 shakeVar1;
extern s16 shakingAmplitude;
extern unsigned int timerFreeze1;

extern hardColStruct* hardColTable[10];

extern s16 hardColStepX;
extern s16 hardColStepZ;

extern ZVStruct hardClip;

extern saveEntry saveTable[];

extern int HQ_Load;
extern int lightX;
extern int lightY;

extern int clipLeft;
extern int clipTop;
extern int clipRight;
extern int clipBottom;

extern unsigned char* g_MaskPtr;

#if defined(FITD_DEBUGGER)
enum backgroundModeEnum
{
	backgroundModeEnum_2D,
	backgroundModeEnum_3D,
}; typedef enum backgroundModeEnum backgroundModeEnum;

extern backgroundModeEnum backgroundMode;
#endif

struct sGroupState
{
	s16 m_type; // 8
	s16 m_delta[3]; // A
	s16 m_rotateDelta[3]; // 10 (AITD2+) if Info_optimise
};

struct sGroup
{
	s16 m_start; // 0
	s16 m_numVertices; // 2
	s16 m_baseVertices; // 4
	s8 m_orgGroup; // 6
	s8 m_numGroup; // 7
	sGroupState m_state;
};

/// @brief Defines the primitive types.
enum primTypeEnum
{
	primTypeEnum_Line = 0,
	primTypeEnum_Poly = 1,
	primTypeEnum_Point = 2,
	primTypeEnum_Sphere = 3,
	primTypeEnum_Disk = 4,
	primTypeEnum_Cylinder = 5,
	primTypeEnum_BigPoint = 6,
	primTypeEnum_Zixel = 7,
	processPrim_PolyTexture8 = 8,
	processPrim_PolyTexture9 = 9,
	processPrim_PolyTexture10 = 10,
};

struct sPrimitive
{
	primTypeEnum m_type;
	u8 m_material;
	u8 m_color;
	u8 m_even;
	u16 m_size;
	std::vector<u16> m_points;
};

struct sExtraBody
{
	u16 m_startOfKeyframe; // 2
};

// scratch buffer:
// 4: u16 timer


struct sBody
{
	void* m_raw;

	u16 m_flags;
	ZVStruct m_zv;
	std::vector<u8> m_scratchBuffer;
	std::vector<point3dStruct> m_vertices;
	std::vector<uint16> m_groupOrder;
	std::vector<sGroup> m_groups;
	std::vector<sPrimitive> m_primitives;

	void sync();
};

struct sFrame
{
	u16 m_timestamp;
	s16 m_animStep[3];
	std::vector<sGroupState> m_groups;
};

struct sAnimation
{
	void* m_raw;

	u16 m_numFrames;
	u16 m_numGroups;
	std::vector<sFrame> m_frames;
};

