#include "shaders.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdio.h>

#include "typedef.h"
#include "shader-base.h"
#include "Vertex.h"
#include "Texture.h"

namespace gl::shader
{
	
namespace indexed_triangle
{

	/*
	 * shader that takes in vertices (type Vertex) and indices (type u16 or u32)
	 * and draws elements using the currently bound texture and transform matrix
	 *
	 * shader files: indexed-triangle .vert, .frag
	 *
	 * user must set textures and transform matrix manually before calling draw()!
	 *
	 * all vertices and indices must be set at the same time, index or vertex data
	 * can't be appended once set.
	*/

	GLuint ID;

	GLuint vao;
	GLuint vbo;
	GLuint ibo;

	//this is total count for all vertices
	u32 element_count;
	u32 index_count;
	GLenum index_data_type; //GL_UNSIGNED_BYTE or GL_UNSIGNED_SHORT


	//compiles the shader program. can be recalled at any point to recompile the shader
	void init()
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

		//shader takes in vertices: Vertex {vec2<float>, vec2<float>, vec2<u8>}
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
		glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));

		for (int i = 0; i <= 2; i++)
			glEnableVertexAttribArray(i);
	}

	void set_vertex_data(Vertex *data, u32 size)
	{
		element_count = size/sizeof(Vertex);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

	void set_index_data(u16 *data, u32 size)
	{
		index_count = size/sizeof(u16);
		index_data_type = GL_UNSIGNED_SHORT;

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

	void set_index_data(u32 *data, u32 size)
	{
		index_count = size/sizeof(u32);
		index_data_type = GL_UNSIGNED_INT;

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

	//count: number of elements to draw
	//offset: first element to be drawm (start from indices[offset])
	void draw(int count, int offset)
	{
		if (offset < 0 || (offset + count) > index_count)
	    {
	        printf("Invalid range passed to render_indexed_triangles()!\n"
	               "Got [%d, %d] but currently %d indices are set.",
	               offset, offset + count - 1, index_count);
	        return;
	    }

	    glUseProgram(ID);
		glBindVertexArray(vao);
	    
	    const int index_data_size = (index_data_type == GL_UNSIGNED_SHORT) ? sizeof(u16) : sizeof(u32);

	    glDrawElements(GL_TRIANGLES, count, index_data_type, (void*)(offset * index_data_size));
	}

}


namespace quad
{
	GLuint ID;
	GLuint vao;

	GLuint vbo_position,
		   vbo_wh,
		   vbo_texture;

	u32 element_count;
	bool element_count_valid;

	GLuint active_texture;


	void init()
	{
		ID = glCreateProgram();

		GLuint vert_id = compile_shader("shaders/quad.vert", GL_VERTEX_SHADER);
		glAttachShader(ID, vert_id);
		GLuint geom_id = compile_shader("shaders/quad.geom", GL_GEOMETRY_SHADER);
		glAttachShader(ID, geom_id);
		GLuint frag_id = compile_shader("shaders/quad.frag", GL_FRAGMENT_SHADER);
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


		glGenBuffers(1, &vbo_position);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_position);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vec2), (void*)0); //quad's position

		glGenBuffers(1, &vbo_wh);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_wh);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vec2), (void*)0); //quad's wh (x=w, y=h)

		using texture::Texture;
		glBindBuffer(GL_ARRAY_BUFFER, vbo[TEXTURE]);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Texture), (void*)offsetof(Texture, coord)); //"texture vertex" coord on atlas
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Texture), (void*)offsetof(Texture, wh)); //texture wh

		for (int i = 0; i <= 3; i++)
			glEnableVertexAttribArray(i);

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

}