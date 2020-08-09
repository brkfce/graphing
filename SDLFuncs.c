#include "SDLFuncs.h"

// create SDL window and surface
void SDLStartUp(SDL_Window *window, SDL_Surface *surface) {
  // init SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("Error: SDL could not initialise. SDL_Error: %s\n", SDL_GetError());
    exit(-1);
  }
  else {
    // create window
    window = SDL_CreateWindow("SDL Testing", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
      printf("Error: Window could not be created. SDL_Error: %s\n", SDL_GetError());
      exit(-1);
    }
    else {
      printf("SDL Window and Surface created successfully.\n");
    }
  }
}
