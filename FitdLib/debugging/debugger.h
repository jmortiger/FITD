#ifndef _DEBUGGER_H_
#define _DEBUGGER_H_

#ifdef FITD_DEBUGGER
// #region debug var used in engine
extern bool debuggerVar_drawModelZv;
extern bool debuggerVar_drawCameraCoverZone;
extern bool debuggerVar_noHardClip;
extern bool debuggerVar_topCamera;
extern long int debufferVar_topCameraZoom;

extern bool debuggerVar_useBlackBG;
extern bool debuggerVar_fastForward;
// #endregion debug var used in engine

void debugger_draw(void);
#endif // FITD_DEBUGGER

// #region Formatted Standard Out
/// @brief Read the command-line parameters and accordingly set the debug output.
/// @param argc 
/// @param argv 
/// @todo Support flag to print palette
void parseDebugParam(int argc, char* argv[]);

// TODO: Convert to use a struct/object.

/// @brief The categories of debug info supported, as a bit flag.
/// @todo Add mask to enum
enum debugCategoryEnum : unsigned char {
	DBO_NONE	 = 0b0000'0000,
	DBO_PAK		 = 0b0000'0001,
	DBO_FLOOR	 = 0b0000'0010,
	DBO_MASK	 = 0b0000'0100,
	DBO_CAMERA	 = 0b0000'1000,
	DBO_SOUND	 = 0b0001'0000,
	DBO_ITD		 = 0b0010'0000,
	DBO_LIFE	 = 0b0100'0000,
};
#define DBO_ALL 0b0111'1111
typedef enum debugCategoryEnum debugCategoryEnum;
ENUM_CLASS_FLAGS(debugCategoryEnum)

/// @brief The debug levels supported, as a bit flag.
/// @todo Add mask to enum
enum debugLevelEnum : unsigned char {
	DBO_L_NONE	= 0b0000'0000,
	DBO_L_DEBUG	= 0b0000'0001,
	DBO_L_LOG2	= 0b0000'0010,
	DBO_L_LOG	= 0b0000'0100,
	DBO_L_INFO2	= 0b0000'1000,
	DBO_L_INFO1	= 0b0001'0000,
	DBO_L_INFO	= 0b0010'0000,
	DBO_L_WARN	= 0b0100'0000,
	DBO_L_ERROR	= 0b1000'0000,
	// DBO_L_NONE	= 0b0000'0000,
	// DBO_L_DEBUG	= 0b0000'0001,
	// DBO_L_LOG	= 0b0000'0010,
	// DBO_L_INFO	= 0b0000'0100,
	// DBO_L_WARN	= 0b0000'1000,
	// DBO_L_ERROR	= 0b0001'0000,
};
#define DBO_L_ALL 0b1111'1111
// #define DBO_L_ALL 0b0001'1111

typedef enum debugLevelEnum debugLevelEnum;
ENUM_CLASS_FLAGS(debugLevelEnum)

// #region Color Macros
#ifndef ANSI_RESET
#define ANSI_RESET "\033[0m"
#define ANSI_BLACK 0
#define ANSI_RED 1
#define ANSI_GREEN 2
#define ANSI_YELLOW 3
#define ANSI_BLUE 4
#define ANSI_MAGENTA 5
#define ANSI_CYAN 6
#define ANSI_WHITE 7
#endif
#define Str(x) #x
#define XStr(x) Str(x)
#define _AnsiInvertFg(x) "\033[7;3" Str(x) "m"
#define AnsiInvertFg(x) _AnsiInvertFg(x)
#define _AnsiFgInvertAndReset(x, y) "\033[7;3" Str(x) "m" Str(y)ANSI_RESET
#define FormatDleLabel(x, y) _AnsiFgInvertAndReset(x, y)
#define W_PrependDle3(x) x
#define W_PrependDle2(x) W_PrependDle3(##x##)
#define _PrependDle(d, x) W_PrependDle2(d##x)
#define PrependDle(x) _PrependDle(##DLE_COLOR_,x)
// #define DLE_COLOR_ DLE_COLOR_
#define DLE_COLOR_() DLE_COLOR_
// #define __GetDLE_COLOR_(y) y
// #define _GetDLE_COLOR_(x,y) x##y##
// #define GetDLE_COLOR_(x) _GetDLE_COLOR_(DLE_COLOR_,PrependDle(x))
#define DLE_COLOR_2 DEBUG
#define DLE_COLOR_DEBUG() 2
#define DLE_COLOR_LOG() 6
#define DLE_COLOR_INFO 4
#define DLE_COLOR_WARN 3
#define DLE_COLOR_ERROR 1
#define DLE_C_DEBUG ANSI_GREEN
#define DLE_C_LOG ANSI_CYAN
#define DLE_C_INFO ANSI_BLUE
#define DLE_C_WARN ANSI_YELLOW
#define DLE_C_ERROR ANSI_RED
// PrependDle(2)
// GetDLE_COLOR_(2)
// #endregion Color Macros
struct debugOutputConfig {
	unsigned char verbosity_none;
	unsigned char verbosity_pak;
	unsigned char verbosity_floor;
	unsigned char verbosity_mask;
	unsigned char verbosity_camera;
	unsigned char verbosity_sound;
	unsigned char verbosity_itd;
	unsigned char verbosity_life;
	/// @brief A bit flag of `debugCategoryEnum`
	unsigned char debugOutputEnabled;
	/// @todo enable recursive debug toggling
	// bool recurse_none;
	// bool recurse_pak;
	// bool recurse_floor;
	// bool recurse_mask;
	// bool recurse_camera;
	// bool recurse_sound;
	// bool recurse_itd;
	// bool recurse_life;
};

