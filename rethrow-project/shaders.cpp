#include "shaders.h"

#include <GL/glew.h>

#include <string.h>
#include <cstdio>
#include <vector>
#include <string>
#include <fstream>

#include "macros.h"
#include "Vertex.h"
#include "Vector.h"


namespace gl
{


enum ShaderKind //T = texture, C = color
{
	SHADER_UNKNOWN,
	SHADER_TRIANGLE_CT,
	SHADER_QUAD_T,

	SHADER_TOTAL
};

struct
{
	GLuint ID[SHADER_TOTAL];
	GLuint vao[SHADER_TOTAL];
	GLuint vbo[SHADER_TOTAL];
	GLuint ibo[SHADER_TOTAL];
	GLuint instbo[SHADER_TOTAL];
} shaders;

GLuint compile_shader(const char* path, GLenum shader_type)
{
	//read source from file
	std::string shaderCode;
	std::ifstream shaderStream(path, std::ios::in);
	if (shaderStream.is_open()) {
		std::string Line = "";
		while (getline(shaderStream, Line))
			shaderCode += "\n" + Line;
		shaderStream.close();
	}
	else {
		printf("Can't open shader source file: %s\n", path);
		getchar();
		return 0; // change to fatal error
	}

	//create the shader
	GLuint ID;
	ID = glCreateShader(shader_type);

	// Compile Shader
	char const * SourcePointer = shaderCode.c_str();
	glShaderSource(ID, 1, &SourcePointer, NULL);
	glCompileShader(ID);

	// Check Shader
	//for openGL error handling
	GLint Result = GL_FALSE;
	int InfoLogLength;

	glGetShaderiv(ID, GL_COMPILE_STATUS, &Result);
	if (Result == GL_FALSE)
	{
		glGetShaderiv(ID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		std::vector<char> ShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(ID, InfoLogLength, NULL, &ShaderErrorMessage[0]);
		printf("%s\n", &ShaderErrorMessage[0]);

		ID = 0; //return default shader?
	}

	return ID;
}

void link_shader_program(GLuint ID)
{
	glLinkProgram(ID);

	// Check the program
	GLint Result = GL_FALSE;
	int InfoLogLength;
	glGetProgramiv(ID, GL_LINK_STATUS, &Result);
	if (Result == GL_FALSE)
	{
		glGetProgramiv(ID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}
}


void init_shader_array()
{
	memset(&shaders, 0, sizeof(shaders));
}

void init_shader_triangle_ct()
{
	auto &ID = shaders.ID[SHADER_TRIANGLE_CT];
	auto &vao = shaders.vao[SHADER_TRIANGLE_CT];
	auto &vbo = shaders.vbo[SHADER_TRIANGLE_CT];

	ID = glCreateProgram();

	GLuint vert_id = compile_shader("shaders/sample.vert", GL_VERTEX_SHADER);
	glAttachShader(ID, vert_id);
	GLuint frag_id = compile_shader("shaders/sample.frag", GL_FRAGMENT_SHADER);
	glAttachShader(ID, frag_id);

	link_shader_program(ID);

	glDetachShader(ID, vert_id);
	glDeleteShader(vert_id);
	glDetachShader(ID, frag_id);
	glDeleteShader(frag_id);
	//program now compiled

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo); //this is not linked to vbo state

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

	glEnableVertexArrayAttrib(vao, 0);
	glEnableVertexArrayAttrib(vao, 1);
	glEnableVertexArrayAttrib(vao, 2);

}

void init_shader_quad_t()
{

}

void init_shaders()
{
	DO_ONCE
	{
		init_shader_array();
		init_shader_triangle_ct();
		init_shader_quad_t();
	}
}

void swap_frame_buffer(SDL_Window *window_handle)
{
	SDL_GL_SwapWindow(window_handle);
}

void draw_shader_triangle_ct(Vertex *data, int len_bytes)
{
	auto &ID = shaders.ID[SHADER_TRIANGLE_CT];
	auto &vao = shaders.vao[SHADER_TRIANGLE_CT];
	auto &vbo = shaders.vbo[SHADER_TRIANGLE_CT];

	bool is_program = glIsProgram(ID);

	glClear(GL_COLOR_BUFFER_BIT);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, len_bytes, data, GL_STATIC_DRAW);

	glUseProgram(ID);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}


}