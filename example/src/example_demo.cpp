// example_demo.cpp
// Project: Basic SDL Map Editor Example Demo
// Author: Richard Marks

// C STANDARD LIBRARY
#include <cstdio>
#include <cstdlib>

// SDL
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#define SCREEN_W 	1024
#define SCREEN_H 	768
#define SCREEN_BPP 	24

// a useful keyboard handler structure to simplify SDL keyboard control
struct Keyboard
{
	// this holds the number of keys
	int keycount;

	// this holds the state of every key
	unsigned char* key;

	// this is used to create non-repeating key presses
	bool* keywaspressed;
};

// creates a new instance of the keyboard handler structure
// make sure you call this AFTER you init SDL and only call it once
Keyboard* keyboard_create()
{
	Keyboard* keyboard = new Keyboard;

	keyboard->key = SDL_GetKeyState(&keyboard->keycount);

	keyboard->keywaspressed = new bool [keyboard->keycount];

	for (int index = 0; index < keyboard->keycount; index++)
	{
		keyboard->keywaspressed[index] = false;
	}

	return keyboard;
}

// updates the keyboard structure for us
void keyboard_update(Keyboard* keyboard)
{
	// bail out the function if the pointer is invalid
	if (!keyboard)
	{
		return;
	}

	SDL_PumpEvents();
	keyboard->key = SDL_GetKeyState(0);
}

// destroys the keyboard handler structure
void keyboard_destroy(Keyboard* keyboard)
{
	// bail out the function if the pointer is invalid
	if (!keyboard)
	{
		return;
	}

	// clear our tracking array
	if (keyboard->keywaspressed)
	{
		delete [] keyboard->keywaspressed;
	}

	// free the pointer
	delete keyboard;

	// init to zero to prevent logic errors
	keyboard = 0;
}


// our basic tileset structure
struct Tileset
{
	// this holds the image of all the tiles
	SDL_Surface* sheet;

	// this holds the rectangles of all the tiles
	SDL_Rect* tile;

	// keep track of the size of the tiles
	unsigned int width;
	unsigned int height;

	// keeps track of how many tiles are available
	unsigned int count;
};

// creates a new tileset structure
Tileset* tileset_create()
{
	// allocate
	Tileset* tileset = new Tileset;

	if (!tileset)
	{
		fprintf(stderr, "failed to create the tileset instance!\n");
		return 0;
	}

	// init members to zero to keep errors out the logic
	tileset->sheet 	= 0;
	tileset->tile = 0;
	tileset->width = 0;
	tileset->height = 0;
	tileset->count = 0;

	fprintf(stderr, "created an empty new tileset instance!\n");

	return tileset;
}

// destroys a tileset structure
void tileset_destroy(Tileset* tileset)
{
	// if its a null pointer, we bail out the function
	if (!tileset)
	{
		return;
	}

	// free the tile sheet surface
	if (tileset->sheet)
	{
		SDL_FreeSurface(tileset->sheet);
	}

	// free the tile coordinate array
	if (tileset->tile)
	{
		delete [] tileset->tile;
	}

	// free the tileset pointer
	delete tileset;

	// init pointer to zero to keep errors out the logic
	tileset = 0;

	fprintf(stderr, "destroyed the tileset instance!\n");
}

// loads the tile imagery in filename into the tileset
// and pre-calculates the positions of every tile
Tileset* tileset_load(const char* filename, unsigned int tilewidth, unsigned int tileheight)
{
	fprintf(stderr, "loading \"%s\"...\n", filename);

	// load the image file using the SDL_image library function IMG_Load
	// and set the tileset sheet pointer
	SDL_Surface* preload = IMG_Load(filename);

	if (!preload)
	{
		fprintf(stderr, "Unable to load \"%s\"!\n", filename);
		return 0;
	}

	SDL_SetColorKey(preload, (SDL_SRCCOLORKEY | SDL_RLEACCEL), SDL_MapRGB(preload->format, 0xFF, 0x00, 0xFF));

	// create a new tileset (remember that the data is not initialized)
	Tileset* tileset = tileset_create();

	tileset->sheet = SDL_DisplayFormat(preload);

	if (preload)
	{
		SDL_FreeSurface(preload);
	}

	if (!tileset->sheet)
	{
		fprintf(stderr, "SDL_DisplayFormat() failed or we have run out of memory!\n");
		return 0;
	}

	// precalculate the coordinates of each tile
	tileset->width = tilewidth;
	tileset->height = tileheight;
	tileset->count = ((tileset->sheet->w / tileset->width) * (tileset->sheet->h / tileset->height));

	unsigned int tilesperrow 	= tileset->sheet->w / tileset->width;

	tileset->tile = new SDL_Rect [tileset->count];

	for (unsigned int index = 0; index < tileset->count; index++)
	{
		int tilemodperrow 	= (index % tilesperrow);
		int tileoverperrow 	= (index / tilesperrow);

		tileset->tile[index].x = 1 + tilemodperrow + (tilemodperrow * tileset->width);
		tileset->tile[index].y = 1 + tileoverperrow + (tileoverperrow * tileset->height);
		tileset->tile[index].w = tileset->width;
		tileset->tile[index].h = tileset->height;
	}

	fprintf(stderr, "tileset \"%s\" loaded!\n", filename);
	fprintf(stderr, "tileset \"%s\" has %d tiles of %dx%d pixels!\n", filename, tileset->count, tileset->width, tileset->height);

	return tileset;
}


