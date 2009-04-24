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

const int EDITORGUIBUTTONPANELX = 1;
const int EDITORGUIBUTTONPANELY = 30;
const int EDITORGUIBUTTONWIDTH	= 20;
const int EDITORGUIBUTTONHEIGHT = 20;

////////////////////////////////////////////////////////////////////////////////

enum EditorMode
{
	EditorBGEdit,
	EditorFGEdit,
	EditorCDEdit
};

////////////////////////////////////////////////////////////////////////////////

struct EditorCamera
{
	SDL_Rect* intiles;
	SDL_Rect* inpixels;
	SDL_Rect* box;
};

////////////////////////////////////////////////////////////////////////////////

extern void (*editorrefreshviewfunction)();
extern void (*editorhandleinputfunction)();

extern void (*editorsettilefunction)();
extern void (*editorunsettilefunction)();


extern _TTF_Font* editorfont;

extern MapFormat* (*editormapcreatefunction)(unsigned int width, unsigned int height);
extern MapFormat* (*editormaploadfunction)(const char* filename);
extern void (*editormapsavefunction)(MapFormat* data, const char* filename);
extern void (*editormapdestroyfunction)(MapFormat* data);

extern SDL_Event* editorevent;

extern SDL_Surface* editorscreen;
extern SDL_Surface* editortileset;

extern SDL_Surface* editorbgmapsurface;
extern SDL_Surface* editorfgmapsurface;
extern SDL_Surface* editorcdmapsurface;

extern SDL_Surface* editorhelptextsurface;
extern SDL_Surface* editormodebuttonpanelsurface;
extern SDL_Surface* editorbglayermodebuttonsurface;
extern SDL_Surface* editorfglayermodebuttonsurface;
extern SDL_Surface* editorcdlayermodebuttonsurface;

extern SDL_Surface* editorshowselectorbuttonsurface;
extern SDL_Surface* editorsavebmpbuttonsurface;

extern SDL_Surface* editorcursorsurface;
extern SDL_Surface* editorcollisionoverlaysurface;

extern SDL_Rect* editortilecoordinates;

extern EditorCamera* editorcamera;

extern MapFormat* editorlevel;

extern unsigned int editorhelptextrenderx;
extern unsigned int editorhelptextrendery;
extern unsigned int editorcurrentile;
extern unsigned int editormousetilex;
extern unsigned int editormousetiley;
extern unsigned int editorwindowwidth;
extern unsigned int editorwindowheight;
extern unsigned int editorwindowbpp;

extern unsigned int editorcamerawidth;
extern unsigned int editorcameraheight;
extern unsigned int editorcameraxlimit;
extern unsigned int editorcameraylimit;

extern unsigned int editorlevelwidth;
extern unsigned int editorlevelheight;
extern unsigned int editorscreenbgcolor;
extern unsigned int editortextcolor;
extern unsigned int editortilewidth;
extern unsigned int editortileheight;

extern unsigned int editortilewidthx2;
extern unsigned int editortileheightx2;
extern unsigned int editortilewidthover2;
extern unsigned int editortileheightover2;

// lame hack to boost a tiny bit of speed
extern unsigned int editortilewidthx22;
extern unsigned int editortileheightx14;

extern unsigned int editormouseb;

extern unsigned char* editorkeys;

extern int editornumkeys;
extern int editormousex;
extern int editormousey;

extern bool* editorkeywaspressed;

extern bool editorshowtileselector;
extern bool editorrunning;

extern bool editormousebclicked;
extern bool editormousebdown;

extern EditorMode editormode;

////////////////////////////////////////////////////////////////////////////////


extern bool editor_init(int argc, char* argv[]);
extern void editor_destroy();

extern void editor_execute();

extern void editor_move_camera_up();
extern void editor_move_camera_down();
extern void editor_move_camera_left();
extern void editor_move_camera_right();

extern void editor_create_camera();
extern void editor_destroy_camera();

extern void editor_load_level();
extern void editor_save_level();

extern bool editor_load_startup_config_file();
extern void editor_save_startup_config_file();

extern void editor_printf(int x, int y, unsigned int color, const char* format, ...);

extern void editor_create_map_surfaces();

extern void editor_create_scene();

extern void editor_create_static_help_text();

extern void editor_create_gui_elements();
extern void editor_destroy_gui_elements();


extern void editor_create_cursor_surface();
extern void editor_update_cursor_surface();
extern void editor_destroy_cursor_surface();

extern void editor_select_previous_tile();
extern void editor_select_next_tile();



extern void editor_handle_input_bgeditor();
extern void editor_handle_input_fgeditor();
extern void editor_handle_input_cdeditor();

extern void editor_refresh_view_bgeditor();
extern void editor_refresh_view_fgeditor();
extern void editor_refresh_view_cdeditor();

extern void editor_set_tile_bgeditor();
extern void editor_set_tile_fgeditor();
extern void editor_set_tile_cdeditor();

extern void editor_unset_tile_bgeditor();
extern void editor_unset_tile_fgeditor();
extern void editor_unset_tile_cdeditor();

extern void editor_set_mode(EditorMode mode);

extern void editor_refresh_view_common();
extern void editor_handle_input_common();


#endif
