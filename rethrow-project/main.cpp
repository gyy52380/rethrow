//#define GL_DEBUG

#include <cstdio>

#include "typedef.h"
#include "window.h"
#include "io.h"

#include "Vector.h"
#include "Vertex.h"
#include "shaders.h"
#include "macros.h"
#include "gl.h"

#include "ui.h"
#include "lib/imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>


#undef main
int main(int argc, char *argv[])
{
	bool window_inited = window::init();
	if (!window_inited)
	{
		printf("Window and backend (SDL2) failed to initalize!\n");
		window::cleanup();
		return -1;
	}


	//quad test
	Vec2 q_pos = vec2(0.0f, 5.0f);
	Vec2 q_wh = vec2(10.0f, 10.0f);

	gl::Texture q_tex;
	gl::make_texture_from_file(&q_tex, "../data/textures/sample_atlas.png");

	gl::AtlasComponent texture;
	texture.atlasID = q_tex.ID;
	texture.uv_position = vec2(0.3, 0.2);
	texture.uv_wh = vec2(0.7, 0.4);

	gl::shader::quad::set_position_data(&q_pos, 1);
	gl::shader::quad::set_wh_data(&q_wh, 1);
	gl::shader::quad::set_texture_data(&texture, 1);

	//indexed triangle test
	static const Vec4_byte blue = vec4_byte(0, 255, 0, 255);
	static Vertex blue_quad[4] = {
		{{0.0f, 0.0f}, 		{0.0f, 0.0f}, blue},
		{{100.0f, 0.0f}, 	{1.0f, 0.0f}, blue},
		{{0.0f, 100.0f}, 	{0.0f, 1.0f}, blue},
		{{100.0f, 100.0f}, 	{1.0f, 1.0f}, blue}
	};

	static u16 indices[6] = {0, 1, 2, 1, 2, 3};

	using namespace gl::shader;

	indexed_triangle::set_vertex_data(blue_quad, LEN(blue_quad));
	indexed_triangle::set_index_data(indices, LEN(indices));

	static const glm::mat4 ortho = glm::ortho(0.0f, (float)window::drawable_width, 0.0f, (float)window::drawable_height);
	util::set_mat4(indexed_triangle::ID, "transform", ortho);

	static const glm::mat4 grid = glm::ortho(0.0f, 32.0f, 0.0f, 32.0f);
	util::set_mat4(quad::ID, "transform", grid);

	while (!io::user_quit)
	{
		io::update();

		

		////
		ui::begin_frame();
		ImGui::ShowDemoWindow();
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
		gl::shader::quad::set_position_data(&q_pos, 1);

		

		gl::clear_screen();
		ui::end_frame();

		glBindTexture(GL_TEXTURE_2D, q_tex.ID);

		indexed_triangle::render(6, 0);
		quad::render(1, 0);
		

		
		window::swap_buffers();

		if (io::key_released[KEY_q])
			printf("Screen wh: %i, %i. Drawable wh: %i, %i\n",
				window::screen_width, window::screen_height, window::drawable_width, window::drawable_height);

	}
	

	window::cleanup();

	return 0;
}