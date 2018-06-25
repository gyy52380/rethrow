#include "shader-base.h"

#include <cstdio>
#include <vector>
#include <string>
#include <fstream>

#include <glm/glm.hpp>

#include "Vector.h"

namespace gl::shader
{


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
		GLuint id = glCreateShader(shader_type);

		// Compile Shader
		char const * SourcePointer = shaderCode.c_str();
		glShaderSource(id, 1, &SourcePointer, NULL);
		glCompileShader(id);

		// Check Shader
		//for openGL error handling
		GLint Result = GL_FALSE;
		int InfoLogLength;

		glGetShaderiv(id, GL_COMPILE_STATUS, &Result);
		if (Result == GL_FALSE)
		{
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &InfoLogLength);
			std::vector<char> ShaderErrorMessage(InfoLogLength + 1);
			glGetShaderInfoLog(id, InfoLogLength, NULL, &ShaderErrorMessage[0]);
			printf("%s\n", &ShaderErrorMessage[0]);

			return 0; //return default shader?
		}

		return id;
	}

	void link_shader_program(GLuint id)
	{
		glLinkProgram(id);

		// Check the program
		GLint Result = GL_FALSE;

		glGetProgramiv(id, GL_LINK_STATUS, &Result);
		if (Result == GL_FALSE)
		{
			int InfoLogLength;
			glGetProgramiv(id, GL_INFO_LOG_LENGTH, &InfoLogLength);
			std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
			glGetProgramInfoLog(id, InfoLogLength, NULL, &ProgramErrorMessage[0]);
			printf("%s\n", &ProgramErrorMessage[0]);
		}
	}


	//for setting uniforms
	void set_bool(GLuint ID, const char* name, bool value) 		
	{ glUseProgram(ID); glUniform1i(glGetUniformLocation(ID, name), (int)value); }

	void set_int(GLuint ID, const char* name, int value) 	
	{ glUseProgram(ID); glUniform1i(glGetUniformLocation(ID, name), value); }

	void set_float(GLuint ID, const char* name, float value) 
	{ glUseProgram(ID);  glUniform1f(glGetUniformLocation(ID, name), value); }


	void set_vec2(GLuint ID, const char* name, const Vec2 &value) 					
	{ glUseProgram(ID);  glUniform2fv(glGetUniformLocation(ID, name), 1, &value.data[0]); }

	void set_vec2(GLuint ID, const char* name, const glm::vec2 &value) 				
	{ glUseProgram(ID);  glUniform2fv(glGetUniformLocation(ID, name), 1, &value[0]); }

	void set_vec2(GLuint ID, const char* name, float x, float y) 					
	{ glUseProgram(ID);  glUniform2f(glGetUniformLocation(ID, name), x, y); }


	//void set_vec3(GLuint ID, const char* name, const Vec3 &value) 					
	//{ glUseProgram(ID);  glUniform3fv(glGetUniformLocation(ID, name), 1, &value.data[0]); }

	void set_vec3(GLuint ID, const char* name, const glm::vec3 &value) 				
	{ glUseProgram(ID);  glUniform3fv(glGetUniformLocation(ID, name), 1, &value[0]); }

	void set_vec3(GLuint ID, const char* name, float x, float y, float z) 			
	{ glUseProgram(ID);  glUniform3f(glGetUniformLocation(ID, name), x, y, z); }


	void set_vec4(GLuint ID, const char* name, const Vec4 &value) 					
	{ glUseProgram(ID);  glUniform4fv(glGetUniformLocation(ID, name), 1, &value.data[0]); }

	void set_vec4(GLuint ID, const char* name, const glm::vec4 &value) 				
	{ glUseProgram(ID);  glUniform4fv(glGetUniformLocation(ID, name), 1, &value[0]); }

	void set_vec4(GLuint ID, const char* name, float x, float y, float z, float w) 	
	{ glUseProgram(ID);  glUniform4f(glGetUniformLocation(ID, name), x, y, z, w); }


	void set_mat2(GLuint ID, const char* name, const glm::mat2 &mat) 
	{ glUseProgram(ID); glUniformMatrix2fv(glGetUniformLocation(ID, name), 1, GL_FALSE, &mat[0][0]); }

	void set_mat3(GLuint ID, const char* name, const glm::mat3 &mat) 
	{ glUseProgram(ID); glUniformMatrix3fv(glGetUniformLocation(ID, name), 1, GL_FALSE, &mat[0][0]); }

	void set_mat4(GLuint ID, const char* name, const glm::mat4 &mat) 
	{ glUseProgram(ID); glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, &mat[0][0]); }

	void set_mat4(GLuint ID, const char* name, const float* mat)
	{ glUseProgram(ID); glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, mat); }


}