/*
	There are lots of ways that you can make use of the maps.
	One is to simply include the mapformat.h and mapformat.cpp files directly
	in your project, and then make use of the MapFormat struct and functions
	to handle everything for you. This is recommended so that your code will
	always use the right code for handling the map data.

	However, I am going to show you directly here how to use the maps without
	using the other files. You're going to roll-your-own file loaders here.
*/

// our map structure
struct World
{
	// the size of the world in tiles
	unsigned int width;
	unsigned int height;

	// the background layer
	unsigned int* bg;

	// the foreground layer
	unsigned int* fg;

	// the collision layer
	bool* solid;
};

// creates a new world structure
World* world_create()
{
	// a whole new world ..a dazzling place... lol ^-^
	World* world = new World;

	if (!world)
	{
		fprintf(stderr, "failed to create the new world instance!\n");
		return 0;
	}

	world->width = 0;
	world->height = 0;
	world->bg = 0;
	world->fg = 0;
	world->solid = 0;

	fprintf(stderr, "created an empty new world instance!\n");

	return world;
}

// destroys a world structure
void world_destroy(World* world)
{
	// cannot destroy a world that doesn't exist!
	if (!world)
	{
		return;
	}

	// free the background layer
	if (world->bg) { delete [] world->bg; }

	// free the foreground layer
	if (world->fg) { delete [] world->fg; }

	// free the collision layer
	if (world->solid) { delete [] world->solid; }

	// free the world and init the pointer back to zero
	delete world;
	world = 0;

	fprintf(stderr, "destroyed the world instance!\n");
}

// loads a map file into the world structure
World* world_load(const char* filename)
{
	fprintf(stderr, "loading \"%s\"...\n", filename);

	// try to open the map file
	FILE* fp = fopen(filename, "rb");

	if (!fp)
	{
		fprintf(stderr, "Unable to open \"%s\" for reading!\n", filename);
		return 0;
	}

	// read the header of the map file
	unsigned int header[6];
	fread(header, sizeof(header), 1, fp);

	if (!(
		header[0] == 'M' &&
		header[1] == 'A' &&
		header[2] == 'P' &&
		header[3] == 'B'))
	{
		fprintf(stderr, "The file \"%s\" contains an invalid header!\nExpected MAPB file format!\n", filename);
		return 0;
	}

	// create the world instance that will hold our loaded data
	World* world = world_create();

	// set the world size
	world->width = header[4];
	world->height = header[5];

	// create the world data arrays
	unsigned int elements = world->width * world->height;
	world->bg = new unsigned int [elements];
	world->fg = new unsigned int [elements];
	world->solid = new bool [elements];

	// read the map data
	for (unsigned int index = 0; index < elements; index++)
	{
		unsigned int flag;
		fread(&world->bg[index], sizeof(unsigned int), 1, fp);
		fread(&world->fg[index], sizeof(unsigned int), 1, fp);
		fread(&flag, sizeof(unsigned int), 1, fp);
		world->solid[index] = (bool)((!flag) ? false : true);
	}

	// close the map file
	fclose(fp);


	fprintf(stderr, "loaded \"%s\" into the new world instance!\n", filename);

	return world;
}

// a simple camera structure
struct Camera
{
	// the size and position of the camera in tiles
	SDL_Rect* intiles;

	// the size and position of the camera in pixels
	SDL_Rect* inpixels;

	// the pixel location on the screen that the camera will be anchored to
	SDL_Rect* anchor;
};

