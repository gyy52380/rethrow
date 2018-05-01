#include "sdl.h"

#include <SDL2/SDL.h>
#include <cstdio>

#include "typedef.h"

namespace sdl
{


bool init(SDL_Window* &window_handle, SDL_GLContext &gl_context, const u32 SCREEN_WIDTH, const u32 SCREEN_HEIGHT)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL2 failed to initialize!, SDL error: %s\n", SDL_GetError());
		return false;
	}
		

	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1); //MSAA
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4); //MSAA
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetSwapInterval(1); //vsync

#ifdef GL_DEBUG
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
#endif

	window_handle = SDL_CreateWindow(	"Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT,
										SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (!window_handle)
		{
		printf("SDL2 failed to create a window!, SDL error: %s\n", SDL_GetError());
		return false;
		}

	gl_context = SDL_GL_CreateContext(window_handle);
	if (!gl_context)
	{
		printf("SDL2 failed to create a gl context!, SDL error: %s\n", SDL_GetError());
		return false;
	}

	return true;
}

void cleanup(SDL_Window* &window_handle, SDL_GLContext &gl_context)
{
	SDL_GL_DeleteContext(gl_context);
	SDL_DestroyWindow(window_handle);
	SDL_Quit();
	exit(0);
}


}