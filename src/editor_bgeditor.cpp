// editor_bgeditor.cpp
// Project: Basic SDL Map Editor
// Author: Richard Marks

#include "common_includes.h"

void editor_set_tile_bgeditor()
{
	editorlevel->data[editormousetilex + (editormousetiley * editorlevelwidth)].backtilevalue = editorcurrentile;

	SDL_Rect dst;

	dst.x = editormousetilex * editortilewidth;
	dst.y = editormousetiley * editortileheight;

	SDL_BlitSurface(editortileset, &editortilecoordinates[editorcurrentile], editorbgmapsurface, &dst);
}

////////////////////////////////////////////////////////////////////////////////

void editor_unset_tile_bgeditor()
{
	editorlevel->data[editormousetilex + (editormousetiley * editorlevelwidth)].backtilevalue = 0;

	SDL_Rect dst;

	dst.x = editormousetilex * editortilewidth;
	dst.y = editormousetiley * editortileheight;

	SDL_BlitSurface(editortileset, &editortilecoordinates[0], editorbgmapsurface, &dst);
}

////////////////////////////////////////////////////////////////////////////////

void editor_handle_input_bgeditor()
{
	unsigned int mousex = (unsigned int) editormousex;
	unsigned int mousey = (unsigned int) editormousey;

	if (!editorshowtileselector)
	{
		if (mousex >= editortilewidth && mousex <= editorcameraxlimit && mousey >= editortileheight && mousey <= editorcameraylimit)
		{
			SDL_ShowCursor(SDL_DISABLE);


			unsigned int mousetilex		= mousex / editortilewidth;
			unsigned int mousetiley		= mousey / editortileheight;

			editormousetilex = (editorcamera->intiles->x + mousetilex) - 1;
			editormousetiley = (editorcamera->intiles->y + mousetiley) - 1;

			if (editormouseb & SDL_BUTTON(1))
			{
				editorsettilefunction();
				editorlevelchanged = true;
			}
			else if (editormouseb & SDL_BUTTON(3))
			{
				editorunsettilefunction();
				editorlevelchanged = true;
			}
		}
		else
		{
			SDL_ShowCursor(SDL_ENABLE);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////

void editor_refresh_view_bgeditor()
{
	// blit the map
	SDL_BlitSurface(editorbgmapsurface, editorcamera->inpixels, editorscreen, editorcamera->box);

	// blit the mouse cursor
	if (!SDL_ShowCursor(SDL_QUERY))
	{
		SDL_Rect dst;

		dst.x = editortilewidth + (editormousetilex * editortilewidth);
		dst.y = editortileheight + (editormousetiley * editortileheight);

		dst.x -= (editorcamera->inpixels->x + 1);
		dst.y -= (editorcamera->inpixels->y + 1);

		SDL_BlitSurface(editorcursorsurface, 0, editorscreen, &dst);
	}
}
