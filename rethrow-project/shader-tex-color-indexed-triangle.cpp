#include "shader-tex-color-indexed-triangle.h"

#include <GL/glew.h>

namespace gl::shader::tex_color_triangle
{

	GLuint vao;
	GLuint vbo;
	GLuint ibo;

	
	void init()
	{
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ibo);


	}

}