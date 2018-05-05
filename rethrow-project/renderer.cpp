#include "renderer.h"

#include <GL/glew.h>
#include <SDL2/SDL.h>


namespace gl::renderer
{


	void clear_screen()
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void swap_framebuffers(SDL_Window *window_handle)
	{
		SDL_GL_SwapWindow(window_handle);
	}

}