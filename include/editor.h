// editor.h
// Project: Basic SDL Map Editor
// Author: Richard Marks

#ifndef __EDITOR_H__
#define __EDITOR_H__

struct MapFormat;
struct SDL_Surface;
struct SDL_Rect;
union SDL_Event;

////////////////////////////////////////////////////////////////////////////////

extern bool editor_init(int argc, char* argv[]);
extern void editor_destroy();
extern void editor_execute();
extern void editor_move_camera_up();
extern void editor_move_camera_down();
extern void editor_move_camera_left();
extern void editor_move_camera_right();
extern void editor_draw_tile(int tileid, int x, int y);
extern void editor_draw_level();
extern void editor_load_level();
extern void editor_save_level();
extern void editor_refresh_view();
extern void editor_handle_input();
extern bool editor_load_startup_config_file();
extern void editor_save_startup_config_file();

////////////////////////////////////////////////////////////////////////////////

extern MapFormat* (*editormaploadfunction)(const char* filename);
extern void (*editormapsavefunction)(MapFormat* data, const char* filename);
extern void (*editormapdestroyfunction)(MapFormat* data);
extern SDL_Surface* editorscreen;
extern SDL_Event* editorevent;
extern SDL_Rect* editortileset;
extern SDL_Rect* editorcamera;
extern MapFormat* editorlevel;
extern int editormousetilex;
extern int editormousetiley;

extern unsigned int editorwindowwidth;
extern unsigned int editorwindowheight;
extern unsigned int editorwindowbpp;

extern int editorcamerawidth;
extern int editorcameraheight;

extern unsigned char* editorkeys;
extern bool editorrunning;
extern unsigned int editorscreenbgcolor;

#endif
