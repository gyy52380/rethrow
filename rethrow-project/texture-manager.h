#pragma once

#include "Texture.h"

namespace gl::texture
{

	Texture make_simple_texture(const char* path);
	void make_texure_from_bytes(Texture *unloaded_texture);

}