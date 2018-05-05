#pragma once

#include <SDL2/SDL.h>


namespace gl::renderer
{

	void clear_screen();
	void swap_framebuffers(SDL_Window *window_handle);

}