#include "shader-quad.h"

#include <GL/glew.h>

#include <cstddef>
#include <cstdio>

#include "typedef.h"
#include "macros.h"
#include "Vector.h"
#include "Texture.h"
#include "shader-base.h"

namespace gl::shader::quad
{

	GLuint ID;
	GLuint vao;

	enum Attribute { COORD, WH, TEXTURE, ATTRIB_COUNT };
	GLuint vbo[VBO_COUNT];

	u32 data_count;


	void init()
	{
		ID = glCreateProgram();

		GLuint vert_id = compile_shader("shaders/quad.vs", GL_VERTEX_SHADER);
		glAttachShader(ID, vert_id);
		GLuint geom_id = compile_shader("shaders/quad.gs", GL_GEOMETRY_SHADER);
		glAttachShader(ID, geom_id);
		GLuint frag_id = compile_shader("shaders/quad.fs", GL_FRAGMENT_SHADER);
		glAttachShader(ID, frag_id);

		link_shader_program(ID);

		glDetachShader(ID, vert_id);
		glDeleteShader(vert_id);
		glDetachShader(ID, geom_id);
		glDeleteShader(geom_id);
		glDetachShader(ID, frag_id);
		glDeleteShader(frag_id);
		//program now compiled

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(LEN(vbo), vbo);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[COORD]);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vec2), (void*)0); //vertex coord

		glBindBuffer(GL_ARRAY_BUFFER, vbo[WH]);
		glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(Vec2), (void*)offsetof(Vec2, w)); //quad width
		glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Vec2), (void*)offsetof(Vec2, h)); //quad height

		using texture::Texture;
		glBindBuffer(GL_ARRAY_BUFFER, vbo[TEXTURE]);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Texture), (void*)offsetof(Texture, coord)); //"texture vertex" coord on atlas
		
		const auto texture_w_offset = offsetof(Texture, wh) + offsetof(Vec2, w);
		glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(Texture), (void*)texture_w_offset); //texture quad width
		
		const auto texture_h_offset = offsetof(Texture, wh) + offsetof(Vec2, h);
		glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(Texture), (void*)texture_h_offset); //texture quad height

		for (int i = 0; i <= 5; i++)
			glEnableVertexArrayAttrib(vao, i);
	}

	void update_data_vertex(Vec2 *data, u32 count, Attribute attribute) //TODO handle type of data..
	{
			data_count = count;
			const u32 size = count*sizeof(decltype(data[0]));

			glBindBuffer(GL_ARRAY_BUFFER, vbo[attribute]);
			glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

}