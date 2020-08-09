#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SDL_MAIN_HANDLED
#include <SDL.h>


// screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;


int main(int argc, char *args[]) {

  // window to render to
  SDL_Window *window = NULL;

  // surface contained by window
  SDL_Surface *screen_surface = NULL;

  // init SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("Error: SDL could not initialise. SDL_Error: %s\n", SDL_GetError());
  }
  else {
    // create window
    window = SDL_CreateWindow("SDL Testing", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
      printf("Error: Window could not be created. SDL_Error: %s\n", SDL_GetError());
    }
    else {
      // get window surface
      screen_surface = SDL_GetWindowSurface(window);

      // fill the window white
      SDL_FillRect(screen_surface, NULL, SDL_MapRGB(screen_surface->format, 0xFF, 0xFF, 0xFF));

      // update the surface
      SDL_UpdateWindowSurface(window);

      // wait two seconds
      SDL_Delay(2000);
    }
  }

  // destroy window
  SDL_DestroyWindow(window);

  // quit SDL subsystems
  SDL_Quit();

  return 0;
}
