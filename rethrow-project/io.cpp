#include "io.h"

#include <SDL2\SDL.h>
#include <string.h>

#include "window.h"
#include "ui.h"
#include "gl.h"


namespace io
{


	bool user_quit = false;

	bool key_pressed	[KEY_TOTAL];
	bool key_released	[KEY_TOTAL];
	bool key_held		[KEY_TOTAL];

	bool mouse_key_pressed	[MOUSE_TOTAL];
	bool mouse_key_released	[MOUSE_TOTAL];
	bool mouse_key_held		[MOUSE_TOTAL];

	Vec2 mouse_pos;


	void update()
	{

		//reset single-frame events
		memset(key_pressed, false, sizeof(key_pressed));
		memset(key_released, false, sizeof(key_released));
		memset(mouse_key_pressed, false, sizeof(mouse_key_pressed));
		memset(mouse_key_released, false, sizeof(mouse_key_released));


		static SDL_Event event;
		SDL_PollEvent(&event);

		ui::handle_events(&event);

		switch (event.type)
		{

		case SDL_QUIT:
		{
			user_quit = true;

		}	break;

		case SDL_WINDOWEVENT:
		{
			window::update_screen_size();
			gl::update_viewport(window::drawable_width, window::drawable_height); //this is super ugly here, change later

		}	break;

		case SDL_KEYDOWN:
		{
			const auto KEY = event.key.keysym.sym;

			if (KEY > KEY_TOTAL)
				break;

			if (key_held[KEY] == false)
				key_pressed[KEY] = true;

			key_held[KEY]	     = true;
		
		} 	break;

		case SDL_KEYUP:
		{
			const auto KEY = event.key.keysym.sym;

			if (KEY > KEY_TOTAL)
				break;

			if (key_held[KEY] == true)
				key_released[KEY] = true;

			key_held[KEY] 		  = false;	
		
		}	break;


		case SDL_MOUSEBUTTONDOWN:
		{
			mouse_pos = vec2(event.button.x, event.button.y);

			Mouse changed_button = MOUSE_INVALID;
			switch (event.button.button)
			{
				case SDL_BUTTON_LEFT: changed_button = MOUSE_LEFT; 		break;
				case SDL_BUTTON_MIDDLE: changed_button = MOUSE_MIDDLE; 	break;
				case SDL_BUTTON_RIGHT: changed_button = MOUSE_RIGHT; 	break;
			}

			if (mouse_key_held[changed_button] == false)
				mouse_key_pressed[changed_button] = true;

			mouse_key_held[changed_button] = true;
		
		}	break;

		case SDL_MOUSEBUTTONUP:
		{
			mouse_pos = vec2(event.button.x, event.button.y);
			
			Mouse changed_button = MOUSE_INVALID;
			switch (event.button.button)
			{
				case SDL_BUTTON_LEFT: changed_button = MOUSE_LEFT; 		break;
				case SDL_BUTTON_MIDDLE: changed_button = MOUSE_MIDDLE; 	break;
				case SDL_BUTTON_RIGHT: changed_button = MOUSE_RIGHT; 	break;
			}

			if (mouse_key_held[changed_button] == true)
				mouse_key_released[changed_button] = true;

			mouse_key_held[changed_button] = false;
		
		}	break;

		default:
			break;
		}
	}


}