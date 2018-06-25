//#define GL_DEBUG

#include <cstdio>

#include "typedef.h"
#include "window.h"
#include "io.h"

#include "Vector.h"
#include "Vertex.h"
#include "shader-manager.h"
#include "shader-triangle.h"
#include "shader-quad.h"
#include "shader-indexed-triangle.h"
#include "renderer.h"
#include "texture-manager.h"

#include "gl.h"

#include "ui.h"
#include "lib/imgui/imgui.h"


#undef main
int main(int argc, char *argv[])
{
	bool window_init_status = window::init();
	if (window_init_status == false)
	{
		printf("Window and backend (SDL2) failed to initalize!\n");
		window::cleanup();
		return -1;
	}

	//triangle test
	static const Vec4_byte red = vec4_byte(255, 0, 0, 255);
	static Vertex triangle[3];

	static Vec2 positions[3] = { vec2(5, 2), vec2(0, 11), vec2(4, 2) }; //colinear lol
	gl::renderer::convert_coords_to_gl_space(vec2(10.0f, 10.0f), positions, 3);

	for (int i = 0; i < 3; i++)
		triangle[i] = {positions[i], vec2(0, 0), red};

	gl::shader::triangle::update_data(triangle, 3);


	//quad test
	Vec2 q_pos = vec2(0.0f, 5.0f);
	Vec2 q_wh = vec2(10.0f, 10.0f);

	gl::texture::Texture q_tex = gl::texture::make_simple_texture("../data/textures/sample_atlas.png");

	gl::shader::quad::set_count(1);
	gl::shader::quad::update_data_coord(&q_pos);
	gl::shader::quad::update_data_wh(&q_wh);
	gl::shader::quad::update_data_tex(&q_tex);

	//indexed triangle test
	static const Vec4_byte blue = vec4_byte(0, 255, 0, 255);
	static Vertex blue_quad[4] = {
		{{0.0f, 0.0f}, 		{0.0f, 0.0f}, blue},
		{{100.0f, 0.0f}, 	{1.0f, 0.0f}, blue},
		{{0.0f, 100.0f}, 	{0.0f, 1.0f}, blue},
		{{100.0f, 100.0f}, 	{1.0f, 1.0f}, blue}
	};

	static u16 indices[6] = {0, 1, 2, 1, 2, 3};

	gl::shader::indexed_triangle::set_vertex_data(blue_quad, sizeof(blue_quad));
	gl::shader::indexed_triangle::set_index_data(indices, sizeof(indices));

	while (io::user_quit != true)
	{
		io::update();

		////
		ui::begin_frame();
		ImGui::Begin("test", NULL, ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::Text("Hello, world %d", 123);
		static float f = 5;
		if (ImGui::Button("Save"))
		{
		    f += 0.5f;
		}

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
		////

		if (io::key_held[KEY_w]) q_pos.y += 0.01f;
		if (io::key_held[KEY_s]) q_pos.y -= 0.01f;
		if (io::key_held[KEY_a]) q_pos.x -= 0.01f;
		if (io::key_held[KEY_d]) q_pos.x += 0.01f;

		gl::shader::quad::set_count(1);
		gl::shader::quad::update_data_coord(&q_pos);

		gl::renderer::clear_screen();
		gl::shader::draw_all();

		gl::shader::indexed_triangle::draw(6, 0);

		ui::end_frame();
		window::swap_buffers();

		if (io::key_released[KEY_q])
			printf("Screen wh: %i, %i. Drawable wh: %i, %i\n",
				window::screen_width, window::screen_height, window::drawable_width, window::drawable_height);

	}
	

	window::cleanup();

	return 0;
}