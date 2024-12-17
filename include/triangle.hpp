#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/ext.hpp"
#include "glm/glm.hpp"
#include <glm/ext/vector_float4.hpp>
#include <ostream>

class Triangle {
public:
  glm::vec4 v0;
  glm::vec4 v1;
  glm::vec4 v2;
  Triangle(glm::vec4 v0, glm::vec4 v1, glm::vec4 v2) : v0(v0), v1(v1), v2(v2) {}
  Triangle transformed(glm::mat4 m);
  glm::vec4 normal();

  friend std::ostream &operator<<(std::ostream &os, const Triangle &t);
};
