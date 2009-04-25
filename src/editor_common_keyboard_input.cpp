// editor_common_keyboard_input.cpp
// Project: Basic SDL Map Editor
// Author: Richard Marks

#include "common_includes.h"

// handles the keyboard input that is common to all editing modes
void editor_handle_keyboard_input_common()
{
	/// new in v0.0.5 -- makes adding key controls easier
	#define IF_SINGLEKEY_START(k) if(editorkeys[k]){if(!editorkeywaspressed[k]){editorkeywaspressed[k]=true;}}else{if(editorkeywaspressed[k]){
	#define IF_SINGLEKEY_END(k) editorkeywaspressed[k]=false;}}

	////////////////////////////////////////////////////////////////////////////
	// exit the map editor
	////////////////////////////////////////////////////////////////////////////
	IF_SINGLEKEY_START(SDLK_ESCAPE)
	{
		// get the hell out the editor already
		editorrunning = false;
	}
	IF_SINGLEKEY_END(SDLK_ESCAPE)

	////////////////////////////////////////////////////////////////////////////
	// nukes all of the current map data
	////////////////////////////////////////////////////////////////////////////
	IF_SINGLEKEY_START(SDLK_n)
	{
		editorlevelchanged = true;
		for (unsigned int row = 0; row < editorlevelheight; row++)
		{
			for (unsigned int column = 0; column < editorlevelwidth; column++)
			{
				unsigned int index = column + (row * editorlevelwidth);
				editorlevel->data[index].backtilevalue 	= 0;
				editorlevel->data[index].foretilevalue 	= 0;
				editorlevel->data[index].solid 			= false;
			}
		}
		editor_create_scene();
	}
	IF_SINGLEKEY_END(SDLK_n)

	////////////////////////////////////////////////////////////////////////////
	// toggle tile selector panel
	////////////////////////////////////////////////////////////////////////////
	IF_SINGLEKEY_START(SDLK_TAB)
	{
		if (editormode != EditorCDEdit)
		{
			editorshowtileselector = !editorshowtileselector;
			selector_redraw_tile_selection_panel();
		}
	}
	IF_SINGLEKEY_END(SDLK_TAB)


	////////////////////////////////////////////////////////////////////////////
	// save wip.map
	////////////////////////////////////////////////////////////////////////////
	IF_SINGLEKEY_START(SDLK_s)
	{
		editor_save_level();
	}
	IF_SINGLEKEY_END(SDLK_s)
	IF_SINGLEKEY_START(SDLK_F5)
	{
		editor_save_level();
	}
	IF_SINGLEKEY_END(SDLK_F5)

	////////////////////////////////////////////////////////////////////////////
	// load wip.map
	////////////////////////////////////////////////////////////////////////////
	IF_SINGLEKEY_START(SDLK_l)
	{
		editor_load_level();
		editor_create_scene();
	}
	IF_SINGLEKEY_END(SDLK_l)
	IF_SINGLEKEY_START(SDLK_F8)
	{
		editor_load_level();
		editor_create_scene();
	}
	IF_SINGLEKEY_END(SDLK_F8)

	////////////////////////////////////////////////////////////////////////////
	// jump to first row
	////////////////////////////////////////////////////////////////////////////
	IF_SINGLEKEY_START(SDLK_PAGEUP)
	{
		editorcamera->intiles->y = 0;
		editorcamera->inpixels->y = 0;
	}
	IF_SINGLEKEY_END(SDLK_PAGEUP)

	////////////////////////////////////////////////////////////////////////////
	// jump to first column
	////////////////////////////////////////////////////////////////////////////
	IF_SINGLEKEY_START(SDLK_HOME)
	{
		editorcamera->intiles->x = 0;
		editorcamera->inpixels->x = 0;
	}
	IF_SINGLEKEY_END(SDLK_HOME)

	////////////////////////////////////////////////////////////////////////////
	// jump to last row
	////////////////////////////////////////////////////////////////////////////
	IF_SINGLEKEY_START(SDLK_PAGEDOWN)
	{
		editorcamera->intiles->y = editorlevel->height - editorcamera->intiles->h;
		editorcamera->inpixels->y = editorcamera->intiles->y * editortileheight;
	}
	IF_SINGLEKEY_END(SDLK_PAGEDOWN)

	////////////////////////////////////////////////////////////////////////////
	// jump to last column
	////////////////////////////////////////////////////////////////////////////
	IF_SINGLEKEY_START(SDLK_END)
	{
		editorcamera->intiles->x = editorlevel->width - editorcamera->intiles->w;
		editorcamera->inpixels->x = editorcamera->intiles->x * editortilewidth;
	}
	IF_SINGLEKEY_END(SDLK_END)
}

