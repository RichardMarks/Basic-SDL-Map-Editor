// globals.cpp
// Project: Basic SDL Map Editor
// Author: Richard Marks

#include "common_includes.h"

////////////////////////////////////////////////////////////////////////////////
// function pointers for the modes of execution
////////////////////////////////////////////////////////////////////////////////
void (*editorrefreshviewfunction)() = 0;
void (*editorhandleinputfunction)() = 0;
void (*editorsettilefunction)()		= 0;
void (*editorunsettilefunction)()	= 0;

////////////////////////////////////////////////////////////////////////////////
// function pointers for handling the map format
////////////////////////////////////////////////////////////////////////////////
MapFormat* (*editormapcreatefunction)(unsigned int width, unsigned int height)	= 0;
MapFormat* (*editormaploadfunction)(const char* filename) 						= 0;
void (*editormapsavefunction)(MapFormat* data, const char* filename)			= 0;
void (*editormapdestroyfunction)(MapFormat* data)								= 0;

////////////////////////////////////////////////////////////////////////////////
// SDL screen and event pointers
////////////////////////////////////////////////////////////////////////////////
SDL_Surface* editorscreen	= 0;
SDL_Event* editorevent		= 0;

////////////////////////////////////////////////////////////////////////////////
// camera system
////////////////////////////////////////////////////////////////////////////////
EditorCamera* editorcamera			= 0;

unsigned int editorcamerawidth		= 30;
unsigned int editorcameraheight		= 22;

////////////////////////////////////////////////////////////////////////////////
// level data
////////////////////////////////////////////////////////////////////////////////
MapFormat* editorlevel				= 0;

//unsigned int editorlevelwidth		= 130; // 4 full "screens" of tiles plus a 2 tile border
//unsigned int editorlevelheight		= 98;

unsigned int editorlevelwidth		= 122; // 4 full "screens" of tiles plus a 2 tile border
unsigned int editorlevelheight		= 90;


unsigned int editortilewidth		= 32;
unsigned int editortileheight		= 32;

// used in mouse location coordiante checking to get a boost of speed
unsigned int editorcameraxlimit		= editortilewidth * (editorcamerawidth + 1);
unsigned int editorcameraylimit		= editortileheight * (editorcameraheight + 1);


unsigned int editortilewidthx2		= editortilewidth * 2;
unsigned int editortileheightx2		= editortileheight * 2;
unsigned int editortilewidthover2	= editortilewidth / 2;
unsigned int editortileheightover2	= editortileheight / 2;

unsigned int editortilewidthx22		= editortilewidth * 22;
unsigned int editortileheightx14	= editortileheight * 14;

////////////////////////////////////////////////////////////////////////////////
// editor
////////////////////////////////////////////////////////////////////////////////
SDL_Surface* editortileset						= 0;
SDL_Surface* editormodebuttonpanelsurface		= 0;
SDL_Surface* editorbglayermodebuttonsurface		= 0;
SDL_Surface* editorfglayermodebuttonsurface		= 0;
SDL_Surface* editorcdlayermodebuttonsurface		= 0;
SDL_Surface* editorshowselectorbuttonsurface	= 0;
SDL_Surface* editorsavebmpbuttonsurface			= 0;
SDL_Surface* editorcursorsurface				= 0;


SDL_Surface* editorbgmapsurface					= 0;
SDL_Surface* editorfgmapsurface					= 0;
SDL_Surface* editorcdmapsurface					= 0;


SDL_Surface* editorhelptextsurface				= 0;
SDL_Surface* editorcollisionoverlaysurface		= 0;

SDL_Rect* editortilecoordinates					= 0;

TTF_Font* editorfont							= 0;

unsigned int editorwindowwidth					= 1024;
unsigned int editorwindowheight					= 768;
unsigned int editorwindowbpp					= 24;
unsigned int editormousetilex					= 0;
unsigned int editormousetiley					= 0;
unsigned int editorscreenbgcolor				= 0;
unsigned int editortextcolor					= 0;
unsigned int editorcurrentile					= 0;
unsigned int editorhelptextrenderx				= 0;
unsigned int editorhelptextrendery				= 0;
unsigned int editormouseb						= 0;

int editornumkeys								= 0;
int editormousex								= 0;
int editormousey								= 0;

bool editorrunning								= false;
bool editorshowtileselector						= false;
bool editormousebclicked						= false;
bool editormousebdown							= false;

bool* editorkeywaspressed						= 0;
unsigned char* editorkeys						= 0;

EditorMode editormode							= EditorBGEdit;

////////////////////////////////////////////////////////////////////////////////
// selector
////////////////////////////////////////////////////////////////////////////////

SDL_Rect* selectorpanelview							= 0;

SDL_Surface* selectortileselectionpanel				= 0;
SDL_Surface* selectorsharedtileset					= 0;
SDL_Surface* selectorhelptextsurface				= 0;

unsigned int selectormouseclicks					= 0;
unsigned int selectortileundermouse					= 0;
unsigned int selectortileselected					= 0;
unsigned int selectortilecount						= 0;
unsigned int selectortilesacross					= 0;
unsigned int selectortilesdown						= 0;
unsigned int selectorscroll							= 0;
unsigned int selectorpanelwidth						= 0;
unsigned int selectorpanelheight					= 0;
unsigned int selectormousetilex						= 0;
unsigned int selectormousetiley						= 0;

bool selectorlmbdown								= false;
bool selectorlmbclicked								= false;

int selectormousex									= 0;
int selectormousey									= 0;






