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

	editor_handle_keyboard_input_common();

	unsigned int mousex = (unsigned int) editormousex;
	unsigned int mousey = (unsigned int) editormousey;

	////////////////////////////////////////////////////////////////////////////
	// v0.0.5 mouse camera panning controls
	////////////////////////////////////////////////////////////////////////////
	#if 1
	if (editorkeys[SDLK_LCTRL] || editorkeys[SDLK_RCTRL])
	{
		static unsigned int zone[] =
		{
			// top
			editortilewidth, 0, editortilewidth + editorcamera->inpixels->w, editortileheight,

			// bottom
			editortilewidth, editortileheight + editorcamera->inpixels->h, editortilewidth + editorcamera->inpixels->w, editorscreen->h,

			// left
			0, editortileheight, editortilewidth, editorscreen->h - editortileheight,

			// right
			editortilewidth + editorcamera->inpixels->w, editortileheight, editorscreen->w, editorscreen->h - editortileheight
		};

		bool btn1 = (mousex >=  zone[0] && mousex <= zone[2] && mousey >= zone[1] && mousey <= zone[3]);
		bool btn2 = (mousex >=  zone[4] && mousex <= zone[6] && mousey >= zone[5] && mousey <= zone[7]);
		bool btn3 = (mousex >=  zone[8] && mousex <= zone[10] && mousey >= zone[9] && mousey <= zone[11]);
		bool btn4 = (mousex >= zone[12] && mousex <= zone[14] && mousey >= zone[13] && mousey <= zone[15]);

		if (btn1)
		{
			// pan up
			editor_move_camera_up();
		} else if (btn2)
		{
			// pan down
			editor_move_camera_down();
		} else if (btn3)
		{
			// pan left
			editor_move_camera_left();
		} else if (btn4)
		{
			// pan right
			editor_move_camera_right();
		}
	}
	#endif

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
				  6 + EDITORGUIBUTTONPANELY,
				  4 + EDITORGUIBUTTONPANELX + EDITORGUIBUTTONWIDTH,
				  6 + EDITORGUIBUTTONPANELY + EDITORGUIBUTTONHEIGHT,

				  4 + EDITORGUIBUTTONPANELX,
				 38 + EDITORGUIBUTTONPANELY,
				  4 + EDITORGUIBUTTONPANELX + EDITORGUIBUTTONWIDTH,
				 38 + EDITORGUIBUTTONPANELY + EDITORGUIBUTTONHEIGHT,

				  4 + EDITORGUIBUTTONPANELX,
				 70 + EDITORGUIBUTTONPANELY,
				  4 + EDITORGUIBUTTONPANELX + EDITORGUIBUTTONWIDTH,
				 70 + EDITORGUIBUTTONPANELY + EDITORGUIBUTTONHEIGHT,

				  4 + EDITORGUIBUTTONPANELX,
				102 + EDITORGUIBUTTONPANELY,
				  4 + EDITORGUIBUTTONPANELX + EDITORGUIBUTTONWIDTH,
				102 + EDITORGUIBUTTONPANELY + EDITORGUIBUTTONHEIGHT,

				  4 + EDITORGUIBUTTONPANELX,
				134 + EDITORGUIBUTTONPANELY,
				  4 + EDITORGUIBUTTONPANELX + EDITORGUIBUTTONWIDTH,
				134 + EDITORGUIBUTTONPANELY + EDITORGUIBUTTONHEIGHT
			};

			bool btn1 = (mousex >=  button[0] && mousex <= button[2] && mousey >= button[1] && mousey <= button[3]);
			bool btn2 = (mousex >=  button[4] && mousex <= button[6] && mousey >= button[5] && mousey <= button[7]);
			bool btn3 = (mousex >=  button[8] && mousex <= button[10] && mousey >= button[9] && mousey <= button[11]);
			bool btn4 = (mousex >= button[12] && mousex <= button[14] && mousey >= button[13] && mousey <= button[15]);
			bool btn5 = (mousex >= button[16] && mousex <= button[18] && mousey >= button[17] && mousey <= button[19]);

			if (btn1)
			{
				editor_set_mode(EditorBGEdit);
				editor_create_scene();
			} else if (btn2)
			{
				editor_set_mode(EditorFGEdit);
				editor_create_scene();
			} else if (btn3)
			{
				editor_set_mode(EditorCDEdit);
				editor_create_scene();
			} else if (btn4)
			{
				editorshowtileselector = !editorshowtileselector;
				selector_redraw_tile_selection_panel();
			}
			else if (btn5)
			{
				// save snapshot of the map
				if (!SDL_SaveBMP(editorbgmapsurface, "wip.bmp"))
				{
					fprintf(stderr, "A snapshot of the current map was taken and saved to wip.bmp\n");
				}
				else
				{
					fprintf(stderr, "Unable to save a snapshot of the map to wip.bmp!\nSDL Error: %s\n", SDL_GetError());
				}
			}
		}
	}
}






