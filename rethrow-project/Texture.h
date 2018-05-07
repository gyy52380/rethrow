#pragma once

#include <GL/glew.h>
#include "Vector.h"


namespace gl::texture
{

	typedef GLuint texID;
	typedef texID atlasID;

	struct Texture
	{
		GLuint ID = 0; //opengl ID
		int channels = 4;

		Vec2 coord; //uv coords, for atlas and such just (0, 0)
		Vec2 wh;    //uv wh, for atlas and such just (1, 1)
	};

}
