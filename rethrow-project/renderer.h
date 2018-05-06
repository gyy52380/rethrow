#pragma once

#include <SDL2/SDL.h>

#include "typedef.h"
#include "Vector.h"


namespace gl::renderer
{

	void clear_screen();
	void swap_framebuffers(SDL_Window* window_handle);
	void convert_coords_to_gl_space(Vec2 max_screen_wh, Vec2 *data, u32 count);

}