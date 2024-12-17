#include "renderer.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "model.hpp"
#include "triangle.hpp"
#include <glm/ext.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <iostream>

Renderer::Renderer(Model *model) : model(model) {
  this->triangles = model->triangles;
  this->view = glm::mat4(1.);
}

void set_pixel(uint32_t *pixels, size_t w, size_t h, size_t x, size_t y,
               uint32_t c) {
  if (y >= h || x >= w) {
    return;
  }
  pixels[y * w + x] = 0xffffffff;
}

void draw_line(uint32_t *pixels, size_t w, size_t h, int x0, int y0, int x1,
               int y1) {
  int dx = abs(x1 - x0);
  int dy = abs(y1 - y0);
  int sx = (x0 < x1) ? 1 : -1;
  int sy = (y0 < y1) ? 1 : -1;
  int err = dx - dy;

  while (true) {
    set_pixel(pixels, w, h, x0, y0, 0xffffffff);

    if (x0 == x1 && y0 == y1)
      break;

    int e2 = 2 * err;
    if (e2 > -dy) {
      err -= dy;
      x0 += sx;
    }
    if (e2 < dx) {
      err += dx;
      y0 += sy;
    }
  }
}

void draw_triangle_wireframe(uint32_t *pixels, size_t w, size_t h,
                             Triangle *t) {
  auto tv0 = t->v0 * 0.5 + glm::vec4(0.5);
  auto tv1 = t->v1 * 0.5 + glm::vec4(0.5);
  auto tv2 = t->v2 * 0.5 + glm::vec4(0.5);

  auto x0 = (size_t)(tv0.x * w);
  auto y0 = (size_t)(tv0.y * h);

  auto x1 = (size_t)(tv1.x * w);
  auto y1 = (size_t)(tv1.y * h);

  auto x2 = (size_t)(tv2.x * w);
  auto y2 = (size_t)(tv2.y * h);

  draw_line(pixels, w, h, x0, y0, x1, y1);
  draw_line(pixels, w, h, x0, y0, x2, y2);
  draw_line(pixels, w, h, x1, y1, x2, y2);
}

void Renderer::render(uint32_t *pixels, size_t w, size_t h) {
  this->projection = glm::perspective(glm::pi<float>() / 4.0f,
                                      (float)w / (float)h, -0.1f, -1000.0f);
  //   this->view = glm::translate(glm::mat4(1.), glm::vec3(0., 0., -10.));
  //   std::cout << glm::to_string(this->view) << std::endl;

  for (auto &tri : this->triangles) {
    // std::cout << &tri << std::endl;
    // model -> world
    // auto t = tri.transformed(model->transform);
    // std::cout << t << std::endl;

    // world -> view
    // t = t.transformed(this->view);
    // std::cout << t << std::endl;

    // view -> clip
    // t = t.transformed(this->projection);
    // std::cout << t << std::endl;
    auto t = tri.transformed(this->projection * this->view * model->transform);

    // perspective division
    auto v0 = t.v0 / -t.v0.w;
    auto v1 = t.v1 / -t.v1.w;
    auto v2 = t.v2 / -t.v2.w;
    t = Triangle(v0, v1, v2);
    // std::cout << t << std::endl;

    draw_triangle_wireframe(pixels, w, h, &t);
  }
  //   exit(0);
}