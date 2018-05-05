#pragma once

#include "typedef.h"
#include "Vector.h"


namespace gl::texture
{

	typedef u32 atlasID;

	struct Texture
	{
		atlasID ID = 0;

		Vec2 coord;
		Vec2 wh;
	};

}