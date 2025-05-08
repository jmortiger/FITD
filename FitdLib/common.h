#ifndef _COMMON_H_
#define _COMMON_H_

#include "config.h"

#ifdef USE_IMGUI
#include "imgui.h"
#endif

/* #region Screen */
/// @todo Why 64800? AITD is 320*200=64000 & is rendered in 4:3 as 320*240=76800.
#define _SCREEN_BUFFER_SIZE 64800
#define _SCREEN_INTERNAL_PIXELS 64000
#define _SCREEN_INTERNAL_WIDTH 320
#define _SCREEN_INTERNAL_HEIGHT 200
#define _SCREEN_INTERNAL_WIDTH_FLOAT 320.f
#define _SCREEN_INTERNAL_HEIGHT_FLOAT 200.f
/* #endregion */

/* #region GAME SPECIFIC DEFINES */
#define NUM_MAX_CAMERA_IN_ROOM 20
//#define NUM_MAX_OBJ         300
#define NUM_MAX_OBJECT       50
#define NUM_MAX_TEXT        40
#define NUM_MAX_MESSAGE     5

// 250
#define NUM_MAX_TEXT_ENTRY  1000
/* #endregion */

/* #region  CVars */
enum enumCVars
{
	SAMPLE_PAGE = 0,
	BODY_FLAMME = 1, // gun flash body
	MAX_WEIGHT_LOADABLE = 2, // max inventory weight
	TEXTE_CREDITS = 3, // Credits text
	SAMPLE_TONNERRE = 4, // Thunder SFX
	INTRO_DETECTIVE = 5, // Detective intro text (Edward)
	INTRO_HERITIERE = 6, // Heiress intro text (Emily)
	WORLD_NUM_PERSO = 7, // initial camera target
	CHOOSE_PERSO = 8, // The selected protagonist (0 for Edward, 1 for Emily)
	SAMPLE_CHOC = 9, // Hit SFX
	SAMPLE_PLOUF = 10, // Splash SFX
	REVERSE_OBJECT = 11, // revert lantern object id
	KILLED_SORCERER = 12, // Pregtz dead
	LIGHT_OBJECT = 13,
	FOG_FLAG = 14, // ashtray smoke flag
	DEAD_PERSO = 15, // Game over flag
	JET_SARBACANE, // Blowgun jet
	TIR_CANON, // Cannon shot
	JET_SCALPEL,
	POIVRE, // Pepper
	DORTOIR, // Dormitory
	EXT_JACK,
	NUM_MATRICE_PROTECT_1,
	NUM_MATRICE_PROTECT_2,
	NUM_PERSO,
	TYPE_INVENTAIRE,
	PROLOGUE,
	POIGNARD, // Dagger
	MATRICE_FORME,
	MATRICE_COULEUR, // Color Matrix

	UNKNOWN_CVAR // for table padding, shouldn't be called !
}; typedef enum enumCVars enumCVars;

extern int AITD1KnownCVars[];
extern int AITD2KnownCVars[];
extern int* currentCVarTable;

int getCVarsIdx(enumCVars);
int getCVarsIdx(int);
/* #endregion */

/* #region Sized types */
#if !AITD_UE4
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned long uint32;
#ifndef UNIX
typedef unsigned int uint;
#endif
typedef signed char int8;
typedef signed short int16;
typedef signed long int32;
#endif
// TODO: Unused out of ThirdParty & redundant w/ members in config.h
typedef unsigned char U8;
typedef unsigned short U16;
typedef unsigned int U32;
typedef signed char S8;
typedef signed short S16;
typedef signed int S32;
/* #endregion */

// #define TYPE_MASK 0x1D1 // Redundant w/ actorFlags::AF_MASK

#define ANIM_ONCE             0
#define ANIM_REPEAT           1
#define ANIM_UNINTERRUPTABLE  2
#define ANIM_RESET            4

#include "debugging/logging_defines.h"
#include "room.h"
#include "vars.h"
#include "main.h"
#include "fileAccess.h"
#include "screen.h"
#include "videoMode.h"
#include "pak/pak.h"
#include "pak/unpak.h"
#include "tatou.h"
#include "threadCode.h"
#include "renderer.h"
#include "input.h"
#include "version.h"
#include "cosTable.h"
#include "hqr.h"
#include "gameTime.h"
#include "font.h"
#include "aitdBox.h"
#include "save.h"
#include "anim/anim.h"
#include "anim/animAction.h"
#include "actorList.h"
#include "mainLoop.h"
#include "inventory.h"
#include "startupMenu.h"
#include "systemMenu.h"
#include "floor.h"
#include "object.h"
#include "collision/zv.h"
#include "audio/music.h"
#include "audio/fmopl.h"
#include "main.h"
#include "sequence.h"

/* #region include game specific stuff */
#include "AITD1.h"
#include "AITD2.h"
#include "AITD3.h"
#include "JACK.h"
/* #endregion */

/* #region debugger */
#ifdef FITD_DEBUGGER
#include "debugging/debugger.h"
#endif
/* #endregion */

/* #region scripting */
#include "track.h"
#include "life.h"
#include "evalVar.h"
/* #endregion */

#include "osystem.h"

/* #region Endianess */
//typedef unsigned char byte;

#ifdef UNIX
#define FORCEINLINE static inline
#else
#ifdef WIN32
#define FORCEINLINE __forceinline
#else
#define FORCEINLINE inline
#endif
#endif

FORCEINLINE u8 READ_LE_U8(void* ptr)
{
	return *(u8*)ptr;
}

FORCEINLINE s8 READ_LE_S8(void* ptr)
{
	return *(s8*)ptr;
}

FORCEINLINE u16 READ_LE_U16(void* ptr)
{
#ifdef MACOSX
	return (((u8*)ptr)[1] << 8) | ((u8*)ptr)[0];
#else
	return *(u16*)ptr;
#endif
}

FORCEINLINE s16 READ_LE_S16(void* ptr)
{
	return (s16)READ_LE_U16(ptr);
}

FORCEINLINE u16 READ_BE_U16(void* ptr)
{
#ifdef MACOSX
	return *(u16*)ptr;
#else
	return (((u8*)ptr)[0] << 8) | ((u8*)ptr)[1];
#endif
}

FORCEINLINE s16 READ_BE_S16(void* ptr)
{
	return (s16)READ_BE_U16(ptr);
}

FORCEINLINE u32 READ_LE_U32(void* ptr)
{
#ifdef MACOSX
	return (((u8*)ptr)[3] << 24) | (((u8*)ptr)[2] << 16) | (((u8*)ptr)[1] << 8) | ((u8*)ptr)[0];
#else
	return *(u32*)ptr;
#endif
}

FORCEINLINE s32 READ_LE_S32(void* ptr)
{
	return (s32)READ_LE_U32(ptr);
}

FORCEINLINE u32 READ_BE_U32(void* ptr)
{
#ifdef MACOSX
	return *(u32*)ptr;
#else
	return (((u8*)ptr)[3] << 24) | (((u8*)ptr)[2] << 16) | (((u8*)ptr)[1] << 8) | ((u8*)ptr)[0];
#endif
}

FORCEINLINE s32 READ_BE_S32(void* ptr)
{
	return (s32)READ_LE_U32(ptr);
}
/* #endregion */

/// @brief Add a breakpoint here to catch all fatal exits.
FORCEINLINE void FITD_throwFatal()
{
	assert(0);
}

#endif
