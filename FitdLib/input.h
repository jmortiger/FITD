#ifndef _INPUT_H_
#define _INPUT_H_

extern "C" {
	void readKeyboard(void);
}

/// @brief Used to allow easy input mapping
/// @todo Implement
/// @todo Finish
enum gameInputs {
	INPUT_SPACE = 1,
	INPUT_RETURN,
	INPUT_ESC,
	INPUT_UP, // UP
	INPUT_DOWN, // DOWN
	INPUT_LEFT, // LEFT
	INPUT_RIGHT, // RIGHT
	INPUT_SFX, // S
	INPUT_MUSIC, // M
	INPUT_PAUSE, // P
	INPUT_INVENTORY // I
}; typedef enum gameInputs gameInputs;

/// @brief Used to allow any input to trigger specific actions
/// @todo Implement
/// @todo Finish
enum gameInputsGeneric {
	INPUT_G_NONE,
	INPUT_G_ACTION,
	INPUT_G_MOVE_FORWARD,
	INPUT_G_MOVE_BACKWARD,
	INPUT_G_MOVE_LEFT,
	INPUT_G_MOVE_RIGHT,
	INPUT_G_JUMP,
	INPUT_G_OPEN_INVENTORY,
	INPUT_G_OPEN_MENU,
	INPUT_G_TOGGLE_SFX,
	INPUT_G_TOGGLE_MUSIC,
	INPUT_G_TOGGLE_PAUSE,
	INPUT_G_MENU_CONFIRM,
	INPUT_G_MENU_CANCEL,
	INPUT_G_MENU_UP,
	INPUT_G_MENU_DOWN,
	INPUT_G_MENU_LEFT,
	INPUT_G_MENU_RIGHT,
	INPUT_G_FIGHT_UP,
	INPUT_G_FIGHT_DOWN,
	INPUT_G_FIGHT_LEFT,
	INPUT_G_FIGHT_RIGHT,

	INPUT_G_TURN_LEFT = INPUT_G_MOVE_LEFT,
	INPUT_G_TURN_RIGHT = INPUT_G_MOVE_RIGHT,
	INPUT_G_AIM_LEFT = INPUT_G_FIGHT_LEFT,
	INPUT_G_AIM_RIGHT = INPUT_G_FIGHT_RIGHT,
	INPUT_G_HOP = INPUT_G_JUMP,
	INPUT_G_LEAP = INPUT_G_JUMP,

	INPUT_G_ACTION_SEARCH = INPUT_G_NONE, // Switch action to SEARCH w/o opening the menu; would require a lot of work
	INPUT_G_ACTION_FIGHT = INPUT_G_NONE, // Switch action to FIGHT w/o opening the menu; would require a lot of work
	INPUT_G_ACTION_PUSH = INPUT_G_NONE, // Switch action to PUSH w/o opening the menu; would require a lot of work
	INPUT_G_ACTION_JUMP = INPUT_G_NONE, // Switch action to JUMP w/o opening the menu; would require a lot of work
	INPUT_G_ACTION_RELOAD = INPUT_G_NONE // Switch action to RELOAD w/o opening the menu; would require a lot of work
}; typedef enum gameInputsGeneric gameInputsGeneric;

#endif
