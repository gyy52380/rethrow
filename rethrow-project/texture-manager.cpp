#include "texture-manager.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb\stb_image.h>
#include <GL/glew.h>
#include <cstdio>

#include "Texture.h"

namespace gl::texture
{


	Texture make_simple_texture(const char* path)
	{
		Texture tex;

		tex.coord = vec2(0, 0); //uv
		tex.wh = vec2(1, 1);    //uv
		

		int w, h; //in pixels
		tex.data = stbi_load(path, &tex.w, &tex.h, &tex.channels, STBI_rgb_alpha);
		if (tex.data == NULL)
		{
			printf("Cant load image file for texture: %s\n", path);
			return tex;
		}

		printf("w: %d, h: %d\n", tex.w, tex.h);

		glGenTextures(1, &tex.ID);
		glBindTexture(GL_TEXTURE_2D, tex.ID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex.w, tex.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.data);

		stbi_image_free(tex.data);

		//basic trilinear filtering, no need to make a mipmap
		/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);*/
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		
	    return tex;
	}

	void make_texure_from_bytes(Texture *unloaded_texture)
	{
		Texture &tex = *unloaded_texture;

		tex.coord = vec2(0, 0); //uv
		tex.wh = vec2(1, 1);    //uv

		glGenTextures(1, &tex.ID);
		glBindTexture(GL_TEXTURE_2D, tex.ID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex.w, tex.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}


}