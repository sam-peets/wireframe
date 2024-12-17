#include "screen.hpp"
#include "model.hpp"
#include "renderer.hpp"
#include <glm/ext.hpp>
#include <iostream>
#include <vector>

Screen::Screen(const char *title, const size_t width, const size_t height,
               Model *model) {
  this->window =
      SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                       width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
  if (!window) {
    printf("Window could not be created!\n"
           "SDL_Error: %s\n",
           SDL_GetError());
    exit(2);
  }
  this->renderer = new Renderer(model);
  this->last_frame_time = 0;
  this->cur_frame_time = 0;
  this->last_frame_time_fps = 0;
}

Screen::~Screen() {
  SDL_DestroyWindow(window);
  delete this->renderer;
}

void Screen::show() {
  while (!quit) {
    this->cur_frame_time = SDL_GetTicks();
    size_t d = this->cur_frame_time - last_frame_time;
    if (d < 1000. / 60) {
      continue;
    }
    if (this->cur_frame_time - this->last_frame_time_fps > 1000) {
      std::cout << "fps: " << 1. / (d / 1000.) << std::endl;
      this->last_frame_time_fps = this->cur_frame_time;
    }

    this->update();
    this->render();

    SDL_UpdateWindowSurface(window);
    last_frame_time = cur_frame_time;
  }
}

void Screen::update() {
  this->renderer->view = glm::rotate(
      glm::translate(glm::vec3(0., 0., -10.)),
      (float)glm::radians(this->cur_frame_time / 100.), glm::vec3(0., 1., 0.));
  SDL_Event e;
  if (SDL_PollEvent(&e)) {
    switch (e.type) {
    case SDL_QUIT:
      quit = true;
      break;
    }
  }
}

void Screen::render() {
  SDL_Surface *surface = SDL_GetWindowSurface(window);
  SDL_FillRect(surface, NULL, 0x000000);
  uint32_t *pixels = (uint32_t *)surface->pixels;
  size_t w = surface->w;
  size_t h = surface->h;

  this->renderer->render(pixels, w, h);
}