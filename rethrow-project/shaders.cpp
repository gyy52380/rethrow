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
	

void init_all()
{
	indexed_triangle::init();
	quad::init();
	triangle::init();
}

void cleanup_all()
{
	glDeleteProgram(indexed_triangle::ID);
	glDeleteProgram(quad::ID);
	glDeleteProgram(triangle::ID);
}


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

	void set_vertex_data(Vertex *data, u32 count)
	{
		extern u32 element_count;
		element_count 	= count;
		const int size 	= count*sizeof(Vertex);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

	void set_index_data(u16 *data, u32 count)
	{
		extern u32 index_count;
		index_count = count;
		
		index_data_type = GL_UNSIGNED_SHORT;
		const int size 	= count*sizeof(u16);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

	void set_index_data(u32 *data, u32 count)
	{
		extern u32 index_count;
		index_count = count;
		
		index_data_type = GL_UNSIGNED_INT;
		const int size 	= count*sizeof(u32);

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

	/*
	 * shader that draws textured quads (not colored but easy to add)
	 * takes in components of the quads and stores them in separate vbos
	 * to better work with operations on arrays. instead of taking in an
	 * array of Quads, shader takes in array of quad_positions, _wh, _texture_info
	 * texture and transform matrix must be set manually.
	 * shader files: quad .vert .geom .frag
	 */

	GLuint ID;

	GLuint vao;

	GLuint 	vbo_position,
		   	vbo_wh,
		   	vbo_texture;

	u32 	count_positon,
			count_wh,
			count_texture;


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

		glBindBuffer(GL_ARRAY_BUFFER, vbo_texture);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(AtlasComponent), (void*)offsetof(AtlasComponent, uv_position)); //"texture vertex" coord on atlas
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(AtlasComponent), (void*)offsetof(AtlasComponent, uv_wh)); //texture wh

		for (int i = 0; i <= 3; i++)
			glEnableVertexAttribArray(i);

		//set uniforms
		
		const glm::mat4 projection = glm::ortho(0.0f, 32.0f, 0.0f, 32.0f);
		set_mat4(ID, "projection", projection);

		const Vec2 wh_scale = vec2(2.0f/32.0f, 2.0f/32.0f);
		set_vec2(ID, "wh_scale", wh_scale);
	}

	// every component of each quad is set seperately (works good with ECS system)
	void set_position_data(Vec2 *data, u32 count)
	{
		extern u32 count_positon;
		count_positon 	= count;

		const u32 size 	= count*sizeof(Vec2);

		glBindBuffer(GL_ARRAY_BUFFER, vbo_position);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

	void set_wh_data(Vec2 *data, u32 count)
	{
		extern u32 count_wh;
		count_wh 		= count;

		const u32 size 	= count*sizeof(Vec2);

		glBindBuffer(GL_ARRAY_BUFFER, vbo_wh);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

	void set_texture_data(AtlasComponent *data, u32 count)
	{
		extern u32 count_texture;
		count_texture 	= count;

		const u32 size 	= count*sizeof(AtlasComponent);

		glBindBuffer(GL_ARRAY_BUFFER, vbo_texture);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

	// draws count quads, starting with quad[offest]. texture must be set manually
	void draw(int count, int offset)
	{
		if (count_positon != count_wh || count_positon != count_texture) //if all arent equal
		{
			printf("Different number of each quad's component has been set!\ncount_positon = %i, count_wh = %i, count_texture = %i\n",
					count_positon, count_wh, count_texture);
			return;
		}

	 	glUseProgram(ID);
		glBindVertexArray(vao);
		glDrawArrays(GL_POINTS, offset, count);
	}

}

namespace triangle
{
	/*
	 * simple triangle shader. this is a test shader, has no real use.
	 * takes in array of Vertex, draws
	 * non indexed, textured, colored triangles.
	 * no ability to set transform matrix
	 */

	GLuint ID;

	GLuint vao;
	GLuint vbo;


	void init()
	{
		ID = glCreateProgram();

		//GLuint vert_id = compile_shader("C:/Users/gabri/source/repos/rethrow/rethrow-project/shaders/sample.vert", GL_VERTEX_SHADER);
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
		glBindBuffer(GL_ARRAY_BUFFER, vbo); //this is not linked to vao state

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
		glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));

		for (int i = 0; i <= 2; i++)
			glEnableVertexAttribArray(i);
	}

	//takes in vertex data
	void set_data(Vertex *data, u32 count)
	{
		const u32 size 	= count*sizeof(Vertex);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

	void draw(int count, int offset)
	{
		glUseProgram(ID);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, offset, count);
	}
}


}