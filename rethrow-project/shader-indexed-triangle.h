#pragma once

#include <GL/glew.h>

#include "Vertex.h"
#include "typedef.h"

namespace gl::shader::indexed_triangle
{

	extern GLuint ID;

	void init(float screen_width, float screen_height);
	void set_vertex_data(Vertex *data, u32 size);
	void set_index_data(u16 *data, u32 size);
	void set_index_data(u32 *data, u32 size);
	void draw(int count, int offset);

}