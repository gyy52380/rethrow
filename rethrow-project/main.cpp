//#define GL_DEBUG

#include <cstdio>

#include "typedef.h"
#include "gl.h"
#include "sdl.h"
#include "io.h"

#include "Vector.h"
#include "Vertex.h"
#include "shader-manager.h"
#include "shader-triangle.h"
#include "renderer.h"



SDL_Window *window_handle;
SDL_GLContext gl_context;

constexpr u32 SCREEN_WIDTH  = 640;
constexpr u32 SCREEN_HEIGHT = 480;


#undef main
int main(int argc, char *argv[])
{
	bool backend_init_status = sdl::init(window_handle, gl_context, SCREEN_WIDTH, SCREEN_HEIGHT);
	if (backend_init_status == false)
	{
		printf("Backend (SDL2) failed to initalize!\n");
		sdl::cleanup(window_handle, gl_context);
		return -1;
	}
	bool gl_init_status = gl::gl_init();
	if (gl_init_status == false)
	{
		printf("OpenGL failed to initalize!\n");
		sdl::cleanup(window_handle, gl_context);
		return -1;
	}


	const Vec4 red = vec4(1, 0, 0, 1);
	static Vertex triangle[3];
	triangle[0].position = vec2(0, 1);
	triangle[1].position = vec2(1, 0);
	triangle[2].position = vec2(-1, 0);

	triangle[0].color = red;
	triangle[1].color = red;
	triangle[2].color = red;

	gl::shader::triangle::update_data(triangle, 3);

	while (sdl::io::user_quit != true)
	{
		sdl::io::update();

		gl::renderer::clear_screen();
		gl::shader::draw_all();
		gl::renderer::swap_framebuffers(window_handle);

	}
	

	sdl::cleanup(window_handle, gl_context);

	return 0;
}