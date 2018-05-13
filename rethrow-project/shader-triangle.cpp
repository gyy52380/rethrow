#include "shader-triangle.h"

#include "typedef.h"
#include "shader-base.h"
#include "Vertex.h"

#include <cstddef>

#include <GL/glew.h>


namespace gl::shader::triangle
{


	GLuint ID;
	GLuint vao;
	GLuint vbo;

	u32 element_count;  //count of vertices


	void init()
	{
		ID = glCreateProgram();

		GLuint vert_id = compile_shader("C:/Users/gabri/source/repos/rethrow/rethrow-project/shaders/sample.vert", GL_VERTEX_SHADER);
		glAttachShader(ID, vert_id);
		GLuint frag_id = compile_shader("C:/Users/gabri/source/repos/rethrow/rethrow-project/shaders/sample.frag", GL_FRAGMENT_SHADER);
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
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));

		for (int i = 0; i <= 2; i++)
			glEnableVertexArrayAttrib(vao, i);
	}

	//takes in vertex data
	void update_data(Vertex *data, u32 count)
	{
		//this part can cause a race condition later on!!!
		element_count = count;
		const u32 size = count*sizeof(decltype(data[0]));

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW); //maybe change draw mode
	}

	void draw()
	{
		glUseProgram(ID);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, element_count);
	}

}