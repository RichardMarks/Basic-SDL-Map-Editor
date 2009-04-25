// editor_create.cpp
// Project: Basic SDL Map Editor
// Author: Richard Marks

#include "common_includes.h"

void editor_create_camera()
{
	if (editorcamera)
	{
		editor_destroy_camera();
	}

	editorcamera = new EditorCamera;
	if (!editorcamera)
	{
		fprintf(stderr, "Unable to create EditorCamera instance!\n");
		exit(1);
	}

	editorcamera->intiles 	= new SDL_Rect;
	editorcamera->inpixels 	= new SDL_Rect;
	editorcamera->box		= new SDL_Rect;

	if (!editorcamera->intiles || !editorcamera->inpixels || !editorcamera->box)
	{
		fprintf(stderr, "Unable to create EditorCamera SDL_Rect instances!\n");
		exit(1);
	}

	editorcamera->intiles->x = 1;
	editorcamera->intiles->y = 1;

	// calculate the max number of tiles that we can view
	unsigned int maxtilesacross = (editorwindowwidth / editortilewidth) - 2;
	unsigned int maxtilesdown = (editorwindowheight / editortileheight) - 2;

	editorcamera->intiles->w = maxtilesacross; // 30;
	editorcamera->intiles->h = maxtilesdown; // 22;

	// make sure that small maps are still viewable
	if (editorlevel->width < editorcamera->intiles->w)
	{
		editorcamera->intiles->w = editorlevel->width;
	}

	if (editorlevel->height < editorcamera->intiles->h)
	{
		editorcamera->intiles->h = editorlevel->height;
	}

	editorcamera->inpixels->x = editortilewidth;
	editorcamera->inpixels->y = editortileheight;
	editorcamera->inpixels->w = editorcamera->intiles->w * editortilewidth;
	editorcamera->inpixels->h = editorcamera->intiles->h * editortileheight;

	editorcamera->box->x = editortilewidth;
	editorcamera->box->y = editortileheight;
	editorcamera->box->w = editorcamera->inpixels->w + editortilewidth;
	editorcamera->box->h = editorcamera->inpixels->h + editortileheight;

	editorcameraxlimit = editortilewidth * (editorcamera->intiles->w + 1);
	editorcameraylimit = editortileheight * (editorcamera->intiles->h + 1);
}

////////////////////////////////////////////////////////////////////////////////

void editor_create_map_surfaces()
{
	unsigned int width = editorlevelwidth * editortilewidth;
	unsigned int height = editorlevelheight * editortileheight;

	if (editorbgmapsurface)
	{
		SDL_FreeSurface(editorbgmapsurface);
		editorbgmapsurface = 0;
	}

	editorbgmapsurface = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_SRCCOLORKEY, width, height, editorscreen->format->BitsPerPixel, 0, 0, 0, 0);
	SDL_Surface* t = SDL_DisplayFormat(editorbgmapsurface); SDL_FreeSurface(editorbgmapsurface); editorbgmapsurface = t;

	if (!editorbgmapsurface)
	{
		fprintf(stderr, "Unable to create the map surface!\nSDL Error: %s\n", SDL_GetError());
		exit(1);
	}
}

////////////////////////////////////////////////////////////////////////////////

void editor_create_scene()
{
	#if 1
	SDL_FillRect(editorbgmapsurface, 0, SDL_MapRGB(editorscreen->format, 0x00, 0x00, 0x00));

	SDL_Rect src, dst;

	src.w = editortilewidth;
	src.h = editortileheight;

	// bg
	for (unsigned int row = 0; row < editorlevel->height; row++)
	{
		dst.y = row * editortileheight;
		for (unsigned int column = 0; column < editorlevel->width; column++)
		{
			dst.x = column * editortilewidth;

			unsigned int index 		= column + (row * editorlevel->width);
			unsigned int bgtileid 	= editorlevel->data[index].backtilevalue;

			SDL_BlitSurface(editortileset, &editortilecoordinates[bgtileid], editorbgmapsurface, &dst);
		}
	}

	// fg
	for (unsigned int row = 0; row < editorlevel->height; row++)
	{
		dst.y = row * editortileheight;
		for (unsigned int column = 0; column < editorlevel->width; column++)
		{
			dst.x = column * editortilewidth;

			unsigned int index 		= column + (row * editorlevel->width);
			unsigned int fgtileid 	= editorlevel->data[index].foretilevalue;
			if (fgtileid)
			{
				fgtileid--;
				SDL_BlitSurface(editortileset, &editortilecoordinates[fgtileid], editorbgmapsurface, &dst);
			}
		}
	}

	// cd
	if (editormode == EditorCDEdit)
	{
		for (unsigned int row = 0; row < editorlevel->height; row++)
		{
			dst.y = row * editortileheight;
			for (unsigned int column = 0; column < editorlevel->width; column++)
			{
				dst.x = column * editortilewidth;

				if (!editorlevel->data[column + (row * editorlevel->width)].solid)
				{
					continue;
				}
				else
				{
					SDL_Rect plot;
					plot.x = dst.x - 1;
					plot.y = dst.y - 1;
					SDL_BlitSurface(editorcollisionoverlaysurface, 0, editorbgmapsurface, &plot);
				}
			}
		}
	}
	#endif
}

