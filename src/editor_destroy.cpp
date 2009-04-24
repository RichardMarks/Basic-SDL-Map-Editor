// editor_destroy.cpp
// Project: Basic SDL Map Editor
// Author: Richard Marks

#include "common_includes.h"

void editor_destroy()
{
	// release any newd objects ^-^
	#define _TMP_DELRES(o) if (o){delete o;o=0;}

	_TMP_DELRES(editorevent)

	#undef _TMP_DELRES

	// release the level data
	editormapdestroyfunction(editorlevel);

	// release the surfaces
	#define _TMP_DELRES(o) if (o){SDL_FreeSurface(o); o=0; }

	_TMP_DELRES(editorhelptextsurface)
	_TMP_DELRES(editorbgmapsurface)
	//_TMP_DELRES(editorfgmapsurface)
	//_TMP_DELRES(editorcdmapsurface)

	#undef _TMP_DELRES

	editor_destroy_gui_elements();

	editor_destroy_cursor_surface();

	editor_destroy_camera();

	// release the selector
	selector_destroy();

	// release the tileset coordinate array
	if (editortilecoordinates)
	{
		delete [] editortilecoordinates;
		editortilecoordinates = 0;
	}

	// release the extended key handler
	if (editorkeywaspressed)
	{
		delete [] editorkeywaspressed;
		editorkeywaspressed = 0;
	}

	// shutdown SDL_ttf
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

void editor_destroy_gui_elements()
{
	#define _TMP_DELRES(o) if (o){SDL_FreeSurface(o); o=0; }

	_TMP_DELRES(editormodebuttonpanelsurface)
	_TMP_DELRES(editorbglayermodebuttonsurface)
	_TMP_DELRES(editorfglayermodebuttonsurface)
	_TMP_DELRES(editorcdlayermodebuttonsurface)
	_TMP_DELRES(editorcollisionoverlaysurface)
	_TMP_DELRES(editorshowselectorbuttonsurface)
	_TMP_DELRES(editorsavebmpbuttonsurface)

	#undef _TMP_DELRES
}

////////////////////////////////////////////////////////////////////////////////

void editor_destroy_cursor_surface()
{
	if (editorcursorsurface)
	{
		SDL_FreeSurface(editorcursorsurface);
		editorcursorsurface = 0;
	}
}

////////////////////////////////////////////////////////////////////////////////

void editor_destroy_camera()
{
	if (editorcamera)
	{
		if (editorcamera->intiles)
		{
			delete editorcamera->intiles;
		}

		if (editorcamera->inpixels)
		{
			delete editorcamera->inpixels;
		}

		if (editorcamera->box)
		{
			delete editorcamera->box;
		}

		delete editorcamera;
		editorcamera = 0;
	}
}

