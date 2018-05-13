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
#include "shader-quad.h"
#include "renderer.h"
#include "texture-manager.h"



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

	//triangle test
	const Vec4 red = vec4(1, 0, 0, 1);
	static Vertex triangle[3];

	static Vec2 positions[3] = { vec2(5, 2), vec2(0, 11), vec2(4, 2) }; //colinear lol
	gl::renderer::convert_coords_to_gl_space(vec2(10.0f, 10.0f), positions, 3);

	for (int i = 0; i < 3; i++)
		triangle[i] = {positions[i], vec2(0, 0), red};

	gl::shader::triangle::update_data(triangle, 3);


	//quad test
	Vec2 q_pos = vec2(0.0f, 5.0f);
	//gl::renderer::convert_coords_to_gl_space(vec2(20.0f, 20.0f), &q_pos, 1); //put this in shader

	Vec2 q_wh = vec2(10.0f, 10.0f); //this also must be converted to gl coords!
	gl::texture::Texture q_tex = gl::texture::make_simple_texture("../data/textures/sample_atlas.png");

	gl::shader::quad::set_count(1);
	gl::shader::quad::update_data_coord(&q_pos);
	gl::shader::quad::update_data_wh(&q_wh);
	gl::shader::quad::update_data_tex(&q_tex);


	while (sdl::io::user_quit != true)
	{
		sdl::io::update();

		if (sdl::io::key_held[KEY_w]) q_pos.y += 0.001;
		if (sdl::io::key_held[KEY_s]) q_pos.y -= 0.001;
		if (sdl::io::key_held[KEY_a]) q_pos.x -= 0.001;
		if (sdl::io::key_held[KEY_d]) q_pos.x += 0.001;

		gl::shader::quad::set_count(1);
		gl::shader::quad::update_data_coord(&q_pos);

		gl::renderer::clear_screen();
		gl::shader::draw_all();
		gl::renderer::swap_framebuffers(window_handle);

		if (sdl::io::mouse_key_pressed[MOUSE_RIGHT])
			printf("Mouse pos: %f, %f", sdl::io::mouse_pos.x, sdl::io::mouse_pos.y);

	}
	

	sdl::cleanup(window_handle, gl_context);

	return 0;
}