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

bool set_pixel(uint32_t *pixels, size_t w, size_t h, size_t x, size_t y,
               uint32_t c) {
  if (y >= h || x >= w) {
    return false;
  }
  pixels[y * w + x] = 0xffffffff;
  return true;
}

void draw_line(uint32_t *pixels, size_t w, size_t h, int x0, int y0, int x1,
               int y1) {
  int dx = abs(x1 - x0);
  int dy = abs(y1 - y0);
  int sx = (x0 < x1) ? 1 : -1;
  int sy = (y0 < y1) ? 1 : -1;
  int err = dx - dy;

  while (set_pixel(pixels, w, h, x0, y0, 0xffffffff)) {
    // stop trying to draw a line if it goes offscreen

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

  if (t->v0.z > 1 || t->v1.z > 1 || t->v2.z > 1) {
    // TODO -> this isn't a good way of clipping
    // triangles disappear at weird times, do it properly
    return;
  }
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
  auto mvp = this->projection * this->view * model->transform;
  for (auto &tri : this->triangles) {
    auto t = tri.transformed(mvp);

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