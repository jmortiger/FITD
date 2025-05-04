#ifndef _DEBUGGER_H_
#define _DEBUGGER_H_

#ifdef FITD_DEBUGGER

////// debug var used in engine
extern bool debuggerVar_drawModelZv;
extern bool debuggerVar_drawCameraCoverZone;
extern bool debuggerVar_noHardClip;
extern bool debuggerVar_topCamera;
extern long int debufferVar_topCameraZoom;

extern bool debuggerVar_useBlackBG;
extern bool debuggerVar_fastForward;
///////////////////////////////

void debugger_draw(void);
#endif // INTERNAL_DEBUGGER

/// @brief Read the command-line parameters and accordingly set the debug output.
/// @param argc 
/// @param argv 
/// @todo Support flag to print palette
void parseDebugParam(int argc, char* argv[]);

/// @brief The categories of debug info supported, as a bit flag.
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

// debugCategoryEnum debugCategoryEnumFlags[] = {
// 	DBO_NONE,
// 	DBO_PAK,
// 	DBO_FLOOR,
// 	DBO_MASK,
// 	DBO_CAMERA,
// 	DBO_SOUND,
// 	DBO_ITD,
// };

/// @brief The debug levels supported, as a bit flag.
enum debugLevelEnum : unsigned char {
	DBO_L_NONE	= 0b0000'0000,
	DBO_L_DEBUG	= 0b0000'0001,
	DBO_L_LOG	= 0b0000'0010,
	DBO_L_INFO	= 0b0000'0100,
	DBO_L_WARN	= 0b0000'1000,
	DBO_L_ERROR	= 0b0001'0000,
};
#define DBO_L_ALL 0b0001'1111

typedef enum debugLevelEnum debugLevelEnum;

// Foreground Dark: 3x
// Foreground Light: 9x
// Background Dark: 4x
// Background Light: 10x
// 0: Black
// 1: Red
// 2: Green
// 3: Yellow
// 4: Blue
// 5: Magenta
// 6: Cyan
// 7: White
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

/* #define _PF_LE_S8(address, result, raw) "hhi", result, raw
#define _PF_SPEC_LE_S8 "hhi"
#define _PF_VAL_LE_S8(address) *(s8*)(address)
#define PF_LE_S8(address) _PF_LE_S8(READ_LE_S8((address)))
// #define PF_LE_S8(address) "hhi", READ_LE_S8((address)), *(s8*)(address)
#define _PF_LE_S16(address, result, raw) "hi", result, raw
#define _PF_SPEC_LE_S16 "hi"
#define _PF_VAL_LE_S16(address) *(s16*)(address)
#define PF_LE_S16(address) _PF_LE_S16(READ_LE_S16((address)))
// #define PF_LE_S16(address) "hi", READ_LE_S16((address)), *(s16*)(address)
#define _PF_LE_S32(address, result, raw) "i", result, raw
#define _PF_SPEC_LE_S32 "i"
#define _PF_VAL_LE_S32(address) *(s32*)(address)
#define PF_LE_S32(address) _PF_LE_S32(READ_LE_S32((address)))
// #define PF_LE_S32(address) "i", READ_LE_S32((address)), *(s32*)(address)
#define _PF_LE_U8(address, result, raw) "hhu", result, raw
#define _PF_SPEC_LE_U8 "hhu"
#define _PF_VAL_LE_U8(address) *(u8*)(address)
#define PF_LE_U8(address) _PF_LE_U8(READ_LE_U8((address)))
// #define PF_LE_U8(address) "hhu", READ_LE_U8((address)), *(u8*)(address)
#define _PF_LE_U16(address, result, raw) "hu", result, raw
#define _PF_SPEC_LE_U16 "hu"
#define _PF_VAL_LE_U16(address) *(u16*)(address)
#define PF_LE_U16(address) _PF_LE_U16(READ_LE_U16((address)))
// #define PF_LE_U16(address) "hu", READ_LE_U16((address)), *(u16*)(address)
#define _PF_LE_U32(address, result, raw) "u", result, raw
#define _PF_SPEC_LE_U32 "u"
#define _PF_VAL_LE_U32(address) *(u32*)(address)
#define PF_LE_U32(address) _PF_LE_U32(READ_LE_U32((address)))
// #define PF_LE_U32(address) "u", READ_LE_U32((address)), *(u32*)(address) */
/* #define _PF_LE_S8(result, address) "hhi", result, *(s8*)(address)
#define PF_LE_S8(address) _PF_LE_S8(READ_LE_S8((address)), address)
#define _PF_LE_S16(result, address) "hi", result, *(s16*)(address)
#define PF_LE_S16(address) _PF_LE_S16(READ_LE_S16((address)), address)
#define _PF_LE_S32(result, address) "i", result, *(s32*)(address)
#define PF_LE_S32(address) _PF_LE_S32(READ_LE_S32((address)), address)
#define _PF_LE_U8(result, address) "hhu", result, *(u8*)(address)
#define PF_LE_U8(address) _PF_LE_U8(READ_LE_U8((address)), address)
#define _PF_LE_U16(result, address) "hu", result, *(u16*)(address)
#define PF_LE_U16(address) _PF_LE_U16(READ_LE_U16((address)), address)
#define _PF_LE_U32(result, address) "u", result, *(u32*)(address)
#define PF_LE_U32(address) _PF_LE_U32(READ_LE_U32((address)), address) */

#define PF_LE_S8(address) "%hhi", READ_LE_S8((address)), (*((s8*)(address)))
#define PF_LE_S16(address) "%hi", READ_LE_S16((address)), (*((s16*)(address)))
#define PF_LE_S32(address) "%i", READ_LE_S32((address)), (*((s32*)(address)))
#define PF_LE_U8(address) "%hhu", READ_LE_U8((address)), (*((u8*)(address)))
#define PF_LE_U16(address) "%hu", READ_LE_U16((address)), (*((u16*)(address)))
#define PF_LE_U32(address) "%u", READ_LE_U32((address)), (*((u32*)(address)))
// PF_LE_S8(var + 4)

/// @brief Adds the given flags to the current category.
/// @param category The category to add
/// @return The actual added flags (excluding ones already present); use with `DebugRemoveCategory` to properly remove.
debugCategoryEnum DebugAddCategory(debugCategoryEnum category);
void DebugRemoveCategory(debugCategoryEnum category);
bool DebugBeginSection(debugCategoryEnum category); // bool DebugBeginSection(debugCategoryEnum category, debugLevelEnum level = DBO_L_NONE, const char* format = NULL, ...);
bool DebugEndSection();
void DebugPrintfLn(debugLevelEnum level, const char* format, ...);
void DebugPrintfLnCategory(debugLevelEnum level, debugCategoryEnum category, const char* format, ...);
/// @brief For usage w/ `PF_LE_S8` & such
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

extern const char* varsNameTable[];
#endif
