#ifndef SDLFUNCS_H
#define SDLFUNCS_H

#include "main.h"
#define SDL_MAIN_HANDLED
#include <SDL.h>

// screen dimension constants
//const int SCREEN_WIDTH = 640;
//const int SCREEN_HEIGHT = 480;
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

// SDL functions
void SDLStartUp(SDL_Window *, SDL_Surface *);

#endif
