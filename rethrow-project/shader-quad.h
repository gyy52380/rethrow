#pragma once

/*
 *quad drawer api.
 *init shader program with init()
 *set count of quads to draw EVERY FRAME with set_count()
 *update coords of quads, width and height
 *update texture data of quads (coord, wh)
 *IMPORTANT currently shader can use only one atlasID!!!
 *used atlasID is of the first entity sent to the shader
 *TODO change that to any number of atlasIDs (this currently has to be done outside)
 *draw all quads with draw()
 */


#include "typedef.h"
#include "Vector.h"
#include "Texture.h"

namespace gl::shader::quad
{


	void init();
	void set_count(u32 count);
	void update_data_coord(Vec2 *data);
	void update_data_wh(Vec2 *data);
	void update_data_tex(texture::Texture *data);
	void draw();


}