////////////////////////////////////////////////////////////////////////////////

void editor_create_static_help_text()
{
	SDL_Color color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	editorhelptextsurface = TTF_RenderText_Solid(editorfont,
	"(C) Copyright 2009, Richard Marks <ccpsceo@gmail.com>", color);

	editorhelptextrenderx = 4 + editortilewidth;
	editorhelptextrendery = editorscreen->h - (editorhelptextsurface->h + 4);
}

////////////////////////////////////////////////////////////////////////////////

void editor_create_gui_elements()
{
	editormodebuttonpanelsurface 	= SDL_CreateRGBSurface(SDL_HWSURFACE, 28, editorcamera->inpixels->h+4, editorscreen->format->BitsPerPixel, 0, 0, 0, 0);
	editorbglayermodebuttonsurface	= SDL_CreateRGBSurface(SDL_HWSURFACE, EDITORGUIBUTTONWIDTH, EDITORGUIBUTTONHEIGHT, editorscreen->format->BitsPerPixel, 0, 0, 0, 0);
	editorfglayermodebuttonsurface	= SDL_CreateRGBSurface(SDL_HWSURFACE, EDITORGUIBUTTONWIDTH, EDITORGUIBUTTONHEIGHT, editorscreen->format->BitsPerPixel, 0, 0, 0, 0);
	editorcdlayermodebuttonsurface	= SDL_CreateRGBSurface(SDL_HWSURFACE, EDITORGUIBUTTONWIDTH, EDITORGUIBUTTONHEIGHT, editorscreen->format->BitsPerPixel, 0, 0, 0, 0);
	editorshowselectorbuttonsurface	= SDL_CreateRGBSurface(SDL_HWSURFACE, EDITORGUIBUTTONWIDTH, EDITORGUIBUTTONHEIGHT, editorscreen->format->BitsPerPixel, 0, 0, 0, 0);
	editorsavebmpbuttonsurface		= SDL_CreateRGBSurface(SDL_HWSURFACE, EDITORGUIBUTTONWIDTH, EDITORGUIBUTTONHEIGHT, editorscreen->format->BitsPerPixel, 0, 0, 0, 0);


	editorcollisionoverlaysurface	= SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_SRCCOLORKEY, editortilewidth, editortileheight, editorscreen->format->BitsPerPixel, 0, 0, 0, 0);

	SDL_SetColorKey(editorcollisionoverlaysurface, (SDL_SRCCOLORKEY | SDL_RLEACCEL), SDL_MapRGB(editorscreen->format, 0xFF, 0x00, 0xFF));

	if (!editormodebuttonpanelsurface ||
		!editorbglayermodebuttonsurface ||
		!editorfglayermodebuttonsurface ||
		!editorcdlayermodebuttonsurface ||
		!editorcollisionoverlaysurface ||
		!editorshowselectorbuttonsurface ||
		!editorsavebmpbuttonsurface)
	{
		fprintf(stderr, "GUI elements could not be created!\n");
		exit(1);
	}

	enum GuiColorNames
	{
		PanelBG,
		PanelBorder,
		ButtonBG,
		ButtonBorder,
		ButtonText,
		Transparent,
		CollisionOverlayBorder
	};

	unsigned int guicolor[] =
	{
		// panel bg
		SDL_MapRGB(editorscreen->format, 239, 235, 231),
		// panel border
		SDL_MapRGB(editorscreen->format, 115, 106, 96),
		// button bg
		SDL_MapRGB(editorscreen->format, 233, 227, 222),
		// button border
		SDL_MapRGB(editorscreen->format, 191, 185, 177),
		// button text
		SDL_MapRGB(editorscreen->format, 0, 0, 0),
		// transparent
		SDL_MapRGB(editorscreen->format, 255, 0, 255),
		// collision overlay border
		SDL_MapRGB(editorscreen->format, 255, 0, 0),
	};

	SDL_FillRect(editormodebuttonpanelsurface, 0, guicolor[PanelBG]);
	SDL_FillRect(editorbglayermodebuttonsurface, 0, guicolor[ButtonBG]);
	SDL_FillRect(editorfglayermodebuttonsurface, 0, guicolor[ButtonBG]);
	SDL_FillRect(editorcdlayermodebuttonsurface, 0, guicolor[ButtonBG]);
	SDL_FillRect(editorshowselectorbuttonsurface, 0, guicolor[ButtonBG]);
	SDL_FillRect(editorsavebmpbuttonsurface, 0, guicolor[ButtonBG]);



	SDL_Rect border;

	#if 1
	SDL_FillRect(editorcollisionoverlaysurface, 0, guicolor[Transparent]);

	border.x = border.y = 1;
	border.w = editortilewidth; border.h = 2;
	SDL_FillRect(editorcollisionoverlaysurface, &border, guicolor[CollisionOverlayBorder]);

	border.x = border.y = 1;
	border.w = 2; border.h = editortileheight;
	SDL_FillRect(editorcollisionoverlaysurface, &border, guicolor[CollisionOverlayBorder]);

	border.x = editortilewidth - 2;
	border.y = 1;
	border.w = 2; border.h = editortileheight;
	SDL_FillRect(editorcollisionoverlaysurface, &border, guicolor[CollisionOverlayBorder]);

	border.x = 1;
	border.y = editortileheight - 2;
	border.w = editortilewidth; border.h = 2;
	SDL_FillRect(editorcollisionoverlaysurface, &border, guicolor[CollisionOverlayBorder]);


	#endif


	border.x = 2;
	border.y = 2;

	border.w = editormodebuttonpanelsurface->w - 4;
	border.h = editormodebuttonpanelsurface->h - 4;
	SDL_FillRect(editormodebuttonpanelsurface, &border, guicolor[PanelBorder]);

	border.x = 1;
	border.y = 1;

	border.w = editorbglayermodebuttonsurface->w - 2;
	border.h = editorbglayermodebuttonsurface->h - 2;
	SDL_FillRect(editorbglayermodebuttonsurface, &border, guicolor[ButtonBorder]);

	border.w = editorfglayermodebuttonsurface->w - 2;
	border.h = editorfglayermodebuttonsurface->h - 2;
	SDL_FillRect(editorfglayermodebuttonsurface, &border, guicolor[ButtonBorder]);

	border.w = editorcdlayermodebuttonsurface->w - 2;
	border.h = editorcdlayermodebuttonsurface->h - 2;
	SDL_FillRect(editorcdlayermodebuttonsurface, &border, guicolor[ButtonBorder]);


	border.w = editorshowselectorbuttonsurface->w - 2;
	border.h = editorshowselectorbuttonsurface->h - 2;
	SDL_FillRect(editorshowselectorbuttonsurface, &border, guicolor[ButtonBorder]);

	border.w = editorsavebmpbuttonsurface->w - 2;
	border.h = editorsavebmpbuttonsurface->h - 2;
	SDL_FillRect(editorsavebmpbuttonsurface, &border, guicolor[ButtonBorder]);




	SDL_Rect label;
	label.x = 6;
	label.y = 6;

	SDL_Color color; Uint8 r, g, b;
	SDL_GetRGB(guicolor[ButtonText], editorscreen->format, &r, &g, &b);
	color.r = r;
	color.g = g;
	color.b = b;

	#define _TMP_LABELBTN(btn,text) {SDL_Surface*t=TTF_RenderText_Solid(editorfont,text,color); \
		if(t){SDL_BlitSurface(t,0,btn,&label);SDL_FreeSurface(t);}}

	_TMP_LABELBTN(editorbglayermodebuttonsurface, "BG")
	_TMP_LABELBTN(editorfglayermodebuttonsurface, "FG")
	_TMP_LABELBTN(editorcdlayermodebuttonsurface, "CD")
	_TMP_LABELBTN(editorshowselectorbuttonsurface, "TS")
	_TMP_LABELBTN(editorsavebmpbuttonsurface, "SS")

	#undef _TMP_LABELBTN

	SDL_Rect btnpos;

	btnpos.x = 4;

	btnpos.y = 6;
	SDL_BlitSurface(editorbglayermodebuttonsurface, 0, editormodebuttonpanelsurface, &btnpos);
	btnpos.y = 38;
	SDL_BlitSurface(editorfglayermodebuttonsurface, 0, editormodebuttonpanelsurface, &btnpos);
	btnpos.y = 70;
	SDL_BlitSurface(editorcdlayermodebuttonsurface, 0, editormodebuttonpanelsurface, &btnpos);
	btnpos.y = 102;
	SDL_BlitSurface(editorshowselectorbuttonsurface, 0, editormodebuttonpanelsurface, &btnpos);
	btnpos.y = 134;
	SDL_BlitSurface(editorsavebmpbuttonsurface, 0, editormodebuttonpanelsurface, &btnpos);
}

////////////////////////////////////////////////////////////////////////////////

void editor_create_cursor_surface()
{
	editorcursorsurface = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_SRCCOLORKEY, editortilewidth+2, editortileheight+2, editorscreen->format->BitsPerPixel, 0, 0, 0, 0);

	if (!editorcursorsurface)
	{
		fprintf(stderr, "editor cursor SDL_Surface could not be created!\n");
		exit(1);
	}

	SDL_Rect border;
	SDL_FillRect(editorcursorsurface, 0, editortextcolor);
	border.x = 1;
	border.y = 1;
	border.w = editortilewidth;
	border.h = editortileheight;
	SDL_FillRect(editorcursorsurface, &border, editorscreenbgcolor);
	editor_update_cursor_surface();
}









