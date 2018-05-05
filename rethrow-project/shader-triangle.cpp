#include "shader-triangle.h"

#include "typedef.h"
#include "shader-base.h"
#include "Vertex.h"

#include <cstddef>

#include <GL/glew.h>


namespace gl
{
	namespace shader
	{
		namespace triangle
		{

			//every shader is just a self-contained namespace-class!
			//Shader shader;
			//dont need to bind shader to anything... every shader is self contained
			//void set_shader_address(Shader *shader_address)
			//{
			//	shader = shader_address;
			//}

			GLuint ID;
			GLuint vao;
			GLuint vbo;

			u32 data_count;  //count of vertex data


			void init() //this binds the shader 
			{
				//auto &ID = triangle::shader.ID;
				//auto &vao = triangle::shader.vao;
				//auto &vbo = triangle::shader.vbo[0]; //this shader uses only one vbo

				ID = glCreateProgram();

				GLuint vert_id = compile_shader("shaders/sample.vs", GL_VERTEX_SHADER);
				glAttachShader(ID, vert_id);
				GLuint frag_id = compile_shader("shaders/sample.fs", GL_FRAGMENT_SHADER);
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
				glBindBuffer(GL_ARRAY_BUFFER, vbo); //this is not linked to vao state

				glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
				glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

				glEnableVertexArrayAttrib(vao, 0);
				glEnableVertexArrayAttrib(vao, 1);
				glEnableVertexArrayAttrib(vao, 2);
			}

			//takes in vertex data

			void update_data(Vertex *data, u32 count)
			{
				//const auto &vbo = triangle::shader.vbo[0];

				
				data_count = count;
				const u32 size = count*sizeof(decltype(data[0]));


				glBindBuffer(GL_ARRAY_BUFFER, vbo);
				glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW); //maybe change draw mode
			}

			void draw()
			{
				//const auto &ID = shader.ID;
				//const auto &vao = shader.vao;

				glUseProgram(ID);
				glBindVertexArray(vao);
				glDrawArrays(GL_TRIANGLES, 0, data_count);
			}

		}
	}
}