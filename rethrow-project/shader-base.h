#pragma once

#include <GL\glew.h>
#include <glm/glm.hpp>

#include "Vector.h"


namespace internal
{
	GLuint compile_shader(const char* path, GLenum shader_type);
	void link_shader_program(GLuint id);
}

namespace util
{

	// for setting uniforms

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