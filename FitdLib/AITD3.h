
// TODO: Convert to enum
#define AITD3_CADRE_SPF						0
#define AITD3_ITDFONT						1 // Font gfx
#define AITD3_LETTRE						2 // Letter reading background image
#define AITD3_LIVRE							3 // Book reading background image
#define AITD3_CARNET						4 // Notebook reading background image
#define AITD3_CYM00001						5
#define AITD3_CYM00007						6
#define AITD3_CYM00013						7
#define AITD3_CYM00014						8
#define AITD3_CYM01017						9
#define AITD3_CYM03011						10
#define AITD3_CYM13005						11
#define AITD3_CYM13013						12
#define AITD3_MENU3							13
#define AITD3_INVENTAIRE_CAVERNE			14
#define AITD3_INVENTAIRE_COWBOY				15
#define AITD3_INVENTAIRE_COUGUAR			16
#define AITD3_OPTION_SCREEN					17
#define AITD3_SPRITES_INVENTAIRE_CAVERNE	18
#define AITD3_SPRITES_INVENTAIRE_COWBOY		19
#define AITD3_SPRITES_INVENTAIRE_COUGUAR	20
/// @brief AITD2 ITD_RESS.PAK file index mapping
enum ItdRessIdx2 {
	RESS3_CADRE_SPF						= 0,
	/// @brief Font gfx
	RESS3_ITDFONT						= 1,
	/// @brief Letter reading background image
	RESS3_LETTRE						= 2,
	/// @brief Book reading background image
	RESS3_LIVRE							= 3,
	/// @brief Notebook reading background image
	RESS3_CARNET						= 4,
	RESS3_CYM00001						= 5,
	RESS3_CYM00007						= 6,
	RESS3_CYM00013						= 7,
	RESS3_CYM00014						= 8,
	RESS3_CYM01017						= 9,
	RESS3_CYM03011						= 10,
	RESS3_CYM13005						= 11,
	RESS3_CYM13013						= 12,
	RESS3_MENU3							= 13,
	/// @details Same index as AITD2
	RESS3_INVENTAIRE_CAVERNE			= 14,
	/// @details Same index as AITD2
	RESS3_INVENTAIRE_COWBOY				= 15,
	/// @details Same index as AITD2
	RESS3_INVENTAIRE_COUGUAR			= 16,
	/// @details Same index as AITD2
	RESS3_OPTION_SCREEN					= 17,
	RESS3_SPRITES_INVENTAIRE_CAVERNE	= 18,
	RESS3_SPRITES_INVENTAIRE_COWBOY		= 19,
	RESS3_SPRITES_INVENTAIRE_COUGUAR	= 20,

	/// @details Same index as AITD2
	RESS2_INVENTAIRE_OPT0				= RESS3_INVENTAIRE_CAVERNE,
	/// @details Same index as AITD2
	RESS2_INVENTAIRE_OPT1				= RESS3_INVENTAIRE_COWBOY,
	/// @details Same index as AITD2
	RESS2_INVENTAIRE_OPT2				= RESS3_INVENTAIRE_COUGUAR,
}; typedef enum ItdRessIdx2 ItdRessIdx2;

void startAITD3();
void drawInventoryAITD3();
// void getStageFile(char* buffer, int floorNumber);
