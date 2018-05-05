#include "shader-base.h"

#include <cstdio>
#include <vector>
#include <string>
#include <fstream>

namespace gl
{
	namespace shader
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



  	}
}