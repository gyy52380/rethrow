#pragma once

#include "Vertex.h"
#include "typedef.h"

namespace gl::shader::tex_color_triangle
{

	void init(float screen_width, float screen_height);
	void update_vertex_data(Vertex *data, u32 size);
	void update_index_data(u16 *data, u32 size);
	void update_index_data(u32 *data, u32 size);
	void draw(int count, int offset);

}