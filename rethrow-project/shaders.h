#pragma once

#include <GL/glew.h>

#include "typedef.h"
#include "Vertex.h"
#include "Vector.h"
#include "Texture.h"

namespace gl::shader
{


void init_all();
void cleanup_all();


namespace indexed_triangle
{
	extern GLuint ID;

	void init();
	void set_vertex_data(Vertex *data, u32 count);
	void set_index_data(u16 *data, u32 count);
	void set_index_data(u32 *data, u32 count);
	void draw(int count, int offset);
}

namespace quad
{
	extern GLuint ID;

	void init();
	void set_position_data(Vec2 *data, u32 count);
	void set_wh_data(Vec2 *data, u32 count);
	void set_texture_data(AtlasComponent *data, u32 count);
	void draw(int count, int offset);
}

namespace triangle
{
	extern GLuint ID;

	void init();
	void set_data(Vertex *data, u32 count);
	void draw(int count, int offset);
}


}