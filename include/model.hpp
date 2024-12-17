#pragma once

#include "triangle.hpp"
class Model {
public:
  std::vector<Triangle> triangles;
  glm::mat4 transform;
  Model(std::vector<Triangle> triangles, glm::mat4 transform)
      : triangles(triangles), transform(transform) {};
  static Model from_obj(std::string path);
};