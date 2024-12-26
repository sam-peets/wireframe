#include "renderer.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "model.hpp"
#include "triangle.hpp"
#include <glm/ext.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <iostream>

#include "util.hpp"

Renderer::Renderer(Model *model) : model(model) {
  this->triangles = model->triangles;
  this->view = glm::mat4(1.);
}

bool set_pixel(RenderScreen *screen, size_t x, size_t y, float r, float g,
               float b) {
  if (y >= screen->h || x >= screen->w) {
    return false;
  }

  screen->color[y * screen->w + x] = glm::vec4(r, g, b, 1.);
  return true;
}

void draw_line(RenderScreen *screen, int x0, int y0, int x1, int y1,
               uint32_t c) {
  int dx = abs(x1 - x0);
  int dy = abs(y1 - y0);
  int sx = (x0 < x1) ? 1 : -1;
  int sy = (y0 < y1) ? 1 : -1;
  int err = dx - dy;

  while (set_pixel(screen, x0, y0, 1., 1., 1.)) {
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

// https://www.sunshine2k.de/coding/java/TriangleRasterization/TriangleRasterization.html

void draw_triangle_filled(RenderScreen *screen, Triangle *t) {
  if (t->v0.z > 1 || t->v1.z > 1 || t->v2.z > 1) {
    // TODO -> this isn't a good way of clipping
    // triangles disappear at weird times, do it properly
    return;
  }
  auto tv0 = t->v0 * 0.5 + glm::vec4(0.5);
  auto tv1 = t->v1 * 0.5 + glm::vec4(0.5);
  auto tv2 = t->v2 * 0.5 + glm::vec4(0.5);

  auto x0 = (tv0.x * screen->w);
  auto y0 = (tv0.y * screen->h);

  auto x1 = (tv1.x * screen->w);
  auto y1 = (tv1.y * screen->h);

  auto x2 = (tv2.x * screen->w);
  auto y2 = (tv2.y * screen->h);
  return;
}

void draw_triangle_wireframe(RenderScreen *screen, Triangle *t) {
  if (t->v0.z > 1 || t->v1.z > 1 || t->v2.z > 1) {
    // TODO -> this isn't a good way of clipping
    // triangles disappear at weird times, do it properly
    return;
  }
  auto tv0 = t->v0 * 0.5 + glm::vec4(0.5);
  auto tv1 = t->v1 * 0.5 + glm::vec4(0.5);
  auto tv2 = t->v2 * 0.5 + glm::vec4(0.5);

  auto x0 = (tv0.x * screen->w);
  auto y0 = (tv0.y * screen->h);

  auto x1 = (tv1.x * screen->w);
  auto y1 = (tv1.y * screen->h);

  auto x2 = (tv2.x * screen->w);
  auto y2 = (tv2.y * screen->h);

  uint32_t c = 0xffffffff;
  draw_line(screen, x0, y0, x1, y1, c);
  draw_line(screen, x0, y0, x2, y2, c);
  draw_line(screen, x1, y1, x2, y2, c);
}

void Renderer::render(RenderScreen *screen) {
  this->projection =
      glm::perspective(glm::pi<float>() / 4.0f,
                       (float)screen->w / (float)screen->h, -0.1f, -1000.0f);
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

    draw_triangle_wireframe(screen, &t);
  }
  //   exit(0);
}