#pragma once

#include "model.hpp"
#include "triangle.hpp"
#include <vector>

class Renderer {
private:
  glm::mat4 model;
  glm::mat4 view;
  glm::mat4 projection;
  std::vector<Triangle> triangles;

public:
  void render(uint32_t *pixels, size_t w, size_t h);
  Renderer(Model *model);
};