#include "shader-manager.h"

#include "shader-triangle.h"
#include "shader-quad.h"
#include "shader-indexed-triangle.h"


namespace gl::shader
{

	//add more!
	void init_all()
	{
		triangle::init();
		quad::init();
		indexed_triangle::init(640, 480);
	}

	//TODO better update-data interface for renderer users

	//add more!
	void draw_all()
	{
		triangle::draw();
		quad::draw();
	}

}