
/// @def The number of items in an inventory
/// @brief The number of items in an inventory
#define INVENTORY_SIZE      30
/// @def The number of inventories; presumably for switching characters in later games.
/// @brief The number of inventories; presumably for switching characters in later games.
#define NUM_MAX_INVENTORY	2
extern s16 currentInventory;
extern s16 numObjInInventoryTable[NUM_MAX_INVENTORY];
extern s16 inHandTable[NUM_MAX_INVENTORY];
extern s16 inventoryTable[NUM_MAX_INVENTORY][INVENTORY_SIZE];

extern int statusLeft;
extern int statusTop;
extern int statusRight;
extern int statusBottom;

void processInventory(void);
