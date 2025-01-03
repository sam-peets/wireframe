#include "model.hpp"
#include <fstream>
#include <glm/ext.hpp>
#include <iostream>

Model Model::from_obj(std::string path) {
  // rudimentary implementation
  // assumes models are triangulated, triangles only contain their 3 points
  // (ie. no normals, etc.)
  std::ifstream infile(path);
  std::vector<glm::vec4> vertices;
  // null entry
  vertices.push_back(glm::vec4(0., 0., 0., 0.));
  std::vector<Triangle> triangles;
  std::string opcode;
  float x, y, z;
  while (infile >> opcode >> x >> y >> z) {
    switch (opcode[0]) {
    case 'v':
      vertices.push_back(glm::vec4(x, y, z, 1.));
      break;
    case 'f':
      Triangle t(vertices[(int)x], vertices[(int)y], vertices[(int)z]);
      triangles.push_back(t);
      break;
    }
  }
  std::cout << "read " << triangles.size() << " triangles, " << vertices.size()
            << " vertices" << std::endl;
  // return Model(triangles, glm::scale(glm::vec3(0.02, 0.02, 0.02)));
  return Model(triangles, glm::scale(glm::vec3(1.)));
}
