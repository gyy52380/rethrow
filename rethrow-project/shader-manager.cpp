#include "shader-manager.h"

#include "shader-triangle.h"	


namespace gl::shader
{

	//add more!
	void init_all()
	{
		triangle::init();
	}

	//TODO better update-data interface

	//add more!
	void draw_all()
	{
		triangle::draw();
	}

}