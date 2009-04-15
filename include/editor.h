// editor.h
// Project: Basic SDL Map Editor
// Author: Richard Marks

#ifndef __EDITOR_H__
#define __EDITOR_H__

struct MapFormat;

union SDL_Event;
struct SDL_Surface;
struct SDL_Rect;
struct _TTF_Font;


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

extern void editor_printf(int x, int y, unsigned int color, const char* format, ...);

////////////////////////////////////////////////////////////////////////////////

extern _TTF_Font* editorfont;

extern MapFormat* (*editormapcreatefunction)(unsigned int width, unsigned int height);
extern MapFormat* (*editormaploadfunction)(const char* filename);
extern void (*editormapsavefunction)(MapFormat* data, const char* filename);
extern void (*editormapdestroyfunction)(MapFormat* data);
extern SDL_Surface* editorscreen;
extern SDL_Event* editorevent;
extern SDL_Surface* editortileset;
extern SDL_Rect* editorcamera;
extern MapFormat* editorlevel;
extern unsigned int editorcurrentile;
extern unsigned int editormousetilex;
extern unsigned int editormousetiley;
extern unsigned int editorwindowwidth;
extern unsigned int editorwindowheight;
extern unsigned int editorwindowbpp;
extern unsigned int editorcamerawidth;
extern unsigned int editorcameraheight;
extern unsigned int editorlevelwidth;
extern unsigned int editorlevelheight;
extern unsigned int editorscreenbgcolor;
extern unsigned int editortextcolor;
extern unsigned int editortilewidth;
extern unsigned int editortileheight;
extern unsigned char* editorkeys;
extern bool editorrunning;


extern int editornumkeys;
extern bool* editorkeywaspressed;

#endif
