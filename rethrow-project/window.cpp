#include "window.h"

#include <SDL2/SDL.h>
#include <cstdio>

#include "gl.h"
#include "ui.h"


namespace window
{


	SDL_Window* the_window;
	SDL_GLContext the_gl_context;

	//initial wh
	int screen_width = 640;
	int screen_height = 480;

	bool init()
	{
		//init sdl
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


		the_window = SDL_CreateWindow(	"Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_width, screen_height,
										SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
		if (!the_window)
		{
			printf("SDL2 failed to create a window!, SDL error: %s\n", SDL_GetError());
			return false;
		}

		the_gl_context = SDL_GL_CreateContext(the_window);
		if (!the_gl_context)
		{
			printf("SDL2 failed to create a gl context!, SDL error: %s\n", SDL_GetError());
			return false;
		}

		if (!gl::init())  //internal error handling
			return false;

		//ui::init(); ...

		return true;
	}

	void cleanup()
	{
		SDL_GL_DeleteContext(the_gl_context);
		SDL_DestroyWindow(the_window);

		//ui::cleanup(); ...
		SDL_Quit();
	}

	void update_screen_size()
	{
		SDL_GetWindowSize(the_window, &screen_width, &screen_height);
	}

	void swap_buffers()
	{
		SDL_GL_SwapWindow(the_window);
	}


}