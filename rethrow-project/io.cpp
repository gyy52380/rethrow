#include "io.h"

#include <SDL2\SDL.h>
#include <string.h>
#include <unordered_map>

typedef std::unordered_map<int, bool> key_map;


namespace sdl
{
namespace io
{

bool user_quit = false;
bool key_pressed	[KEY_TOTAL];
bool key_released	[KEY_TOTAL];
bool key_held		[KEY_TOTAL];


void reset_single_frame_events()
{
	memset(key_pressed, false, sizeof(key_pressed));
	memset(key_released, false, sizeof(key_released));
}

void update()
{
	reset_single_frame_events();

	static SDL_Event evnt;
	SDL_PollEvent(&evnt);

	#define KEY evnt.key.keysym.sym
	switch (evnt.type)
	{
	case SDL_QUIT:
		user_quit = true;
		break;
	case SDL_KEYDOWN:
		if (key_held[KEY] == false)
			key_pressed[KEY] = true;

		key_held[KEY]		= true;
		break;
	case SDL_KEYUP:
		if (key_held[KEY] == true)
			key_released[KEY] = true;

		key_held[KEY] = false;
		break;
	default:
		break;
	}
	#undef KEY
}



}
}