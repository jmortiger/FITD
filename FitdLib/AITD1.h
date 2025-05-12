#ifndef _AITD1_H_
#define _AITD1_H_
// ITD_RESS mapping
#define AITD1_TATOU_3DO		0 // Armadillo (3DO)
#define AITD1_TATOU_PAL		1 // Armadillo (PAL)
#define AITD1_TATOU_MCG		2 // Armadillo (MCG) (?)
#define AITD1_PALETTE_JEU	3
#define AITD1_CADRE_SPF		4 // Frame
#define AITD1_ITDFONT		5
#define AITD1_LETTRE		6 // Letter 
#define AITD1_LIVRE			7 // Book
#define AITD1_CARNET		8 // Notebook 
#define AITD1_TEXT_GRAPH	9
#define AITD1_PERSO_CHOICE	10
#define AITD1_GRENOUILLE	11 // Frog
#define AITD1_DEAD_END		12
#define AITD1_TITRE			13 // Title
#define AITD1_FOND_INTRO	14 // Bottom Intro (?)
#define AITD1_CAM07000		15 // Outside Cam (Special Camera)
#define AITD1_CAM07001		16 // Outside Cam (Special Camera)
#define AITD1_CAM06000		17 // (Special Camera)
#define AITD1_CAM06005		18 // (Special Camera)
#define AITD1_CAM06008		19 // (Special Camera)

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

void startAITD1();
void AITD1_ReadBook(int index, int type);
#endif
