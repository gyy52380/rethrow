#include "renderer.h"

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "typedef.h"
#include "Vector.h"


namespace gl::renderer
{


	void clear_screen()
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void convert_coords_to_gl_space(Vec2 screen_wh, Vec2 *data, u32 count)
	{
		static Vec2 max_coords = screen_wh;
		static auto ortho_matrix = glm::ortho(0.0f, max_coords.w, 0.0f, max_coords.h);

		//if worldview changed
		//if (max_coords != screen_wh)
		//{
		//	max_coords = screen_wh;
		//	ortho_matrix = glm::ortho(0.0f, max_coords.w, 0.0f, max_coords.h);
		//}

		for (int i = 0; i < count; i++)
		{
			glm::vec4 temp_coord = ortho_matrix * glm::vec4(vec2_to_glm(data[i]), 0.0f, 1.0f);
			data[i] = vec2(temp_coord.x, temp_coord.y);
		}
			 
	}

}