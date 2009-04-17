// selector.h
// Project: Basic SDL Map Editor
// Author: Richard Marks

#ifndef __SELECTOR_H__
#define __SELECTOR_H__

struct SDL_Surface;
struct SDL_Rect;

extern bool selector_init(SDL_Surface* tileset);
extern void selector_destroy();
extern void selector_scroll_up();
extern void selector_scroll_down();
extern void selector_refresh_view();

extern void selector_create_tile_selection_panel();
extern void selector_redraw_tile_selection_panel();

extern bool selectorlmbdown;
extern bool selectorlmbclicked;
extern unsigned int selectormouseclicks;
extern unsigned int selectortileundermouse;
extern unsigned int selectortileselected;
extern unsigned int selectortilecount;
extern unsigned int selectortilesacross;
extern unsigned int selectortilesdown;
extern unsigned int selectorscroll;
extern unsigned int selectorpanelwidth;
extern unsigned int selectorpanelheight;
extern unsigned int selectormousetilex;
extern unsigned int selectormousetiley;
extern int selectormousex;
extern int selectormousey;
extern SDL_Surface* selectortileselectionpanel;
extern SDL_Surface* selectorsharedtileset;
extern SDL_Surface* selectorhelptextsurface;

extern SDL_Rect* selectorpanelview;

#endif
