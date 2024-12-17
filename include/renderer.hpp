#pragma once

#include "model.hpp"
#include "triangle.hpp"
#include <vector>

class Renderer {
private:
  Model *model;
  glm::mat4 projection;
  std::vector<Triangle> triangles;

public:
  glm::mat4 view;
  void render(uint32_t *pixels, size_t w, size_t h);
  Renderer(Model *model);
};