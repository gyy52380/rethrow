#include "Texture.h"

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>


namespace gl
{
	//loads image of any format supported by stb_image and makes rgba opengl texture, also frees raw data
	void make_texture_from_file(Texture *tex, const char *path)
	{
		
		u8 *stb_data = stbi_load(path, &tex->w, &tex->h, &tex->channels, STBI_rgb_alpha);
		if (tex->data == NULL)
		{
			printf("Cant load image file for texture: %s\n", path);
			return;
		}

		// stb loads image data so that first pixel is the top left one, while opengl maps
		// uv cords (0.0, 0.0) to the bottom left pixel. this inverts the raw image data. 
		const int scanline_count 	= tex->h;
		const int scanline_width 	= tex->w * 4; //4 bytes per pixel (for every channel)
		const int pixel_count 		= scanline_count * scanline_width;

		tex->data = new u8[pixel_count];
		
		for (int scanline = 0; scanline < scanline_count; scanline++)
		{
			const int stb_scanline = (scanline_count-1) -scanline;
			
			const int idx 		= scanline 		* scanline_width;
			const int stb_idx 	= stb_scanline 	* scanline_width;
			
			memcpy(tex->data+idx, stb_data+stb_idx, scanline_width);
		}


		glGenTextures(1, &tex->ID);
		glBindTexture(GL_TEXTURE_2D, tex->ID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex->w, tex->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, stb_data);

		stbi_image_free(stb_data);
		delete[] tex->data;

		//basic trilinear filtering, no need to make a mipmap
		/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);*/
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}

	//this doesnt free the raw image data, used for loading imgui fonts for example (imgui frees data)
	void make_texture_from_bytes(Texture *tex, u8 *data)
	{
		glGenTextures(1, &tex->ID);
		glBindTexture(GL_TEXTURE_2D, tex->ID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex->w, tex->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

		//basic trilinear filtering, no need to make a mipmap
		/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);*/
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}
}