typedef struct debugOutputConfig debugOutputConfig;

#define PF_LE_S8(address) "%hhi", READ_LE_S8((address)), (*((s8*)(address)))
#define PF_LE_S16(address) "%hi", READ_LE_S16((address)), (*((s16*)(address)))
#define PF_LE_S32(address) "%i", READ_LE_S32((address)), (*((s32*)(address)))
#define PF_LE_U8(address) "%hhu", READ_LE_U8((address)), (*((u8*)(address)))
#define PF_LE_U16(address) "%hu", READ_LE_U16((address)), (*((u16*)(address)))
#define PF_LE_U32(address) "%u", READ_LE_U32((address)), (*((u32*)(address)))
// PF_LE_S8(var + 4)

// #region Temp disable output
void DebugDisableOutput();
void DebugEnableOutput();
void DebugToggleOutput();
// #endregion Temp disable output

/// @brief Adds the given flags to the current category.
/// @param category The category to add
/// @return The actual added flags (excluding ones already present); use with `DebugRemoveCategory` to properly remove.
debugCategoryEnum DebugAddCategory(debugCategoryEnum category);
void DebugRemoveCategory(debugCategoryEnum category);
bool DebugBeginSection(debugCategoryEnum category); // bool DebugBeginSection(debugCategoryEnum category, debugLevelEnum level = DBO_L_NONE, const char* format = NULL, ...);
bool DebugEndSection();
bool DebugPrintfLn(debugLevelEnum level, const char* format, ...);
void DebugPrintfLnCategory(debugLevelEnum level, debugCategoryEnum category, const char* format, ...);
/// @brief Automates adding corrected & uncorrected endian representations to the output; For usage w/ `PF_LE_S8` & such
/// @tparam T 
/// @param level 
/// @param typeSpecifier 
/// @param result 
/// @param raw 
template <typename T> void DebugBPrintRaw(debugLevelEnum level, const char* typeSpecifier, T result, T raw);
void DebugBPrintf(debugLevelEnum level, const char* format, ...);
void DebugBFlushLn();
void DebugSPrintZVStruct(char* destination, ZVStruct& zv);
// void DebugSPrintZVStruct(const char* destination, ZVStruct* zv);
// #endregion Formatted Standard Out

// #region Life Script logging
/// @brief A list of descriptions for what each var is used for
/// @details Currently only supported for AITD1.
extern const char* varsNameTable[];

#define _MAX_LOGGED_LIFE_SCRIPTS 100

/// @brief Which life scripts should be output? 
extern int loggedLifeScripts[_MAX_LOGGED_LIFE_SCRIPTS];

/// @brief The total number of life scripts to log.
extern int numLoggedLifeScripts;
// #endregion Life Script logging

// #region Replay
// extern bool enableReplayRecording;
// extern char* replayEvent[];
// #endregion Replay

// #region drawRoomZv // NOTE: These refer to AITD1's palette https://kb.speeddemosarchive.com/images/a/a5/AITD_palette.png
// #define _DBG_drawRoomZv_Color_hardCol_objects 9
// #define _DBG_drawRoomZv_Color_hardCol_walls 100
// #define _DBG_drawRoomZv_Color_hardCol_dummy 180
// #define _DBG_drawRoomZv_Color_hardCol_ground_climb 70
// #define _DBG_drawRoomZv_Color_hardCol_overDoorZones 50
// #define _DBG_drawRoomZv_Color_hardCol_scenario 60
// #define _DBG_drawRoomZv_Color_hardCol_monsters 80
// #define _DBG_drawRoomZv_Color_hardCol_other 40
// #define _DBG_drawRoomZv_Color_sceZone 20
// #define _DBG_drawZv_Color 10
#define _DBG_drawMaskZone_Color 20
// #define _DBG_drawConverZone_Color 20
// #define _DBG_drawConverZone_Height -100 // 0
// #endregion drawRoomZv

// #region Emulation mode
extern bool strictEmulation;
// #endregion Emulation mode

extern bool masterEnableSound;
#endif
