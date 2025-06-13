/// @brief AITD2 ITD_RESS.PAK file index mapping
enum ItdRessIdx2 {
	RESS2_CADRE_SPF						= 0,
	/// @brief Font gfx
	RESS2_ITDFONT						= 1,
	/// @brief Letter reading background image
	RESS2_LETTRE						= 2,
	/// @brief Book reading background image
	RESS2_LIVRE							= 3,
	/// @brief Notebook reading background image
	RESS2_CARNET						= 4,
	RESS2_CYM11001						= 5,
	RESS2_CYM11011						= 6,
	RESS2_CYM11012						= 7,
	RESS2_CYM11013						= 8,
	RESS2_PRO07011						= 9,
	RESS2_PRO07012						= 10,
	RESS2_PRO07013						= 11,
	RESS2_PRO08001						= 12,
	RESS2_PRO08008						= 13,
	RESS2_INVENTAIRE_PIRATE				= 14,
	RESS2_INVENTAIRE_GANG				= 15,
	RESS2_INVENTAIRE_GRACE				= 16,
	RESS2_OPTION_SCREEN					= 17,
	RESS2_SPRITES_INVENTAIRE			= 18,

	RESS2_INVENTAIRE_OPT0				= 14,
	RESS2_INVENTAIRE_OPT1				= 15,
	RESS2_INVENTAIRE_OPT2				= 16,
}; typedef enum ItdRessIdx2 ItdRessIdx2;

#define AITD2_INVENTORY_CAM_X	128
#define AITD2_INVENTORY_CAM_Y	400
#define AITD2_INVENTORY_CAM_Z	390
#define AITD2_STATUS_LEFT		27
#define AITD2_STATUS_TOP		100
#define AITD2_STATUS_RIGHT		159
#define AITD2_STATUS_BOTTOM		174

void startAITD2();
void drawInventoryAITD2();
void redrawInventorySpriteAITD2();
void AITD2_ReadBook(int index, int type);

extern int AITD2MusicToTrackMapping[21];
