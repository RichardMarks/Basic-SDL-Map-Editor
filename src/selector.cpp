// selector.cpp
// Project: Basic SDL Map Editor
// Author: Richard Marks

// C STANDARD LIBRARY
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <ctime>
#include <cctype>
#include <cstdarg>

// STL
#include <vector>
#include <string>
#include <map>
#include <algorithm>

// SDL
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

// EDITOR
#include "editor.h"
#include "selector.h"

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////

bool selector_init(SDL_Surface* tileset)
{
	if (!tileset)
	{
		#if defined(DEBUG)
		fprintf(stderr, "Cannot initialize tile selector with a null tileset!\n");
		#endif
		return false;
	}

	SDL_Color color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	selectorsharedtileset 			= tileset;
	selectorpanelview 				= new SDL_Rect;
	selectorhelptextsurface 		= TTF_RenderText_Solid(editorfont, "Use UP and DOWN arrows to scroll the tileset. Click a tile to select it, Right-click to close selector.", color);
	selectortilecount 				= ((tileset->w / editortilewidth) * (tileset->h / editortileheight));
	selectortilesacross 			= 20;
	selectortilesdown 				= 12;
	selectorpanelwidth				= editortilewidth * 20;
	selectorpanelheight				= editortileheight * (selectortilecount / selectortilesacross);
	selectorpanelview->x 			= 0;
	selectorpanelview->y 			= 0;
	selectorpanelview->w 			= selectorpanelwidth + (2*editortilewidth);
	selectorpanelview->h 			= (editortileheight * selectortilesdown);
	selectortileselectionpanel 		= SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_SRCCOLORKEY,selectorpanelwidth, selectorpanelheight,editorscreen->format->BitsPerPixel,0, 0, 0, 0);

	if (!selectortileselectionpanel)
	{
		#if defined(DEBUG)
		fprintf(stderr, "Cannot create tile selector SDL_Surface!\n");
		#endif
		return false;
	}

	SDL_SetColorKey(selectortileselectionpanel, (SDL_SRCCOLORKEY | SDL_RLEACCEL), SDL_MapRGB(selectortileselectionpanel->format, 0xFF, 0x00, 0xFF));

	selector_create_tile_selection_panel();

	return true;
}

////////////////////////////////////////////////////////////////////////////////

void selector_destroy()
{
	if (selectorpanelview)
	{
		delete selectorpanelview;
		selectorpanelview = 0;
	}

	if (selectorhelptextsurface)
	{
		SDL_FreeSurface(selectorhelptextsurface);
		selectorhelptextsurface = 0;
	}

	if (selectortileselectionpanel)
	{
		SDL_FreeSurface(selectortileselectionpanel);
		selectortileselectionpanel = 0;
	}
}

////////////////////////////////////////////////////////////////////////////////

void selector_scroll_up()
{
	if (selectorscroll > 0)
	{
		selectorscroll -= editortileheight;
		selectortileselected -= 20;
	}
}

////////////////////////////////////////////////////////////////////////////////

void selector_scroll_down()
{
	if (selectorscroll < selectorpanelheight - (editortileheight * 12))
	{
		selectorscroll += editortileheight;
		selectortileselected += 20;
	}
}

////////////////////////////////////////////////////////////////////////////////

