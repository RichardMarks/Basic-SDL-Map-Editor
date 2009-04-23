// editor.cpp
// Project: Basic SDL Map Editor
// Author: Richard Marks

#include "common_includes.h"

void editor_execute()
{
	int startTicks = SDL_GetTicks();

	while(editorrunning)
	{
		while(SDL_PollEvent(editorevent))
		{
			if (SDL_QUIT == editorevent->type)
			{
				editorrunning = false;
			}
			else if (SDL_MOUSEBUTTONDOWN == editorevent->type)
			{
				if (editorshowtileselector)
				{
					if (SDL_BUTTON_WHEELUP == editorevent->button.button)
					{
						selector_scroll_up();
					}
					else if (SDL_BUTTON_WHEELDOWN == editorevent->button.button)
					{
						selector_scroll_down();
					}
				}
				else
				{
					if (SDL_BUTTON_WHEELUP == editorevent->button.button)
					{
						editor_select_previous_tile();
					}
					else if (SDL_BUTTON_WHEELDOWN == editorevent->button.button)
					{
						editor_select_next_tile();
					}
				}
			}
		}

		startTicks = SDL_GetTicks();

		editor_handle_input_common();
		editorhandleinputfunction();

		editor_refresh_view_common();
		editorrefreshviewfunction();

		if (editorshowtileselector)
		{
			selector_refresh_view();
		}

		SDL_Flip(editorscreen);

		int tickDiff = SDL_GetTicks() - startTicks;
		if (tickDiff < 33)
		{
			SDL_Delay(33 - tickDiff);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////

void editor_move_camera_up()
{
	if (editorcamera->intiles->y > 0)
	{
		editorcamera->intiles->y--;
		editorcamera->inpixels->y -= editortileheight;
	}
}

////////////////////////////////////////////////////////////////////////////////

void editor_move_camera_down()
{
	if (editorcamera->intiles->y < editorlevelheight - editorcamera->intiles->h)
	{
		editorcamera->intiles->y++;
		editorcamera->inpixels->y += editortileheight;
	}
}

////////////////////////////////////////////////////////////////////////////////

void editor_move_camera_left()
{
	if (editorcamera->intiles->x > 0)
	{
		editorcamera->intiles->x--;
		editorcamera->inpixels->x -= editortilewidth;
	}
}

////////////////////////////////////////////////////////////////////////////////

void editor_move_camera_right()
{
	if (editorcamera->intiles->x < editorlevelwidth - editorcamera->intiles->w)
	{
		editorcamera->intiles->x++;
		editorcamera->inpixels->x += editortilewidth;
	}
}

////////////////////////////////////////////////////////////////////////////////

void editor_load_level()
{
	if (editorlevel)
	{
		editormapdestroyfunction(editorlevel);
	}

	editorlevel = editormaploadfunction("wip.map");
}

////////////////////////////////////////////////////////////////////////////////

void editor_save_level()
{
	editormapsavefunction(editorlevel, "wip.map");
}

////////////////////////////////////////////////////////////////////////////////

bool editor_load_startup_config_file()
{
	return true;
}

////////////////////////////////////////////////////////////////////////////////

void editor_save_startup_config_file()
{
}

////////////////////////////////////////////////////////////////////////////////

void editor_printf(int x, int y, unsigned int color, const char* format, ...)
{
	char textbuffer[0x100];
	va_list args;
	va_start(args, format);
	vsprintf(textbuffer, format, args);
	SDL_Color c;
	Uint8 r, g, b;
	SDL_GetRGB(color, editorscreen->format, &r, &g, &b);
	c.r = r;
	c.g = g;
	c.b = b;
	SDL_Surface* textsurface = TTF_RenderText_Solid(editorfont, textbuffer, c);
	if (textsurface)
	{
		SDL_Rect dst;
		dst.x = x;
		dst.y = y;
		SDL_BlitSurface(textsurface, 0, editorscreen, &dst);
		SDL_FreeSurface(textsurface);
	}
	va_end(args);
}

////////////////////////////////////////////////////////////////////////////////

void editor_update_cursor_surface()
{
	SDL_Rect src, dst;

	static int tilesperrow = (editortileset->w / editortilewidth);

	int tilemodperrow = (editorcurrentile % tilesperrow);
	int tileoverperrow = (editorcurrentile / tilesperrow);

	src.x = 1 + tilemodperrow + (tilemodperrow * editortilewidth);
	src.y = 1 + tileoverperrow + (tileoverperrow * editortileheight);
	src.w = editortilewidth;
	src.h = editortileheight;

	dst.x = 1;
	dst.y = 1;

	SDL_BlitSurface(editortileset, &src, editorcursorsurface, &dst);
}

////////////////////////////////////////////////////////////////////////////////

void editor_select_previous_tile()
{
	if (editorcurrentile > 0)
	{
		editorcurrentile--;
	}
	editor_update_cursor_surface();
}

////////////////////////////////////////////////////////////////////////////////

void editor_select_next_tile()
{
	if (editorcurrentile < selectortilecount)
	{
		editorcurrentile++;
	}
	editor_update_cursor_surface();
}

////////////////////////////////////////////////////////////////////////////////

void editor_set_mode(EditorMode mode)
{
	switch(mode)
	{
		// assign background editor function pointers
		case EditorBGEdit:
		{
			editorrefreshviewfunction 	= editor_refresh_view_bgeditor;
			editorhandleinputfunction 	= editor_handle_input_bgeditor;
			editorsettilefunction		= editor_set_tile_bgeditor;
			editorunsettilefunction		= editor_unset_tile_bgeditor;
		} break;

		// assign foreground editor function pointers
		case EditorFGEdit:
		{
			editorrefreshviewfunction 	= editor_refresh_view_fgeditor;
			editorhandleinputfunction 	= editor_handle_input_fgeditor;
			editorsettilefunction		= editor_set_tile_fgeditor;
			editorunsettilefunction		= editor_unset_tile_fgeditor;
		} break;

		// assign collision detection editor function pointers
		case EditorCDEdit:
		{
			editorrefreshviewfunction 	= editor_refresh_view_cdeditor;
			editorhandleinputfunction 	= editor_handle_input_cdeditor;
			editorsettilefunction		= editor_set_tile_cdeditor;
			editorunsettilefunction		= editor_unset_tile_cdeditor;
		} break;

		default: break;
	}
}


