// editor.cpp
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
#include "mapformat.h"

////////////////////////////////////////////////////////////////////////////////
// function pointers for handling the map format
////////////////////////////////////////////////////////////////////////////////
MapFormat* (*editormapcreatefunction)(unsigned int width, unsigned int height)	= 0;
MapFormat* (*editormaploadfunction)(const char* filename) 						= 0;
void (*editormapsavefunction)(MapFormat* data, const char* filename)			= 0;
void (*editormapdestroyfunction)(MapFormat* data)								= 0;

////////////////////////////////////////////////////////////////////////////////
// SDL pointers
////////////////////////////////////////////////////////////////////////////////
SDL_Surface* editorscreen 												= 0;
SDL_Event* editorevent 													= 0;
SDL_Surface* editortileset 												= 0;

////////////////////////////////////////////////////////////////////////////////
// camera system
////////////////////////////////////////////////////////////////////////////////
SDL_Rect* editorcamera 													= 0;

unsigned int editorcamerawidth											= 30;
unsigned int editorcameraheight											= 22;

////////////////////////////////////////////////////////////////////////////////
// level data
////////////////////////////////////////////////////////////////////////////////
MapFormat* editorlevel 													= 0;

unsigned int editorlevelwidth											= 130; // 4 full "screens" of tiles plus a 2 tile border
unsigned int editorlevelheight											= 98;

unsigned int editortilewidth											= 32;
unsigned int editortileheight											= 32;

////////////////////////////////////////////////////////////////////////////////
// editor
////////////////////////////////////////////////////////////////////////////////
TTF_Font* editorfont													= 0;
unsigned int editorwindowwidth											= 1024;
unsigned int editorwindowheight											= 768;
unsigned int editorwindowbpp											= 24;
unsigned int editormousetilex 											= 0;
unsigned int editormousetiley 											= 0;
unsigned char* editorkeys												= 0;
unsigned int editorscreenbgcolor										= 0;
unsigned int editortextcolor											= 0;
unsigned int editorcurrentile											= 0;
bool editorrunning														= false;
int editornumkeys														= 0;
bool* editorkeywaspressed												= 0;

////////////////////////////////////////////////////////////////////////////////

bool editor_init(int argc, char* argv[])
{
	/// @todo load startup.cfg file or create one

	// "register" map loader and saver functions
	editormapcreatefunction		= mapformat_a_create_function;
	editormaploadfunction 		= mapformat_a_load_function;
	editormapsavefunction 		= mapformat_a_save_function;
	editormapdestroyfunction	= mapformat_a_destroy_function;

	// this is to create the first blank map file
	#if 0
	editorlevel = editormapcreatefunction(editorlevelwidth, editorlevelheight);
	editor_save_level();
	return false;
	#endif

	// init SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		fprintf(stderr, "SDL Library Initialization Failed!\n\tSDL Error: %s\n", SDL_GetError());
		return false;
	}

	editorscreen = SDL_SetVideoMode(
		editorwindowwidth,
		editorwindowheight,
		editorwindowbpp,
		SDL_HWSURFACE | SDL_DOUBLEBUF);

	if (!editorscreen)
	{
		fprintf(stderr, "SDL Screen Initialization Failed! %dx%d@%dbpp\n\tSDL Error: %s\n",
			editorwindowwidth,
			editorwindowheight,
			editorwindowbpp,
			SDL_GetError());
		return false;
	}

	// set the window caption
	SDL_WM_SetCaption("Basic SDL Map Editor v0.0.1 [wip.map] -- by Richard Marks <ccpsceo@gmail.com>", 0);

	// create the SDL event handler instance
	editorevent = new SDL_Event;
	if (!editorevent)
	{
		fprintf(stderr, "Unable to create SDL_Event event handler instance!\n");
		return false;
	}

	if (-1 == TTF_Init())
	{
		fprintf(stderr, "Unable to initialize SDL_TTF!\nSDL_TTF Error: %s\n", TTF_GetError());
		return false;
	}


	editorfont = TTF_OpenFont("silkscreen.ttf", 8);
	if (!editorfont)
	{
		fprintf(stderr, "Unable to load \"silkscreen.ttf\"!\nSDL_TTF Error: %s\n", TTF_GetError());
		return false;
	}






	////////////////////////////////////////////////////////////////////////////
	SDL_Surface* preLoad = IMG_Load("tileset.png");
	if (!preLoad)
	{
		fprintf(stderr, "Failed to load \"tileset.png\"!\n");
		return false;
	}
	SDL_SetColorKey(preLoad, (SDL_SRCCOLORKEY | SDL_RLEACCEL), SDL_MapRGB(preLoad->format, 0xFF, 0x00, 0xFF));
	editortileset = SDL_DisplayFormat(preLoad);
	if (preLoad){SDL_FreeSurface(preLoad);}
	if (!editortileset)
	{
		fprintf(stderr, "SDL_DisplayFormat() failed or we have run out of memory!\n");
		return false;
	}
	////////////////////////////////////////////////////////////////////////////

	editorcamera 	= new SDL_Rect;
	editorcamera->x = 0;
	editorcamera->y = 0;
	editorcamera->w = editorcamerawidth;
	editorcamera->h = editorcameraheight;


	editorscreenbgcolor = SDL_MapRGB(editorscreen->format, 0, 0, 0);
	editortextcolor = SDL_MapRGB(editorscreen->format, 255, 255, 255);

	editor_load_level();


	SDL_GetKeyState(&editornumkeys);
	editorkeywaspressed = new bool [editornumkeys];
	for (int index = 0; index < editornumkeys; index++)
	{
		editorkeywaspressed[index] = false;
	}
	editorrunning = true;
	return true;
}

