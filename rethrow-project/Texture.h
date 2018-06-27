#pragma once

#include <string>
#include <GL/glew.h>
#include "Vector.h"


//namespace gl::texture
//{
//
//	typedef GLuint texID;
//	typedef texID atlasID;
//
//	struct Texture
//	{
//		GLuint ID = 0; //opengl ID
//		int w, h;
//		int channels = 4;
//		u8 *data;
//
//		Vec2 coord; //uv coords, for atlas and such just (0, 0)
//		Vec2 wh;    //uv wh, for atlas and such just (1, 1)
//	};
//
//}

namespace gl
{
	struct Texture
	{
		GLuint ID;
		std::string name;
		
		int w, h;
		int channels; //actual number of channels
		u8 *data;
	};

	void make_texture_from_file(Texture *tex, const char *path);
	void make_texture_from_bytes(Texture *tex, u8 *data);


	//child texture information that's a part of parent texture atlas.
	//atlasID: opengl id of texture atlas
	//uv_position: bottomleft uv coordinates of the child texture
	//uv_wh: width and height of child texture, in uv coordinates
	struct AtlasComponent
	{
		GLuint atlasID;
		Vec2 uv_position;
		Vec2 uv_wh;
	};
}
