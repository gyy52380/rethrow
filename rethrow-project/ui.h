#pragma once

#include <SDL2/SDL.h>

namespace ui
{
	void init();
	void cleanup();
	void handle_events(SDL_Event* event);

	void begin_frame();
	void end_frame();

	extern bool ui_owns_keyboard;
	extern bool ui_owns_mouse;
}