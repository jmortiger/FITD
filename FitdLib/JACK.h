
/// @brief Jack in the Dark ITD_RESS.PAK file index mapping
enum ItdRessIdxJ {
	JACK_CADRE_SPF						= 0,
	/// @brief Font gfx
	JACK_ITDFONT						= 1,
	/// @brief Book reading background image
	JACK_LIVRE							= 2,
	JACK_IM_EXT_JACK					= 3,
}; typedef enum ItdRessIdxJ ItdRessIdxJ;

void startJACK();
void JACK_ReadBook(int index, int type);
