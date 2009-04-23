
#ifndef __MAPFORMAT_H__
#define __MAPFORMAT_H__

struct MapDataFormat
{
	unsigned int backtilevalue;
	unsigned int foretilevalue;
	bool solid;
};

struct MapFormat
{
	unsigned int width;
	unsigned int height;

	MapDataFormat* data;
};

extern MapDataFormat* mapdataformat_b_create_function(unsigned int width, unsigned int height);
extern void mapdataformat_b_destroy_function(MapDataFormat* data);

extern MapFormat* mapformat_b_create_function(unsigned int width, unsigned int height);
extern MapFormat* mapformat_b_load_function(const char* filename);
extern void mapformat_b_save_function(MapFormat* data, const char* filename);
extern void mapformat_b_destroy_function(MapFormat* data);


// old MAPA format
#if 0
struct MapFormat
{
	unsigned int width;
	unsigned int height;
	unsigned int* tilevalue;
};

extern MapFormat* mapformat_a_create_function(unsigned int width, unsigned int height);
extern MapFormat* mapformat_a_load_function(const char* filename);
extern void mapformat_a_save_function(MapFormat* data, const char* filename);
extern void mapformat_a_destroy_function(MapFormat* data);

#endif

#endif

