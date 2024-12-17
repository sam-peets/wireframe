#include "triangle.hpp"
#include <glm/glm.hpp>

glm::vec4 Triangle::normal() {
  auto v01 = (v1 - v0).xyz();
  auto v02 = (v2 - v0).xyz();

  return glm::vec4(glm::cross(v01, v02), 0.);
}

Triangle Triangle::transformed(glm::mat4 m) {
  auto nv0 = m * this->v0;
  auto nv1 = m * this->v1;
  auto nv2 = m * this->v2;
  return Triangle(nv0, nv1, nv2);
}

std::ostream &operator<<(std::ostream &os, const Triangle &t) {
  os << "Triangle(" << glm::to_string(t.v0) << ", " << glm::to_string(t.v1)
     << ", " << glm::to_string(t.v2) << ")";
  return os;
}
