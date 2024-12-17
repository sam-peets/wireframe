#include "model.hpp"
#include "screen.hpp"
#include <SDL_events.h>
#include <SDL_pixels.h>
#include <SDL_render.h>
#include <SDL_video.h>
#include <iostream>
#include <stdbool.h>
#include <stdio.h>

#include <SDL.h>

int main(int argc, char *argv[]) {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL could not be initialized!\n"
           "SDL_Error: %s\n",
           SDL_GetError());
    exit(1);
  }

  if (argc != 2) {
    std::cout << "usage: Pipeline <in.obj>" << std::endl;
    exit(5);
  }

  Model m = Model::from_obj(std::string(argv[1]));

  Screen s("pipeline", 200, 200, &m);
  s.show();

  SDL_Quit();
  return 0;
}