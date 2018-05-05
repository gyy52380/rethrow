#pragma once

#include <GL\glew.h>

namespace gl
{
	namespace shader
	{
		constexpr size_t MAX_BUFFERS = 8;

		enum ShaderKind
		{
			SHADER_INVALID,

			SHADER_TRIANGLE,
			SHADER_QUAD,

			SHADER_TOTAL
		};

		struct Shader
		{
			ShaderKind kind = SHADER_INVALID;

			GLuint ID;
			GLuint vao;
			GLuint vbo[MAX_BUFFERS];
			GLuint ibo;
			GLuint instbo;
		};


		GLuint compile_shader(const char* path, GLenum shader_type);
		void link_shader_program(GLuint id);


	}
}