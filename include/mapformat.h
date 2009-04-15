
#ifndef __MAPFORMAT_H__
#define __MAPFORMAT_H__

struct MapFormat
{
	unsigned int width;
	unsigned int height;
	unsigned int* tilevalue;
};

extern MapFormat* mapformat_a_load_function(const char* filename);
extern void mapformat_a_save_function(MapFormat* data, const char* filename);
extern void mapformat_a_destroy_function(MapFormat* data);

#endif

