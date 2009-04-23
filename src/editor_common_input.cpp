// editor_common_input.cpp
// Project: Basic SDL Map Editor
// Author: Richard Marks

#include "common_includes.h"

// handles the input that is common to all editing modes and gui interaction
void editor_handle_input_common()
{
	SDL_PumpEvents();
	editorkeys = SDL_GetKeyState(0);
	editormouseb = SDL_GetMouseState(&editormousex, &editormousey);

	unsigned int mousex = (unsigned int) editormousex;
	unsigned int mousey = (unsigned int) editormousey;

	////////////////////////////////////////////////////////////////////////////
	// exit the map editor (does NOT save the map first!)
	////////////////////////////////////////////////////////////////////////////
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
					unsigned int index = column + (row * editorlevelwidth);
					editorlevel->data[index].backtilevalue 	= 0;
					editorlevel->data[index].foretilevalue 	= 0;
					editorlevel->data[index].solid 			= false;
				}
			}
			editor_create_scene();
			editorkeywaspressed[SDLK_n] = false;
		}
	}

	////////////////////////////////////////////////////////////////////////////
	// toggle tile selector panel
	////////////////////////////////////////////////////////////////////////////
	if (editorkeys[SDLK_TAB])
	{
		if (!editorkeywaspressed[SDLK_TAB])
		{
			editorkeywaspressed[SDLK_TAB] = true;
		}
	}
	else
	{
		if (editorkeywaspressed[SDLK_TAB])
		{
			if (editormode != EditorCDEdit)
			{
				editorshowtileselector = !editorshowtileselector;
				selector_redraw_tile_selection_panel();
			}

			editorkeywaspressed[SDLK_TAB] = false;
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
			editor_select_next_tile();
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
			editor_select_previous_tile();
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
			editor_create_scene();
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
		if (editorshowtileselector)
		{
			selector_scroll_up();
		}
		else
		{
			editor_move_camera_up();
		}
	}

	////////////////////////////////////////////////////////////////////////////
	// jump to first row
	////////////////////////////////////////////////////////////////////////////
	if (editorkeys[SDLK_PAGEUP])
	{
		editorcamera->intiles->y = 0;
		editorcamera->inpixels->y = 0;
	}

	////////////////////////////////////////////////////////////////////////////
	// jump to first column
	////////////////////////////////////////////////////////////////////////////
	if (editorkeys[SDLK_HOME])
	{
		editorcamera->intiles->x = 0;
		editorcamera->inpixels->x = 0;
	}

	////////////////////////////////////////////////////////////////////////////
	// jump to last row
	////////////////////////////////////////////////////////////////////////////
	if (editorkeys[SDLK_PAGEDOWN])
	{
		editorcamera->intiles->y = editorlevel->height - editorcamera->intiles->h;
		editorcamera->inpixels->y = editorcamera->intiles->y * editortileheight;
	}

	////////////////////////////////////////////////////////////////////////////
	// jump to last column
	////////////////////////////////////////////////////////////////////////////
	if (editorkeys[SDLK_END])
	{
		editorcamera->intiles->x = editorlevel->width - editorcamera->intiles->w;
		editorcamera->inpixels->x = editorcamera->intiles->x * editortilewidth;
	}

	////////////////////////////////////////////////////////////////////////////
	// scroll down
	////////////////////////////////////////////////////////////////////////////
	if (editorkeys[SDLK_DOWN] || editorkeys[SDLK_s])
	{
		if (editorshowtileselector)
		{
			selector_scroll_down();
		}
		else
		{
			editor_move_camera_down();
		}
	}

	////////////////////////////////////////////////////////////////////////////
	// scroll left
	////////////////////////////////////////////////////////////////////////////
	if (editorkeys[SDLK_LEFT] || editorkeys[SDLK_a])
	{
		if (!editorshowtileselector)
		{
			editor_move_camera_left();
		}
	}

	////////////////////////////////////////////////////////////////////////////
	// scroll right
	////////////////////////////////////////////////////////////////////////////
	if (editorkeys[SDLK_RIGHT] || editorkeys[SDLK_d])
	{
		if (!editorshowtileselector)
		{
			editor_move_camera_right();
		}
	}


	////////////////////////////////////////////////////////////////////////////
	// handle interation with the gui
	////////////////////////////////////////////////////////////////////////////
	static unsigned int guipanel[] =
	{
		EDITORGUIBUTTONPANELX,
		EDITORGUIBUTTONPANELY,
		EDITORGUIBUTTONPANELX + editormodebuttonpanelsurface->w,
		EDITORGUIBUTTONPANELY + editormodebuttonpanelsurface->h
	};

	if (mousex >= guipanel[0] && mousex <= guipanel[2] && mousey >= guipanel[1] && mousey <= guipanel[3])
	{
		// if we have not clicked already
		if (!editormousebclicked)
		{
			// check for left mouse button being down
			if (editormouseb & SDL_BUTTON(1))
			{
				// its down
				editormousebdown = true;
			}

			// check for left mouse button being up
			if (!(editormouseb & SDL_BUTTON(1)))
			{
				// its up
				if (editormousebdown)
				{
					// we clicked
					editormousebclicked	= true;
					editormousebdown	= false;
				}
			}
		}
		else
		{
			// we have clicked
			editormousebclicked	= false;
			editormousebdown	= false;

			// what mode button did we click?
			static unsigned int button[] =
			{
				4 + EDITORGUIBUTTONPANELX,
				8 + EDITORGUIBUTTONPANELY,
				4 + EDITORGUIBUTTONPANELX + EDITORGUIBUTTONWIDTH,
				8 + EDITORGUIBUTTONPANELY + EDITORGUIBUTTONHEIGHT,

				 4 + EDITORGUIBUTTONPANELX,
				42 + EDITORGUIBUTTONPANELY,
				 4 + EDITORGUIBUTTONPANELX + EDITORGUIBUTTONWIDTH,
				42 + EDITORGUIBUTTONPANELY + EDITORGUIBUTTONHEIGHT,

				 4 + EDITORGUIBUTTONPANELX,
				70 + EDITORGUIBUTTONPANELY,
				 4 + EDITORGUIBUTTONPANELX + EDITORGUIBUTTONWIDTH,
				70 + EDITORGUIBUTTONPANELY + EDITORGUIBUTTONHEIGHT
			};

			bool btn1 = (mousex >= button[0] && mousex <= button[2] && mousey >= button[1] && mousey <= button[3]);
			bool btn2 = (mousex >= button[4] && mousex <= button[6] && mousey >= button[5] && mousey <= button[7]);
			bool btn3 = (mousex >= button[8] && mousex <= button[10] && mousey >= button[9] && mousey <= button[11]);

			if (btn1)
			{
				editor_set_mode(EditorBGEdit);
			} else if (btn2)
			{
				editor_set_mode(EditorFGEdit);
			} else if (btn3)
			{
				editor_set_mode(EditorCDEdit);
			}
		}
	}


	////////////////////////////////////////////////////////////////////////////
	// handle interation with the tile selector panel
	////////////////////////////////////////////////////////////////////////////
	if (editorshowtileselector)
	{
		// show the mouse cursor if its hidden
		if (!SDL_ShowCursor(SDL_QUERY))
		{
			SDL_ShowCursor(SDL_ENABLE);
		}

		// if the right mouse button is down, we exit the selector with the currently selected tile
		if (editormouseb & SDL_BUTTON(3))
		{
			editorcurrentile 		= selectortileselected;
			editorshowtileselector 	= false;
			selectormouseclicks 	= 0;
			editor_update_cursor_surface();
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

}






