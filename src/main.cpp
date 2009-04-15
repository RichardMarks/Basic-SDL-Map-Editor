// main.cpp
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

// STL
#include <vector>
#include <string>
#include <map>
#include <algorithm>

// SDL
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

// EDITOR
#include "editor.h"

////////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
	if (!editor_init(argc, argv))
	{
		fprintf(stderr, "Basic SDL Map Editor Initialization Failed!\n");
	}

	atexit(SDL_Quit);
	atexit(editor_destroy);

	editor_execute();

	return 0;
}
