#include "shader-indexed-triangle.h"

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <cstdio>

#include "typedef.h"
#include "Vertex.h"
#include "shader-base.h"

namespace gl::shader::tex_color_triangle
{

	GLuint ID;

	GLuint vao;
	GLuint vbo;
	GLuint ibo;

	//this is total count for all vertices
	u32 element_count;
	u32 index_count;
	GLenum index_data_type;
	
	void init(float screen_width, float screen_height)
	{
		ID = glCreateProgram();

		GLuint vert_id = compile_shader("shaders/indexed-triangle.vert", GL_VERTEX_SHADER);
		glAttachShader(ID, vert_id);
		GLuint frag_id = compile_shader("shaders/indexed-triangle.frag", GL_FRAGMENT_SHADER);
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

		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));

		for (int i = 0; i <= 2; i++)
			glEnableVertexAttribArray(i);

		const glm::mat4 ortho = glm::ortho(0.0f, screen_width, 0.0f, screen_height);
		set_mat4(ID, "transform", ortho);
	}

	void update_vertex_data(Vertex *data, u32 size)
	{
		element_count = size/sizeof(decltype(data));

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

	void update_index_data(u16 *data, u32 size)
	{
		index_count = size/sizeof(decltype(data));
		index_data_type = GL_UNSIGNED_SHORT;

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

	void update_index_data(u32 *data, u32 size)
	{
		index_count = size/sizeof(decltype(data));
		index_data_type = GL_UNSIGNED_INT;

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

	void draw(int count, int offset)
	{
		if (offset < 0 || (offset + count) > index_count)
	    {
	        printf("Invalid range passed to render_indexed_triangles()!\n"
	               "Got [%d, %d] but currently %d indices are set.",
	               offset, offset + count - 1, index_count);
	        return;
	    }
	    
	    const int index_data_size = (index_data_type == GL_UNSIGNED_SHORT) ? 2 : 4;

	    glDrawElements(GL_TRIANGLES, count, index_data_type, (void*)(offset * index_data_size));
	}

}