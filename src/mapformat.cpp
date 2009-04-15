// mapformat.cpp
// Project: Basic SDL Map Editor ()
// Author: Richard Marks

// C STANDARD LIBRARY
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <ctime>
#include <cctype>
#include <cstdarg>

// EDITOR
#include "mapformat.h"

////////////////////////////////////////////////////////////////////////////////

MapFormat* mapformat_a_create_function(unsigned int width, unsigned int height)
{
	MapFormat* data = new MapFormat;

	if (!data)
	{
		#if defined(DEBUG)
		fprintf(stderr, "Unable to allocate new MapFormat memory chunk!\n");
		#endif
		return 0;
	}

	data->width = width;
	data->height = height;

	unsigned int bytes = data->width * data->height;
	data->tilevalue = new unsigned int [bytes];
	if (!data->tilevalue)
	{
		#if defined(DEBUG)
		fprintf(stderr, "Unable to allocate new unsigned int memory chunk of %d element!\n", bytes);
		#endif
		return 0;
	}

	for (unsigned int index = 0; index < bytes; index++)
	{
		data->tilevalue[index] = 0;
	}

	return data;
}

////////////////////////////////////////////////////////////////////////////////

MapFormat* mapformat_a_load_function(const char* filename)
{
	MapFormat* data = new MapFormat;

	if (!data)
	{
		#if defined(DEBUG)
		fprintf(stderr, "Unable to allocate new MapFormat memory chunk!\nLoading \"%s\" has failed!\n", filename);
		#endif
		return 0;
	}

	FILE* fp = fopen(filename, "rb");

	if (!fp)
	{
		#if defined(DEBUG)
		fprintf(stderr, "Unable to open \"%s\" for reading!\n", filename);
		#endif
		return 0;
	}

	unsigned int header[6];
	fread(header, sizeof(header), 1, fp);

	if (!(
		header[0] == 'M' &&
		header[1] == 'A' &&
		header[2] == 'P' &&
		header[3] == 'A'))
	{
		#if defined(DEBUG)
		fprintf(stderr, "The file \"%s\" contains an invalid header!\nExpected MAPA file format!\n", filename);
		#endif
		return 0;
	}

	data->width = header[4];
	data->height = header[5];
	#if defined(DEBUG)
	fprintf(stderr, "Loading \"%s\" whose header says its %d x %d tiles in size.\n", filename, data->width, data->height);
	#endif

	unsigned int bytes = data->width * data->height;
	data->tilevalue = new unsigned int [bytes];
	if (!data->tilevalue)
	{
		#if defined(DEBUG)
		fprintf(stderr, "Unable to allocate new unsigned int memory chunk of %d element!\nLoading \"%s\" has failed!\n", bytes, filename);
		#endif
		return 0;
	}

	for (unsigned int index = 0; index < bytes; index++)
	{
		fread(&data->tilevalue[index], sizeof(unsigned int), 1, fp);
	}

	fclose(fp);
	#if defined(DEBUG)
	fprintf(stderr, "Loading \"%s\" complete.\n", filename);
	#endif
	return data;
}

////////////////////////////////////////////////////////////////////////////////

void mapformat_a_save_function(MapFormat* data, const char* filename)
{
	if (!data)
	{
		#if defined(DEBUG)
		fprintf(stderr, "Unable to save null map data to \"%s\"!\nError: MapFormat* data is NULL!\n", filename);
		#endif
		return;
	}

	FILE* fp = fopen(filename, "wb");

	if (!fp)
	{
		#if defined(DEBUG)
		fprintf(stderr, "Unable to open \"%s\" for writing!\n", filename);
		#endif
		return;
	}

	unsigned int header[] =
	{
		'M','A','P','A',
		data->width,
		data->height
	};

	fwrite(header, sizeof(header), 1, fp);

	unsigned int bytes = data->width * data->height;

	for (unsigned int index = 0; index < bytes; index++)
	{
		fwrite(&data->tilevalue[index], sizeof(unsigned int), 1, fp);
	}

	fclose(fp);
}

////////////////////////////////////////////////////////////////////////////////

void mapformat_a_destroy_function(MapFormat* data)
{
	if (data)
	{
		if (data->tilevalue)
		{
			delete [] data->tilevalue;
		}
		delete data;
		data = 0;
	}
}



