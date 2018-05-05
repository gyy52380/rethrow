#pragma once

#include "Vertex.h"
#include "typedef.h"


namespace gl::shader::triangle
{


	void init();
	void update_data(Vertex *data, u32 count);
	void draw();
	

}