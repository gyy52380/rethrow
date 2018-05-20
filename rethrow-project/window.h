#pragma once

#include <SDL2/SDL.h>


namespace window
{
	extern SDL_Window* the_window;
	extern SDL_GLContext the_gl_context;

	extern int screen_width;
	extern int screen_height;

	bool init();
	void cleanup();

	void update_screen_size();
	void swap_buffers();
}