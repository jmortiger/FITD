#include "common.h"
#include <SDL.h>
#include <backends/imgui_impl_sdl3.h>

extern float nearVal;
extern float farVal;
extern float cameraZoom;
extern float fov;

extern bool debuggerVar_debugMenuDisplayed;

void handleKeyDown(SDL_Event& event)
{
	switch (event.key.key) {
		case SDL_SCANCODE_GRAVE:
			debuggerVar_debugMenuDisplayed ^= 1;
			break;
	}
}

bool wasMouseClicked = false;
void handleButtonDown(SDL_Event& event) {
	switch (event.button.button) {
		case SDL_BUTTON_LEFT:
			wasMouseClicked = true;
			break;
	}
}

void readKeyboard(void)
{
	SDL_Event event;
	int size;
	int j;
	const bool* keyboard;

	JoyD = 0;
	Click = 0;
	key = 0;

	while (SDL_PollEvent(&event)) {

		ImGui_ImplSDL3_ProcessEvent(&event);

		switch (event.type) {
			case SDL_EVENT_KEY_DOWN:
				handleKeyDown(event);
				break;
			case SDL_EVENT_MOUSE_BUTTON_DOWN:
				handleButtonDown(event);
				break;
			case SDL_EVENT_QUIT:
				cleanupAndExit();
				break;
		}

	}

	debuggerVar_fastForward = false;

	keyboard = SDL_GetKeyboardState(&size);

	for (j = 0; j < size; j++) {
		if (keyboard[j]) {
			switch (j) {
				/*        case SDLK_z:
				nearVal-=1;
				break;
				case SDLK_x:
				nearVal+=1;
				break;
				case SDLK_a:
				cameraZoom-=100;
				break;
				case SDLK_s:
				cameraZoom+=100;
				break;
				case SDLK_w:
				{
				float factor = (float)cameraY/(float)cameraZ;
				cameraY+=1;
				cameraZ=(float)cameraY/factor;
				break;
				}
				case SDLK_q:
				{
				float factor = (float)cameraY/(float)cameraZ;
				cameraY-=1;
				cameraZ=(float)cameraY/factor;
				break;
				}
				case SDLK_e:
				fov-=1;
				break;
				case SDLK_r:
				fov+=2;
				break; */
				case SDL_SCANCODE_RETURN:
					key = 0x1C;
					break;
				case SDL_SCANCODE_ESCAPE:
				// case SDL_SCANCODE_P: // TODO: Is the `P` pause menu & the `Esc` pause menu different?
					key = 0x1B;
					break;
				case SDL_SCANCODE_UP:
				case SDL_SCANCODE_W:
					JoyD |= 1;
					break;

				case SDL_SCANCODE_DOWN:
				case SDL_SCANCODE_S:
					JoyD |= 2;
					break;

				case SDL_SCANCODE_RIGHT:
				case SDL_SCANCODE_D:
					JoyD |= 8;
					break;

				case SDL_SCANCODE_LEFT:
				case SDL_SCANCODE_A:
					JoyD |= 4;
					break;
				case SDL_SCANCODE_SPACE:
					Click = 1;
					break;
#ifdef FITD_DEBUGGER
				case SDL_SCANCODE_O:
					debufferVar_topCameraZoom += 100;
					break;
				case SDL_SCANCODE_P:
					debufferVar_topCameraZoom -= 100;
					break;
				case SDL_SCANCODE_T:
					debuggerVar_topCamera = true;
					// backgroundMode = backgroundModeEnum_3D; // This is redundant; it'll get overwritten later anyways & doesn't stop the other keys changing it.
					break;
				case SDL_SCANCODE_Y:
					debuggerVar_topCamera = false;
					backgroundMode = backgroundModeEnum_2D;
					flagInitView = 1; // ???
					break;
				case SDL_SCANCODE_C:
					debuggerVar_noHardClip = !debuggerVar_noHardClip;
					break;
				case SDL_SCANCODE_B:
					backgroundMode = backgroundModeEnum_3D;
					break;
				case SDL_SCANCODE_N:
					backgroundMode = backgroundModeEnum_2D;
					break;
				case SDL_SCANCODE_F:
					debuggerVar_fastForward = true;
					break;


#endif
			}
		}
	}
#ifdef FITD_DEBUGGER
	if (debuggerVar_topCamera) {
		backgroundMode = backgroundModeEnum_3D;
	}
#endif
	if (wasMouseClicked) {
		wasMouseClicked = false;
		Click = 1;
	}
}
