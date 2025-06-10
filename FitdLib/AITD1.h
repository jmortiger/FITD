#ifndef _AITD1_H_
#define _AITD1_H_
/// @brief AITD1 ITD_RESS.PAK file index mapping
/// @todo replace macros w/ enum equivalents
enum ItdRessIdx1 {
	/// @brief Armadillo Model
	RESS1_TATOU_3DO		= 0,
	/// @brief Armadillo Palette
	RESS1_TATOU_PAL		= 1,
	/// @brief Armadillo 2D Image
	RESS1_TATOU_MCG		= 2,
	RESS1_PALETTE_JEU	= 3,
	/// @brief Frame
	RESS1_CADRE_SPF		= 4,
	/// @brief Font gfx
	RESS1_ITDFONT		= 5,
	/// @brief Letter reading background image
	RESS1_LETTRE		= 6,
	/// @brief Book reading background image
	RESS1_LIVRE			= 7,
	/// @brief Notebook reading background image
	RESS1_CARNET		= 8,
	/// @brief Book artwork
	RESS1_TEXT_GRAPH	= 9,
	RESS1_PERSO_CHOICE	= 10,
	/// @brief Frog
	RESS1_GRENOUILLE	= 11,
	RESS1_DEAD_END		= 12,
	/// @brief Title
	RESS1_TITRE			= 13,
	/// @brief Bottom Intro (?)
	RESS1_FOND_INTRO	= 14,
	/// @brief Outside Cam (Special Camera)
	RESS1_CAM07000		= 15,
	/// @brief Outside Cam (Special Camera)
	RESS1_CAM07001		= 16,
	/// @brief (Special Camera)
	RESS1_CAM06000		= 17,
	/// @brief (Special Camera)
	RESS1_CAM06005		= 18,
	/// @brief (Special Camera)
	RESS1_CAM06008		= 19,
}; typedef enum ItdRessIdx1 ItdRessIdx1;

/// @brief The valid selections on the initial startup menu.
enum StartupMenuOptionsAITD1 {
	/// @brief Timeout; start demos
	SMO1_TIMEOUT = -1,
	/// @brief New game
	SMO1_NEW = 0,
	/// @brief Load game
	SMO1_LOAD = 1,
	/// @brief Exit game
	SMO1_EXIT = 2,
}; typedef enum StartupMenuOptionsAITD1 StartupMenuOptionsAITD1;

/// @brief The types of documents you can read; refers to the background image. Add `6` to get the relevant index in `ITD_RESS.PAK`.
enum DocumentTypeAITD1 {
	/// @brief Letter
	DT1_LETTER = 0,
	/// @brief Book
	DT1_BOOK = 1,
	/// @brief Notebook
	DT1_NOTEBOOK = 2,

	/// @brief Letter
	DT1_MESSAGE = DT1_LETTER,
	/// @brief Letter
	DT1_LETTRE = DT1_LETTER,
	/// @brief Book
	DT1_LIVRE = 1,
	/// @brief Notebook
	DT1_CARNET = 2,
}; typedef enum DocumentTypeAITD1 DocumentTypeAITD1;

#define AITD1_AUTO_SCROLL_TIME 300

void startAITD1();
void AITD1_ReadBook(int index, int type);
#endif
