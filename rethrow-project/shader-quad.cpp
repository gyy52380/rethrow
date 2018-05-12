#include "shader-quad.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cstddef>
#include <cstdio>

#include "typedef.h"
#include "macros.h"
#include "Vector.h"
#include "Texture.h"
#include "shader-base.h"

namespace gl::shader::quad
{
	using texture::Texture;
	using texture::texID;
	using texture::atlasID;

	GLuint ID;
	GLuint vao;

	enum Attribute { COORD, WH, TEXTURE, ATTRIB_COUNT };
	GLuint vbo[ATTRIB_COUNT];

	u32 element_count = 0;
	bool element_count_valid = false;

	texID active_texture;


	void init()
	{
		ID = glCreateProgram();

		GLuint vert_id = compile_shader("C:/Users/gabri/source/repos/rethrow/rethrow-project/shaders/quad.vs", GL_VERTEX_SHADER);
		glAttachShader(ID, vert_id);
		GLuint geom_id = compile_shader("C:/Users/gabri/source/repos/rethrow/rethrow-project/shaders/quad.gs", GL_GEOMETRY_SHADER);
		glAttachShader(ID, geom_id);
		GLuint frag_id = compile_shader("C:/Users/gabri/source/repos/rethrow/rethrow-project/shaders/quad.fs", GL_FRAGMENT_SHADER);
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
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vec2), (void*)0); //quad wh

		using texture::Texture;
		glBindBuffer(GL_ARRAY_BUFFER, vbo[TEXTURE]);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Texture), (void*)offsetof(Texture, coord)); //"texture vertex" coord on atlas
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Texture), (void*)offsetof(Texture, wh)); //texture wh

		for (int i = 0; i <= 3; i++)
			glEnableVertexArrayAttrib(vao, i);

		//set uniforms
		const glm::mat4 projection = glm::ortho(0.0f, 32.0f, 0.0f, 32.0f);
		set_mat4(ID, "projection", projection);

		const Vec2 wh_scale = vec2(2.0f/32.0f, 2.0f/32.0f);
		set_vec2(ID, "wh_scale", wh_scale);
	}


	void set_count(u32 count)
	{
		element_count = count;
		element_count_valid = true;
	}

	//TODO test if void* decltype works
	void update_data_coord(Vec2 *data)
	{
		if (!element_count_valid)
		{
			printf("Element count for this frame wasnt set! Set it before updating data.\n");
			return;
		}

		const u32 size = element_count*sizeof(decltype(data[0]));

		glBindBuffer(GL_ARRAY_BUFFER, vbo[COORD]);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

	void update_data_wh(Vec2 *data)
	{

		if (!element_count_valid)
		{
			printf("Element count for this frame wasnt set! Set it before updating data.\n");
			return;
		}

		const u32 size = element_count*sizeof(decltype(data[0]));

		glBindBuffer(GL_ARRAY_BUFFER, vbo[WH]);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

	void update_data_tex(texture::Texture *data) //TODO handle type of data..
	{
		////this part can cause a race condition later on!!!
		//element_count = count;
		if (!element_count_valid)
		{
			printf("Element count for this frame wasnt set! Set it before updating data.\n");
			return;
		}

		active_texture = data[0].ID;

		const u32 size = element_count*sizeof(decltype(data[0]));

		glBindBuffer(GL_ARRAY_BUFFER, vbo[TEXTURE]);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

	void draw() //TODO texture atlas binding
	{
	 	glUseProgram(ID);
		glBindVertexArray(vao);

		glBindTexture(GL_TEXTURE_2D, active_texture);
		glDrawArrays(GL_POINTS, 0, element_count);

		element_count_valid = false;
		//this flag must be reset after drawing.
		//every frame requires resetting count of elements to draw.
		//this will probably prevent data races later on
	}
}