// creates a new camera structure
Camera* camera_create(int x, int y, int widthintiles, int heightintiles, int tilewidth, int tileheight)
{
	Camera* camera = new Camera;
	camera->intiles = new SDL_Rect;
	camera->inpixels = new SDL_Rect;
	camera->anchor = new SDL_Rect;

	if (!camera->intiles || !camera->inpixels || !camera->anchor)
	{
		fprintf(stderr, "Unable to create the SDL_Rect instances for the Camera!\n");
		return 0;
	}

	camera->intiles->x = 0;
	camera->intiles->y = 0;
	camera->intiles->w = widthintiles;
	camera->intiles->h = heightintiles;
	camera->inpixels->x = tilewidth;
	camera->inpixels->y = tileheight;
	camera->inpixels->w = camera->intiles->w * tilewidth;
	camera->inpixels->h = camera->intiles->h * tileheight;
	camera->anchor->x = x;
	camera->anchor->y = y;
	camera->anchor->w = camera->inpixels->w + tilewidth;
	camera->anchor->h = camera->inpixels->h + tileheight;

	fprintf(stderr, "created a new camera instance!\n");
	return camera;
}

// destroys a camera structure
void camera_destroy(Camera* camera)
{
	// bail out the function if the pointer is invalid
	if (!camera)
	{
		return;
	}

	// free the members
	if (camera->intiles) { delete camera->intiles; }
	if (camera->inpixels) { delete camera->inpixels; }
	if (camera->anchor) { delete camera->anchor; }

	// free the pointer and init back to zero
	delete camera;
	camera = 0;

	fprintf(stderr, "destroyed the camera instance!\n");
}

// our renderer structure
struct Renderer
{
	// the tileset to use for rendering
	Tileset* tileset;

	// the world structure to render
	World* world;

	// the camera that defines what part of the world is rendered
	Camera* camera;

	// the surface that holds the entire pre-renderer tile map
	SDL_Surface* scene;
};

// creates a new renderer structure
Renderer* renderer_create(World* world, Tileset* tileset, Camera* camera)
{
	// we need these things to be valid
	if (!world || !tileset || !camera)
	{
		fprintf(stderr, "Unable to create the renderer structure! invalid world, tileset, or camera pointers passed!\n");
		return 0;
	}

	Renderer* renderer = new Renderer;
	renderer->tileset = tileset;
	renderer->world = world;
	renderer->camera = camera;
	renderer->scene = 0;

	fprintf(stderr, "created a new renderer instance!\n");

	return renderer;
}

// pre-renders the scene
void renderer_pre_render(Renderer* renderer)
{
	// bail out the function if the pointer is invalid
	if (!renderer)
	{
		return;
	}

	fprintf(stderr, "renderer is pre-rendering the scene!\n");

	// if the scene has not been created
	if (!renderer->scene)
	{
		// create it
		renderer->scene = SDL_CreateRGBSurface(
			SDL_HWSURFACE | SDL_SRCCOLORKEY,
			renderer->world->width * renderer->tileset->width,
			renderer->world->height * renderer->tileset->height,
			SDL_GetVideoSurface()->format->BitsPerPixel, 0, 0, 0, 0);
		SDL_Surface* temp = SDL_DisplayFormat(renderer->scene);
		SDL_FreeSurface(renderer->scene);
		renderer->scene = temp;
	}

	// pre-render the scene
	SDL_Rect src, dst;
	SDL_FillRect(renderer->scene, 0, 0);
	src.w = renderer->tileset->width;
	src.h = renderer->tileset->height;

	// background layer
	for (unsigned int row = 0; row < renderer->world->height; row++)
	{
		dst.y = row * renderer->tileset->height;
		for (unsigned int column = 0; column < renderer->world->width; column++)
		{
			dst.x = column * renderer->tileset->width;
			SDL_BlitSurface(
				renderer->tileset->sheet,
				&renderer->tileset->tile[renderer->world->bg[column + (row * renderer->world->width)]],
				renderer->scene,
				&dst);
		}
	}

	// foreground layer
	for (unsigned int row = 0; row < renderer->world->height; row++)
	{
		dst.y = row * renderer->tileset->height;
		for (unsigned int column = 0; column < renderer->world->width; column++)
		{
			dst.x = column * renderer->tileset->width;
			unsigned int tilevalue = renderer->world->fg[column + (row * renderer->world->width)];
			if (tilevalue)
			{
				SDL_BlitSurface(
					renderer->tileset->sheet,
					&renderer->tileset->tile[tilevalue - 1],
					renderer->scene,
					&dst);
			}
		}
	}

	fprintf(stderr, "renderer has finished pre-rendering the scene!\n");
}

// renders
void renderer_render(Renderer* renderer)
{
	// bail out the function if the pointer is invalid
	if (!renderer)
	{
		return;
	}

	// blit the scene
	SDL_BlitSurface(
		renderer->scene,
		renderer->camera->inpixels,
		SDL_GetVideoSurface(),
		renderer->camera->anchor);
}

