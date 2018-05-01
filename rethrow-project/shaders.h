#pragma once

#include <SDL2\SDL.h>
#include "Vertex.h"

namespace gl
{


void init_shaders();
void swap_frame_buffer(SDL_Window *window_handle);
void draw_shader_triangle_ct(Vertex *data, int len_bytes);

}
