#pragma once


namespace gl
{

	bool init();
	void cleanup();
	void update_viewport(int w, int h, int blx = 0, int bly = 0);
	void clear_screen();
}