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

// EDITOR
#include "editor.h"
#include "mapformat.h"

////////////////////////////////////////////////////////////////////////////////

MapFormat* (*editormaploadfunction)(const char* filename) 				= 0;
void (*editormapsavefunction)(MapFormat* data, const char* filename)	= 0;
void (*editormapdestroyfunction)(MapFormat* data)						= 0;
SDL_Surface* editorscreen 												= 0;
SDL_Event* editorevent 													= 0;
SDL_Rect* editortileset 												= 0;
SDL_Rect* editorcamera 													= 0;
MapFormat* editorlevel 													= 0;
int editormousetilex 													= 0;
int editormousetiley 													= 0;
unsigned int editorwindowwidth											= 1024;
unsigned int editorwindowheight											= 768;
unsigned int editorwindowbpp											= 24;
int editorcamerawidth													= 0;
int editorcameraheight													= 0;
unsigned char* editorkeys												= 0;
bool editorrunning														= false;
unsigned int editorscreenbgcolor										= 0;
////////////////////////////////////////////////////////////////////////////////

bool editor_init(int argc, char* argv[])
{
	/// @todo load startup.cfg file or create one

	// "register" map loader and saver functions
	editormaploadfunction 		= mapformat_a_load_function;
	editormapsavefunction 		= mapformat_a_save_function;
	editormapdestroyfunction	= mapformat_a_destroy_function;

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
	SDL_WM_SetCaption("Basic SDL Map Editor v0.0.1 -- by Richard Marks <ccpsceo@gmail.com>", 0);

	// create the SDL event handler instance
	editorevent = new SDL_Event;
	if (!editorevent)
	{
		fprintf(stderr, "Unable to create SDL_Event event handler instance!\n");
		return false;
	}


	editorcamera = new SDL_Rect;
	editorcamera->x = 0;
	editorcamera->y = 0;
	editorcamera->w = editorcamerawidth;
	editorcamera->h = editorcameraheight;


	editorscreenbgcolor = SDL_MapRGB(editorscreen->format, 0, 0, 0);

	editorrunning = true;

	return true;
}

////////////////////////////////////////////////////////////////////////////////

void editor_destroy()
{
	if (editorevent)
	{
		delete editorevent;
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
}

////////////////////////////////////////////////////////////////////////////////

void editor_move_camera_down()
{
}

////////////////////////////////////////////////////////////////////////////////

void editor_move_camera_left()
{
}

////////////////////////////////////////////////////////////////////////////////

void editor_move_camera_right()
{
}

////////////////////////////////////////////////////////////////////////////////

void editor_draw_tile(int tileid, int x, int y)
{
}

////////////////////////////////////////////////////////////////////////////////

void editor_draw_level()
{
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


