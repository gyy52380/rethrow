#pragma once

#include <SDL2/SDL.h>

#include "typedef.h"


namespace sdl
{

//int main(int argc, char *argv[]);
bool init(SDL_Window* &the_window_handle, SDL_GLContext &the_gl_context, const u32 SCREEN_WIDTH, const u32 SCREEN_HEIGHT);
void cleanup(SDL_Window* &the_window_handle, SDL_GLContext &the_gl_context);


}