void selector_refresh_view()
{
	/*
		draw a border on the editor screen where the tile selection panel is displayed
		draw the clipped region of the tile selection panel on the editor screen
		draw a border on the editor screen where the currently selected tile is located
		draw the currently selected tile on the editor screen inside the border above to create the illusion of a selection rectangle
		draw the tile selector help text on the editor screen

		draw the tile # near the mouse pointer when its over the tileset
	*/

	SDL_Rect border, src, dst;

	////////////////////////////////////////////////////////////////////////////
	// draw a border on the editor screen where the tile selection panel is displayed
	////////////////////////////////////////////////////////////////////////////
	#if 1
	border.x = editortilewidth - 2;
	border.y = editortileheight - 2;
	border.w = 4 + selectorpanelview->w;
	border.h = 4 + (selectorpanelview->h + (editortileheight * 2));
	SDL_FillRect(editorscreen, &border, editortextcolor);
	border.x +=2;
	border.y +=2;
	border.w -=4;
	border.h -=4;
	SDL_FillRect(editorscreen, &border, 0);
	#endif
	////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////
	// draw the clipped region of the tile selection panel on the editor screen
	////////////////////////////////////////////////////////////////////////////
	#if 1
	src.x = 0;
	src.y = selectorscroll;
	src.w = editortilewidth + selectortileselectionpanel->w;
	src.h = selectorpanelview->h;
	dst.x = (2 * editortilewidth);
	dst.y = (2 * editortileheight);
	SDL_BlitSurface(selectortileselectionpanel, &src, editorscreen, &dst);
	#endif
	////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////
	// draw a border on the editor screen where the currently selected tile is located
	////////////////////////////////////////////////////////////////////////////
	#if 1
	dst.x = (2 * editortilewidth) + ((selectortileselected % 20) * editortilewidth);
	dst.y = (2 * editortileheight) + (((selectortileselected / 20) * editortileheight) - selectorscroll);
	border.x = dst.x - 1;
	border.y = dst.y - 1;
	border.w = 2 + editortilewidth;
	border.h = 2 + editortileheight;
	SDL_FillRect(editorscreen, &border, editortextcolor);
	#endif
	////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////
	// draw the currently selected tile on the editor screen inside the border above to create the illusion of a selection rectangle
	////////////////////////////////////////////////////////////////////////////
	#if 1
	static unsigned int tilesperrow 	= (selectorsharedtileset->w / editortilewidth);
	int tilemodperrow 					= (selectortileselected % tilesperrow);
	int tileoverperrow 					= (selectortileselected / tilesperrow);
	src.x 								= 1 + tilemodperrow + (tilemodperrow * editortilewidth);
	src.y 								= 1 + tileoverperrow + (tileoverperrow * editortileheight);
	src.w 								= editortilewidth;
	src.h 								= editortileheight;
	SDL_BlitSurface(selectorsharedtileset, &src, editorscreen, &dst);
	#endif
	////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////
	// draw the tile selector help text on the editor screen
	////////////////////////////////////////////////////////////////////////////
	#if 1
	dst.x = (2 * editortilewidth) + 4;
	dst.y = (2 * editortileheight) - 14;
	SDL_BlitSurface(selectorhelptextsurface, 0, editorscreen, &dst);
	#endif
	////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////
	// draw the tile # near the mouse pointer when its over the tileset
	////////////////////////////////////////////////////////////////////////////
	#if 1
	selectortileundermouse = (selectormousetilex + (selectormousetiley * 20));
	editor_printf(selectormousex+10, selectormousey-14, editortextcolor, "%d", selectortileundermouse);
	#endif
	////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////

void selector_create_tile_selection_panel()
{
	SDL_Rect src, dst;

	src.w = editortilewidth;
	src.h = editortileheight;

	SDL_FillRect(selectortileselectionpanel, 0, editorscreenbgcolor);

	unsigned int tile 			= 0;
	unsigned int rows 			= (selectortilecount / selectortilesacross);
	unsigned int tilesperrow 	= (selectorsharedtileset->w / editortilewidth);

	for (unsigned int row = 0; row < rows; row++)
	{
		for (unsigned int column = 0; column < selectortilesacross; column++)
		{
			if (tile < selectortilecount)
			{
				int tilemodperrow 	= (tile % tilesperrow);
				int tileoverperrow 	= (tile / tilesperrow);
				src.x 				= 1 + tilemodperrow + (tilemodperrow * editortilewidth);
				src.y 				= 1 + tileoverperrow + (tileoverperrow * editortileheight);
				dst.x 				= column * editortilewidth;
				dst.y 				= row * editortileheight;
				SDL_BlitSurface(selectorsharedtileset, &src, selectortileselectionpanel, &dst);
			}
			tile++;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////

void selector_redraw_tile_selection_panel()
{

}

////////////////////////////////////////////////////////////////////////////////

void selector_handle_input()
{
	unsigned int mousex = (unsigned int) editormousex;
	unsigned int mousey = (unsigned int) editormousey;

	////////////////////////////////////////////////////////////////////////////
	// scroll selector up and down
	////////////////////////////////////////////////////////////////////////////
	if (editorkeys[SDLK_UP]) { selector_scroll_up(); }
	if (editorkeys[SDLK_DOWN]) { selector_scroll_down(); }


	////////////////////////////////////////////////////////////////////////////
	// handle interation with the tile selector panel
	////////////////////////////////////////////////////////////////////////////

	// show the mouse cursor if its hidden
	if (!SDL_ShowCursor(SDL_QUERY))
	{
		SDL_ShowCursor(SDL_ENABLE);
	}

	// if the right mouse button is down, we exit the selector with the currently selected tile

	// if we have not clicked already
	if (!selectorrmbclicked)
	{
		// check for right mouse button being down
		if (editormouseb & SDL_BUTTON(3))
		{
			// its down
			selectorrmbdown = true;
		}

		// check for right mouse button being up
		if (!(editormouseb & SDL_BUTTON(3)))
		{
			// its up
			if (selectorrmbdown)
			{
				// we clicked
				selectorrmbclicked 	= true;
				selectorrmbdown 	= false;
			}
		}
	}
	else
	{
		// we have clicked
		selectorrmbclicked	= false;
		selectorrmbdown		= false;


		editorcurrentile 		= selectortileselected;
		editorshowtileselector 	= false;
		selectormouseclicks 	= 0;
		editor_update_cursor_surface();
		SDL_Delay(500);
	}

	// if we are over the tile selector panel, then we process the tile selection
	if (mousex >= editortilewidthx2 && mousex <= editortilewidthx22 && mousey >= editortileheightx2 && mousey <= editortileheightx14)
	{
		selectormousetilex 	= ((mousex - editortilewidthx2) / editortilewidth);
		selectormousetiley 	= (selectorscroll / editortileheight) + ((mousey - editortileheightx2) / editortileheight);
		selectormousex 		= mousex;
		selectormousey 		= mousey;

		#if 1
		// if we have not clicked already
		if (!selectorlmbclicked)
		{
			// check for left mouse button being down
			if (editormouseb & SDL_BUTTON(1))
			{
				// its down
				selectorlmbdown = true;
			}

			// check for left mouse button being up
			if (!(editormouseb & SDL_BUTTON(1)))
			{
				// its up
				if (selectorlmbdown)
				{
					// we clicked
					selectorlmbclicked 	= true;
					selectorlmbdown 	= false;
				}
			}
		}
		else
		{
			// we have clicked
			selectorlmbclicked	= false;
			selectorlmbdown		= false;

			if (!selectormouseclicks)
			{
				// no clicks, so lets select the tile and up the click count
				selectortileselected = selectortileundermouse;
				selectormouseclicks++;
			}
			else
			{
				//
				if (selectortileundermouse != selectortileselected)
				{
					selectormouseclicks 	= 1;
					selectortileselected 	= selectortileundermouse;
				}
				else
				{
					editorcurrentile 			= selectortileselected;
					editorshowtileselector 		= false;
					selectormouseclicks 		= 0;
					editor_update_cursor_surface();
				}
			}
		}
		#endif
	}
}