////////////////////////////////////////////////////////////////////////////////

void editor_destroy()
{
	#define _TMP_DELRES(o) if (o){delete o;o=0;}

	_TMP_DELRES(editorcamera)
	_TMP_DELRES(editorevent)

	#undef _TMPDELRES

	if (editorkeywaspressed)
	{
		delete [] editorkeywaspressed;
		editorkeywaspressed = 0;
	}

	if (TTF_WasInit())
	{
		if (editorfont)
		{
			TTF_CloseFont(editorfont);
			editorfont = 0;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////

void editor_execute()
{
	while(editorrunning)
	{
		while(SDL_PollEvent(editorevent))
		{
			if (SDL_QUIT == editorevent->type)
			{
				editorrunning = false;
			}
		}

		editor_handle_input();
		editor_refresh_view();

		SDL_Delay(30);
	}
}

////////////////////////////////////////////////////////////////////////////////

void editor_move_camera_up()
{
	if (editorcamera->y > 0)
	{
		editorcamera->y--;
	}
}

////////////////////////////////////////////////////////////////////////////////

void editor_move_camera_down()
{
	if ((unsigned int)editorcamera->y < editorlevelheight - editorcamera->h)
	{
		editorcamera->y++;
	}
}

////////////////////////////////////////////////////////////////////////////////

void editor_move_camera_left()
{
	if (editorcamera->x > 0)
	{
		editorcamera->x--;
	}
}

////////////////////////////////////////////////////////////////////////////////

void editor_move_camera_right()
{
	if ((unsigned int)editorcamera->x < editorlevelwidth - editorcamera->w)
	{
		editorcamera->x++;
	}
}

////////////////////////////////////////////////////////////////////////////////

void editor_draw_tile(int tileid, int x, int y)
{
	SDL_Rect src;
	SDL_Rect dst;
	static int tilesperrow = (editortileset->w / editortilewidth);
	src.x = (tileid % tilesperrow) * editortilewidth;
	src.y = (tileid / tilesperrow) * editortileheight;
	src.w = editortilewidth;
	src.h = editortileheight;
	dst.x = editortilewidth + (x * editortilewidth) - (editorcamera->x * editortilewidth);
	dst.y = editortileheight + (y * editortileheight) - (editorcamera->y * editortileheight);
	SDL_BlitSurface(editortileset, &src, editorscreen, &dst);
}

////////////////////////////////////////////////////////////////////////////////

void editor_draw_level()
{
	int maxy = editorcamera->y + editorcamera->h;
	int maxx = editorcamera->x + editorcamera->w;
	for (int row = editorcamera->y; row != maxy; row++)
	{
		for (int column = editorcamera->x; column != maxx; column++)
		{
			editor_draw_tile(editorlevel->tilevalue[column + (row * editorlevelwidth)], column, row);
		}
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

void editor_refresh_view()
{
	SDL_FillRect(editorscreen, 0, editorscreenbgcolor);
	SDL_Rect border;
	border.x = editortilewidth - 2;
	border.y = editortileheight - 2;
	border.w = 4 + (editorcamera->w * editortilewidth);
	border.h = 4 + (editorcamera->h * editortileheight);
	SDL_FillRect(editorscreen, &border, editortextcolor);

	editor_draw_level();



	if (!SDL_ShowCursor(SDL_QUERY))
	{
		SDL_Rect src;
		SDL_Rect dst;
		static int tilesperrow = (editortileset->w / editortilewidth);
		src.x = (editorcurrentile % tilesperrow) * editortilewidth;
		src.y = (editorcurrentile / tilesperrow) * editortileheight;
		src.w = editortilewidth;
		src.h = editortileheight;
		dst.x = editortilewidth + (editormousetilex * editortilewidth);
		dst.y = editortileheight + (editormousetiley * editortileheight);
		dst.x -= editorcamera->x * editortilewidth;
		dst.y -= editorcamera->y * editortileheight;
		border.x = dst.x - 1;
		border.y = dst.y - 1;
		border.w = 2 + editortilewidth;
		border.h = 2 + editortileheight;
		SDL_FillRect(editorscreen, &border, editortextcolor);
		SDL_BlitSurface(editortileset, &src, editorscreen, &dst);
	}

	editor_printf(editortilewidth+4, editortileheight-14,   editortextcolor,
		"Mouse Tile X %3d    Mouse Tile Y %3d    Camera X %3d    Camera Y %3d    "
		"Camera Width %3d    Camera Height %3d    "
		"Current Tile Index: %4d (HEX: 0x%04X)",
	editormousetilex, editormousetiley,
	editorcamera->x, editorcamera->y,
	editorcamera->w, editorcamera->h,editorcurrentile,editorcurrentile);


	editor_printf(editortilewidth+4, 4 + (editortileheight*(editorcamera->h+1)),   editortextcolor,
		"Press N to clear map, F5 to save, F8 to load, W, S, A, D, or Arrow Keys to scroll the camera, "
		"( or ) to select a tile.                (C) Copyright 2009, Richard Marks <ccpsceo@gmail.com>");



	SDL_Flip(editorscreen);
}

////////////////////////////////////////////////////////////////////////////////

void editor_handle_input()
{
	SDL_PumpEvents();
	editorkeys = SDL_GetKeyState(0);

	if (editorkeys[SDLK_ESCAPE])
	{
		editorrunning = false;
	}


	////////////////////////////////////////////////////////////////////////////
	// clears the current map data
	////////////////////////////////////////////////////////////////////////////
	if (editorkeys[SDLK_n])
	{
		if (!editorkeywaspressed[SDLK_n])
		{
			editorkeywaspressed[SDLK_n] = true;
		}
	}
	else
	{
		if (editorkeywaspressed[SDLK_n])
		{
			for (unsigned int row = 0; row < editorlevelheight; row++)
			{
				for (unsigned int column = 0; column < editorlevelwidth; column++)
				{
					editorlevel->tilevalue[column + (row * editorlevelwidth)] = 0;
				}
			}
			editorkeywaspressed[SDLK_n] = false;
		}
	}

	////////////////////////////////////////////////////////////////////////////
	// next tile in tileset
	////////////////////////////////////////////////////////////////////////////
	if (editorkeys[SDLK_0])
	{
		if (!editorkeywaspressed[SDLK_0])
		{
			editorkeywaspressed[SDLK_0] = true;
		}
	}
	else
	{
		if (editorkeywaspressed[SDLK_0])
		{
			if (editorcurrentile < ((editortileset->w / editortilewidth) * (editortileset->h / editortileheight)))
			{
				editorcurrentile++;
			}
			editorkeywaspressed[SDLK_0] = false;
		}
	}

	////////////////////////////////////////////////////////////////////////////
	// previous tile in tileset
	////////////////////////////////////////////////////////////////////////////
	if (editorkeys[SDLK_9])
	{
		if (!editorkeywaspressed[SDLK_9])
		{
			editorkeywaspressed[SDLK_9] = true;
		}
	}
	else
	{
		if (editorkeywaspressed[SDLK_9])
		{
			if (editorcurrentile > 0)
			{
				editorcurrentile--;
			}
			editorkeywaspressed[SDLK_9] = false;
		}
	}

	////////////////////////////////////////////////////////////////////////////
	// load wip.map
	////////////////////////////////////////////////////////////////////////////
	if (editorkeys[SDLK_F8])
	{
		if (!editorkeywaspressed[SDLK_F8])
		{
			editorkeywaspressed[SDLK_F8] = true;
		}
	}
	else
	{
		if (editorkeywaspressed[SDLK_F8])
		{
			editor_load_level();
			editorkeywaspressed[SDLK_F8] = false;
		}
	}

	////////////////////////////////////////////////////////////////////////////
	// save wip.map
	////////////////////////////////////////////////////////////////////////////
	if (editorkeys[SDLK_F5])
	{
		if (!editorkeywaspressed[SDLK_F5])
		{
			editorkeywaspressed[SDLK_F5] = true;
		}
	}
	else
	{
		if (editorkeywaspressed[SDLK_F5])
		{
			editor_save_level();
			editorkeywaspressed[SDLK_F5] = false;
		}
	}

	////////////////////////////////////////////////////////////////////////////
	// scroll up
	////////////////////////////////////////////////////////////////////////////
	if (editorkeys[SDLK_UP] || editorkeys[SDLK_w])
	{
		editor_move_camera_up();
	}

	////////////////////////////////////////////////////////////////////////////
	// scroll down
	////////////////////////////////////////////////////////////////////////////
	if (editorkeys[SDLK_DOWN] || editorkeys[SDLK_s])
	{
		editor_move_camera_down();
	}

	////////////////////////////////////////////////////////////////////////////
	// scroll left
	////////////////////////////////////////////////////////////////////////////
	if (editorkeys[SDLK_LEFT] || editorkeys[SDLK_a])
	{
		editor_move_camera_left();
	}

	////////////////////////////////////////////////////////////////////////////
	// scroll right
	////////////////////////////////////////////////////////////////////////////
	if (editorkeys[SDLK_RIGHT] || editorkeys[SDLK_d])
	{
		editor_move_camera_right();
	}

	////////////////////////////////////////////////////////////////////////////
	// handle mouse input
	////////////////////////////////////////////////////////////////////////////
	int mousex, mousey;
	unsigned int mouseb = SDL_GetMouseState(&mousex, &mousey);


	if (
		(unsigned int)mousex >= editortilewidth && (unsigned int)mousex <= ((1 + editorcamera->w) * editortilewidth) &&
		(unsigned int)mousey >= editortileheight && (unsigned int)mousey <= ((1 + editorcamera->h) * editortileheight))
	{
		SDL_ShowCursor(SDL_DISABLE);
		editormousetilex = editorcamera->x + ((mousex - editortilewidth) / editortilewidth);
		editormousetiley = editorcamera->y + ((mousey - editortileheight) / editortileheight);

		if (mouseb & SDL_BUTTON(1))
		{
			editorlevel->tilevalue[editormousetilex + (editormousetiley * editorlevelwidth)] = editorcurrentile;
		}
		else if (mouseb & SDL_BUTTON(3))
		{
			editorlevel->tilevalue[editormousetilex + (editormousetiley * editorlevelwidth)] = 0;
		}
	}
	else
	{
		SDL_ShowCursor(SDL_ENABLE);
	}
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



