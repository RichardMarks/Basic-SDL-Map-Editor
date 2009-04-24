
// editor_init.cpp
// Project: Basic SDL Map Editor
// Author: Richard Marks

#include "common_includes.h"

bool editor_init(int argc, char* argv[])
{
	// "register" map loader and saver functions
	editormapcreatefunction		= mapformat_b_create_function;
	editormaploadfunction 		= mapformat_b_load_function;
	editormapsavefunction 		= mapformat_b_save_function;
	editormapdestroyfunction	= mapformat_b_destroy_function;

	// this is to create the first blank map file
	#if 0
	editorlevel = editormapcreatefunction(editorlevelwidth, editorlevelheight);
	editor_save_level();
	return false;
	#endif



	// init SDL

	#ifdef __linux__
	//putenv("SDL_VIDEODRIVER=glSDL");
	#endif

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		fprintf(stderr, "SDL Library Initialization Failed!\n\tSDL Error: %s\n", SDL_GetError());
		return false;
	}

	editorscreen = SDL_SetVideoMode(editorwindowwidth, editorwindowheight, editorwindowbpp, SDL_HWSURFACE | SDL_DOUBLEBUF);

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
	SDL_WM_SetCaption("Basic SDL Map Editor v0.0.5 [wip.map] -- by Richard Marks <ccpsceo@gmail.com>", 0);

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

	// load the tileset
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

	// pre-calculate all tile coordinates
	unsigned int tilesperrow 	= (editortileset->w / editortilewidth);
	unsigned int tilecount 		= ((editortileset->w / editortilewidth) * (editortileset->h / editortileheight));
	editortilecoordinates 		= new SDL_Rect [tilecount];

	for (unsigned int index = 0; index < tilecount; index++)
	{
		int tilemodperrow 	= (index % tilesperrow);
		int tileoverperrow 	= (index / tilesperrow);

		editortilecoordinates[index].x = 1 + tilemodperrow + (tilemodperrow * editortilewidth);
		editortilecoordinates[index].y = 1 + tileoverperrow + (tileoverperrow * editortileheight);
		editortilecoordinates[index].w = editortilewidth;
		editortilecoordinates[index].h = editortileheight;
	}

	////////////////////////////////////////////////////////////////////////////


	editorscreenbgcolor = SDL_MapRGB(editorscreen->format, 0, 0, 0);
	editortextcolor = SDL_MapRGB(editorscreen->format, 255, 255, 255);

	editor_load_level();

	editor_create_camera();

	editor_create_map_surfaces();
	editor_create_static_help_text();

	editor_create_gui_elements();
	editor_create_cursor_surface();

	editor_create_scene();

	SDL_GetKeyState(&editornumkeys);
	editorkeywaspressed = new bool [editornumkeys];
	for (int index = 0; index < editornumkeys; index++)
	{
		editorkeywaspressed[index] = false;
	}


	if (!selector_init(editortileset))
	{
		return false;
	}

	editor_set_mode(EditorBGEdit);

	editorrunning = true;
	return true;
}

