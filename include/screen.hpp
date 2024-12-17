#pragma once

#include "renderer.hpp"
#include <SDL.h>
struct Screen {
private:
  SDL_Window *window;
  bool quit = false;
  size_t cur_frame_time, last_frame_time;
  Renderer *renderer;

public:
  Screen(const char *title, const size_t width, const size_t height,
         Model *model);
  ~Screen();
  void show();
  void update();
  void render();
};