#pragma once

#include "model.hpp"
#include "triangle.hpp"
#include <iostream>
#include <vector>
class RenderScreen {
public:
  glm::vec4 *color;
  glm::vec4 *normal;
  glm::vec4 *depth;
  size_t w;
  size_t h;
  RenderScreen(size_t w, size_t h) : w(w), h(h) {
    color = new glm::vec4[w * h];
    normal = new glm::vec4[w * h];
    depth = new glm::vec4[w * h];
    for (size_t i = 0; i < w * h; i++) {
      color[i] = glm::vec4(0.);
      normal[i] = glm::vec4(0.);
      depth[i] = glm::vec4(0.);
    }
  }
  ~RenderScreen() {
    delete color;
    delete normal;
    delete depth;
  }
};
class Renderer {
private:
  Model *model;
  glm::mat4 projection;
  std::vector<Triangle> triangles;

public:
  glm::mat4 view;
  void render(RenderScreen *render_screen);
  Renderer(Model *model);
};
