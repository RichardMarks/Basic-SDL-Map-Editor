// editor_cdeditor.cpp
// Project: Basic SDL Map Editor
// Author: Richard Marks

#include "common_includes.h"

void editor_set_tile_cdeditor()
{
	SDL_Rect dst;

	editorlevel->data[editormousetilex + (editormousetiley * editorlevelwidth)].solid = true;

	dst.x = (editormousetilex * editortilewidth) - 1;
	dst.y = (editormousetiley * editortileheight) - 1;

	SDL_BlitSurface(editorcollisionoverlaysurface, 0, editorcdmapsurface, &dst);

	//dst.x = (editormousetilex * editortilewidth) - 1;
	//dst.y = (editormousetiley * editortileheight) - 1;
	//dst.w = editortilewidth + 2;
	//dst.h = editortileheight + 2;

	//SDL_FillRect(editorcdmapsurface, &dst, SDL_MapRGB(editorscreen->format, 255, 0, 0));

}

////////////////////////////////////////////////////////////////////////////////

void editor_unset_tile_cdeditor()
{
	editorlevel->data[editormousetilex + (editormousetiley * editorlevelwidth)].solid = false;

	SDL_Rect dst;

	dst.x = (editormousetilex * editortilewidth) - 1;
	dst.y = (editormousetiley * editortileheight) - 1;
	dst.w = editortilewidth + 2;
	dst.h = editortileheight + 2;

	SDL_FillRect(editorcdmapsurface, &dst, SDL_MapRGB(editorscreen->format, 255, 0, 255));
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
			}
			else if (editormouseb & SDL_BUTTON(3))
			{
				editorunsettilefunction();
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////

void editor_refresh_view_cdeditor()
{
	// blit the map
	SDL_BlitSurface(editorbgmapsurface, editorcamera->inpixels, editorscreen, editorcamera->box);
	SDL_BlitSurface(editorfgmapsurface, editorcamera->inpixels, editorscreen, editorcamera->box);
	SDL_BlitSurface(editorcdmapsurface, editorcamera->inpixels, editorscreen, editorcamera->box);
}













