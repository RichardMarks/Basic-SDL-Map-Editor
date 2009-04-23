// editor_render_common.cpp
// Project: Basic SDL Map Editor
// Author: Richard Marks

#include "common_includes.h"

// renders the view that doesn't change between modes such as the gui and stuff
void editor_refresh_view_common()
{
	SDL_Rect border, dst;

	// clear screen
	SDL_FillRect(editorscreen, 0, editorscreenbgcolor);

	border.x = editortilewidth - 2;
	border.y = editortileheight - 2;
	border.w = 4 + editorcamera->inpixels->w;
	border.h = 4 + editorcamera->inpixels->h;

	// add a border around the map editing view
	SDL_FillRect(editorscreen, &border, editortextcolor);

	// blit the static help text at the bottom
	dst.x = editorhelptextrenderx;
	dst.y = editorhelptextrendery;
	SDL_BlitSurface(editorhelptextsurface, 0, editorscreen, &dst);

	// blit the gui button panels
	dst.x = EDITORGUIBUTTONPANELX;
	dst.y = EDITORGUIBUTTONPANELY;
	SDL_BlitSurface(editormodebuttonpanelsurface, 0, editorscreen, &dst);

	// blit the status bar text
	#if 1
	editor_printf(
		editortilewidth + 4,
		editortileheight - 14,
		editortextcolor,
		"Mouse Tile X %3d/%3d | Mouse Tile Y %3d/%3d | Camera X %3d | Camera Y %3d | "
		"Camera Width %2d | Camera Height %2d | Tile 0x%04X",
	editormousetilex, editorlevel->width,
	editormousetiley, editorlevel->height,
	editorcamera->intiles->x, editorcamera->intiles->y,
	editorcamera->intiles->w, editorcamera->intiles->h, editorcurrentile);
	#endif
}

