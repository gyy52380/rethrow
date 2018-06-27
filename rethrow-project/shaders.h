#pragma once

#include <GL/glew.h>

#include "typedef.h"
#include "Vertex.h"
#include "Vector.h"
#include "Texture.h"

namespace gl::shader
{


void init_all();
void cleanup_all();

namespace util // for setting uniforms
{	
	void set_bool(GLuint ID, const char* name, bool value); 		
	void set_int(GLuint ID, const char* name, int value); 	
	void set_float(GLuint ID, const char* name, float value);

	void set_vec2(GLuint ID, const char* name, const Vec2 &value); 					
	void set_vec2(GLuint ID, const char* name, const glm::vec2 &value); 				
	void set_vec2(GLuint ID, const char* name, float x, float y);

	//void set_vec3(GLuint ID, const char* name, const Vec3 &value); 					
	void set_vec3(GLuint ID, const char* name, const glm::vec3 &value); 				
	void set_vec3(GLuint ID, const char* name, float x, float y, float z);

	void set_vec4(GLuint ID, const char* name, const Vec4 &value); 					
	void set_vec4(GLuint ID, const char* name, const glm::vec4 &value); 				
	void set_vec4(GLuint ID, const char* name, float x, float y, float z, float w);
	 	
	void set_mat2(GLuint ID, const char* name, const glm::mat2 &mat); 
	void set_mat3(GLuint ID, const char* name, const glm::mat3 &mat); 
	void set_mat4(GLuint ID, const char* name, const glm::mat4 &mat);
	void set_mat4(GLuint ID, const char* name, const float* mat);
}

namespace indexed_triangle
{
	extern GLuint ID;

	void init();
	void set_vertex_data(Vertex *data, u32 count);
	void set_index_data(u16 *data, u32 count);
	void set_index_data(u32 *data, u32 count);
	void render(int count, int offset);
}

namespace indexed_triangle_ui
{
	extern GLuint ID;

	void init();
	void set_vertex_data(Vertex *data, u32 count);
	void set_index_data(u16 *data, u32 count);
	void set_index_data(u32 *data, u32 count);
	void render(int count, int offset);
}

namespace quad
{
	extern GLuint ID;

	void init();
	void set_position_data(Vec2 *data, u32 count);
	void set_wh_data(Vec2 *data, u32 count);
	void set_texture_data(AtlasComponent *data, u32 count);
	void render(int count, int offset);
}

namespace triangle
{
	extern GLuint ID;

	void init();
	void set_data(Vertex *data, u32 count);
	void render(int count, int offset);
}

namespace internal //used for parsing and compiling shader files
{
	GLuint compile_shader(const char* path, GLenum shader_type);
	void link_shader_program(GLuint id);
}


}