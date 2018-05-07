#include "shader-manager.h"

#include "shader-triangle.h"
#include "shader-quad.h"	


namespace gl::shader
{

	//add more!
	void init_all()
	{
		triangle::init();
		quad::init();
	}

	//TODO better update-data interface for renderer users

	//add more!
	void draw_all()
	{
		triangle::draw();
		quad::draw();
	}

}