// destroys a renderer structure
void renderer_destroy(Renderer* renderer)
{
	// bail out the function if the pointer is invalid
	if (!renderer)
	{
		return;
	}

	// free the scene surface
	if (renderer->scene)
	{
		SDL_FreeSurface(renderer->scene);
	}

	// free the pointer and init the pointer back to zero
	delete renderer;
	renderer = 0;

	fprintf(stderr, "destroyed the renderer instance!\n");
}

int main(int argc, char* argv[])
{
	// our demo variables
	World* demoworld = 0;
	Camera* democamera = 0;
	Tileset* demotileset = 0;
	Renderer* demorenderer = 0;

	// init SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		fprintf(stderr, "SDL Library Initialization Failed!\n\tSDL Error: %s\n", SDL_GetError());
		exit(1);
	}
	atexit(SDL_Quit);

	// create the window
	if (!SDL_SetVideoMode(SCREEN_W, SCREEN_H, SCREEN_BPP, SDL_HWSURFACE | SDL_DOUBLEBUF))
	{
		fprintf(stderr, "SDL Screen Initialization Failed! %dx%d@%dbpp\nSDL Error: %s\n",
			SCREEN_W,
			SCREEN_H,
			SCREEN_BPP,
			SDL_GetError());
		exit(1);
	}

	// set the window caption
	SDL_WM_SetCaption("Basic SDL Map Editor Example Demo -- by Richard Marks <ccpsceo@gmail.com>", 0);

	// create our keyboard handler
	Keyboard* keyboard = keyboard_create();

	// load the world
	demoworld = world_load("example_demo.map");
	if (!demoworld)
	{
		fprintf(stderr, "unable to load the world file!\n");
		exit(1);
	}

	// load the tileset
	demotileset = tileset_load("example_demo.png", 32, 32);
	if (!demotileset)
	{
		fprintf(stderr, "unable to load the tileset file!\n");
		exit(1);
	}

	// create the camera
	democamera = camera_create(
		// anchor position of the camera is at 32,32 on the screen
		32, 32,
		// the camera view is 30 x 22 tiles
		30, 22,
		// need to tell the camera the size of the tiles
		demotileset->width, demotileset->height);

	// create the renderer
	demorenderer = renderer_create(demoworld, demotileset, democamera);

	// pre-render the scene
	renderer_pre_render(demorenderer);

	fprintf(stderr, "\nstarting main loop...\n\n");
	// start the main loop
	SDL_Event event;
	bool running = true;
	int startticks = SDL_GetTicks();
	int mainlooptimer = SDL_GetTicks();
	while(running)
	{
		// check for our window being closed
		while(SDL_PollEvent(&event))
		{
			if (SDL_QUIT == event.type)
			{
				running = false;
			}
		}

		// update our keyboard handler
		keyboard_update(keyboard);

		// start timing our code
		startticks = SDL_GetTicks();

		// check for our controls

		// ESC and Q quits
		if (keyboard->key[SDLK_ESCAPE] || keyboard->key[SDLK_q])
		{
			running = false;
		}

		const unsigned int CAMERASPEED = 16;

		// W and up arrow
		if (keyboard->key[SDLK_w] || keyboard->key[SDLK_UP])
		{
			if (democamera->inpixels->y > 0)
			{
				democamera->inpixels->y -= CAMERASPEED;
			}
		}

		// S and down arrow
		if (keyboard->key[SDLK_s] || keyboard->key[SDLK_DOWN])
		{
			if (democamera->inpixels->y < demorenderer->scene->h - democamera->inpixels->h)
			{
				democamera->inpixels->y += CAMERASPEED;
			}
		}

		// A and left arrow
		if (keyboard->key[SDLK_a] || keyboard->key[SDLK_LEFT])
		{
			if (democamera->inpixels->x > 0)
			{
				democamera->inpixels->x -= CAMERASPEED;
			}
		}

		// D and right arrow
		if (keyboard->key[SDLK_d] || keyboard->key[SDLK_RIGHT])
		{
			if (democamera->inpixels->x < demorenderer->scene->w - democamera->inpixels->w)
			{
				democamera->inpixels->x += CAMERASPEED;
			}
		}

		// render our scene
		renderer_render(demorenderer);

		// flip our double buffer
		SDL_Flip(SDL_GetVideoSurface());

		// lock our framerate to roughly 30 FPS
		int tickdiff = SDL_GetTicks() - startticks;
		if (tickdiff < 33)
		{
			SDL_Delay(33 - tickdiff);
		}
	}

	int tickdiff = SDL_GetTicks() - mainlooptimer;

	fprintf(stderr, "finished after %d ticks!\n\n", tickdiff);

	// destroy our pointers
	tileset_destroy(demotileset);
	renderer_destroy(demorenderer);
	world_destroy(demoworld);
	camera_destroy(democamera);
	keyboard_destroy(keyboard);

	return 0;
}
