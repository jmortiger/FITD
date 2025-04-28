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
};
#define DBO_ALL 0b0001'1111

typedef enum debugCategoryEnum debugCategoryEnum;

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

struct debugOutputConfig {
	unsigned char verbosity_none;
	unsigned char verbosity_pak;
	unsigned char verbosity_floor;
	unsigned char verbosity_mask;
	unsigned char verbosity_camera;
	unsigned char verbosity_sound;
	/// @brief A bit flag of `debugCategoryEnum`
	unsigned char debugOutputEnabled;
};

typedef struct debugOutputConfig debugOutputConfig;

/// @brief Adds the given flags to the current category.
/// @param category The category to add
/// @return The actual added flags (excluding ones already present); use with `DebugRemoveCategory` to properly remove.
debugCategoryEnum DebugAddCategory(debugCategoryEnum category);
void DebugRemoveCategory(debugCategoryEnum category);
bool DebugBeginSection(debugCategoryEnum category); // bool DebugBeginSection(debugCategoryEnum category, debugLevelEnum level = DBO_L_NONE, const char* format = NULL, ...);
bool DebugEndSection();
void DebugPrintfLn(debugLevelEnum level, const char* format, ...);
#endif
