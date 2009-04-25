// editor_cdeditor.cpp
// Project: Basic SDL Map Editor
// Author: Richard Marks

#include "common_includes.h"

void editor_set_tile_cdeditor()
{
	unsigned int tileindex = editormousetilex + (editormousetiley * editorlevelwidth);

	SDL_Rect dst;

	editorlevel->data[tileindex].solid = true;

	dst.x = (editormousetilex * editortilewidth);
	dst.y = (editormousetiley * editortileheight);

	SDL_BlitSurface(editortileset, &editortilecoordinates[editorlevel->data[tileindex].backtilevalue], editorbgmapsurface, &dst);
	if (editorlevel->data[tileindex].foretilevalue)
	{
		SDL_BlitSurface(editortileset, &editortilecoordinates[editorlevel->data[tileindex].foretilevalue - 1], editorbgmapsurface, &dst);
	}
	dst.x -= 1;
	dst.y -= 1;
	SDL_BlitSurface(editorcollisionoverlaysurface, 0, editorbgmapsurface, &dst);
}

////////////////////////////////////////////////////////////////////////////////

void editor_unset_tile_cdeditor()
{
	unsigned int tileindex = editormousetilex + (editormousetiley * editorlevelwidth);

	editorlevel->data[tileindex].solid = false;

	SDL_Rect dst;

	dst.x = (editormousetilex * editortilewidth);
	dst.y = (editormousetiley * editortileheight);

	SDL_BlitSurface(editortileset, &editortilecoordinates[editorlevel->data[tileindex].backtilevalue], editorbgmapsurface, &dst);
	if (editorlevel->data[tileindex].foretilevalue)
	{
		SDL_BlitSurface(editortileset, &editortilecoordinates[editorlevel->data[tileindex].foretilevalue - 1], editorbgmapsurface, &dst);
	}
}

////////////////////////////////////////////////////////////////////////////////

void editor_handle_input_cdeditor()
{
	unsigned int mousex = (unsigned int) editormousex;
	unsigned int mousey = (unsigned int) editormousey;

	if (!SDL_ShowCursor(SDL_QUERY))
	{
		SDL_ShowCursor(SDL_ENABLE);
	}

	if (!editorshowtileselector)
	{
		if (mousex >= editortilewidth && mousex <= editorcameraxlimit && mousey >= editortileheight && mousey <= editorcameraylimit)
		{

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
	}
}

////////////////////////////////////////////////////////////////////////////////

void editor_refresh_view_cdeditor()
{
	// blit the map
	SDL_BlitSurface(editorbgmapsurface, editorcamera->inpixels, editorscreen, editorcamera->box);
	//SDL_BlitSurface(editorfgmapsurface, editorcamera->inpixels, editorscreen, editorcamera->box);
	//SDL_BlitSurface(editorcdmapsurface, editorcamera->inpixels, editorscreen, editorcamera->box);
}













