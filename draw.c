// Created by saad on 8/4/26.
//

#include "draw.h"
#include "constant.h"
#include "mesh.h"
#include "texture.h"
#include "vec.h"
#include <SDL3/SDL_haptic.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>

bool is_back_face(vec3 v1, vec3 v2, vec3 v3);

uint32_t *create_framebuff() {
  uint32_t *framebuff = (uint32_t *)malloc(WIDTH * HIEGHT * sizeof(uint32_t));
  return framebuff;
}

// function will put a single pixel with specified color in the
// framebuffer
void put_pixel(uint32_t *framebuffer, int x, int y, uint32_t color) {
  if (x >= WIDTH || x <= 0 || y >= HIEGHT || y <= 0) {
    return;
  }

  framebuffer[y * WIDTH + x] = color;
}

// clears the framebuffer with given color
void clear_framebuffer(uint32_t *framebuffer, uint32_t color) {
  for (int r = 0; r < HIEGHT; ++r) {
    for (int c = 0; c < WIDTH; ++c) {
      framebuffer[r * WIDTH + c] = color;
    }
  }
}

void draw_line(uint32_t *framebuffer, float x1, float y1, float x2, float y2,
               uint32_t color) {
  float xdiff = x2 - x1;
  float ydiff = y2 - y1;

  if (xdiff == 0.0f && ydiff == 0.0f) {
    put_pixel(framebuffer, x1, y1, color);
    return;
  }

  //
  if (fabsf(xdiff) > fabsf(ydiff)) {
    float xmin;
    float xmax;
    if (x2 > x1) {
      xmax = x2;
      xmin = x1;
    } else {
      xmin = x2;
      xmax = x1;
    }

    float slop = ydiff / xdiff;
    for (float x = xmin; x <= xmax; x += 1.0f) {
      // calculate how much we need to increase y
      float y = y1 + (x - x1) * slop;
      put_pixel(framebuffer, x, y, color);
    }
  } else {
    float ymin;
    float ymax;
    if (y2 > y1) {
      ymax = y2;
      ymin = y1;
    } else {
      ymax = y1;
      ymin = y2;
    }

    float slop = xdiff / ydiff;
    for (float y = ymin; y <= ymax; y += 1.0f) {
      float x = x1 + (y - y1) * slop;
      put_pixel(framebuffer, x, y, color);
    }
  }
}

void draw_triangle(float x1, float y1, float x2, float y2, float x3, float y3,
                   uint32_t *framebuffer, uint32_t color) {

  draw_line(framebuffer, x1, y1, x2, y2, color);
  draw_line(framebuffer, x2, y2, x3, y3, color);
  draw_line(framebuffer, x3, y3, x1, y1, color);
}

void swap(float *a, float *b) {
  float temp = *a;
  *a = *b;
  *b = temp;
}

// void draw_wireframe(mesh *mesh, uint32_t color, matrix proj_matrix,
//                     uint32_t *framebuffer) {
//   for (int i = 0; i < mesh->num_face_t; ++i) {
//     vec3 v1 = mesh->vertices[mesh->faces[i].vertex_indices[0]];
//     vec3 v2 = mesh->vertices[mesh->faces[i].vertex_indices[1]];
//     vec3 v3 = mesh->vertices[mesh->faces[i].vertex_indices[2]];
//
//     vec3 p1 = project_to_screen(v1, proj_matrix);
//     vec3 p2 = project_to_screen(v2, proj_matrix);
//     vec3 p3 = project_to_screen(v3, proj_matrix);
//
//     // we should check here for back face culling
//     if (is_back_face(v1, v2, v3)) {
//       continue;
//     }
//     draw_line(framebuffer, p1.x, p1.y, p2.x, p2.y, color);
//     draw_line(framebuffer, p2.x, p2.y, p3.x, p3.y, color);
//     draw_line(framebuffer, p3.x, p3.y, p1.x, p1.y, color);
//   }
// }

bool is_back_face(vec3 v1, vec3 v2, vec3 v3) {
  vec3 edge_1 = v3_sub(v2, v1);
  vec3 edge_2 = v3_sub(v3, v1);

  vec3 cross = v3_cross(edge_1, edge_2);
  vec3 cross_norm = v3_normalize(cross);
  vec3 to_camera = v3_normalize(v1);

  float dot = v3_dot(cross_norm, to_camera);

  return dot >= 0.0;
}

float edge_function(vec3 a, vec3 b, vec3 p) {
  return (p.x - a.x) * (b.y - a.y) - (p.y - a.y) * (b.x - a.x);
}

// pass three color here
// and calculate by bc cordinate the color of triangles

/// for testing the fill vertices we need to pass the
/// projection matrix here with project to screen do this
///
void fill_triangle(vec3 p1, vec3 p2, vec3 p3, uint32_t *framebuffer,
                   float *zbuffer, matrix proj_matrix, uint32_t color1,
                   uint32_t color2, uint32_t color3) {
  screen_space_vertex a = project_to_screen(p1, proj_matrix);
  screen_space_vertex b = project_to_screen(p2, proj_matrix);
  screen_space_vertex c = project_to_screen(p3, proj_matrix);
  // float to interger conversion

  // because we are returning now the screen_space structer
  // so we need to acccess points like this
  int minx = fminf(a.vertex.x, fminf(b.vertex.x, c.vertex.x));
  int maxx = fmaxf(a.vertex.x, fmaxf(b.vertex.x, c.vertex.x));

  int miny = fminf(a.vertex.y, fminf(b.vertex.y, c.vertex.y));
  int maxy = fmaxf(a.vertex.y, fmaxf(b.vertex.y, c.vertex.y));

  float bt_area = edge_function(a.vertex, b.vertex, c.vertex);
  // edge function formula
  // w0 * c0[0] + w1 * c1[0] + w2 * c2[0]

  for (int i = miny; i < maxy; i++) {
    for (int j = minx; j < maxx; j++) {
      vec3 point = (vec3){j + 0.5, i + 0.5, 0.0};
      float w0 = edge_function(a.vertex, b.vertex, point);
      float w1 = edge_function(b.vertex, c.vertex, point);
      float w2 = edge_function(c.vertex, a.vertex, point);

      float c0r = (color1 >> 16) & 0xFF;
      float c0g = (color1 >> 8) & 0xFF;
      float c0b = (color1) & 0xFF;

      float c1r = (color2 >> 16) & 0xFF;
      float c1g = (color2 >> 8) & 0xFF;
      float c1b = (color2) & 0xFF;

      float c2r = (color3 >> 16) & 0xFF;
      float c2g = (color3 >> 8) & 0xFF;
      float c2b = (color3) & 0xFF;

      if ((w0 >= 0 && w1 >= 0 && w2 >= 0) || (w0 <= 0 && w1 <= 0 && w2 <= 0)) {
        w0 /= bt_area;
        w1 /= bt_area;
        w2 /= bt_area;
        // NOTE this is normal barycentric cordinate
        //  this will not work after prespective divide
        //  this wiegths will not work
        // float red = w0 * c0r + w1 * c1r + w2 * c2r;
        // float green = w0 * c0g + w1 * c1g + w2 * c2g;
        // float blue = w0 * c0b + w1 * c1b + w2 * c2b;

        // these are the steps for completing this todo
        // 1. we need to find the inverse w of this pixel by wieght of point and
        // vertices inw
        // 2. then we need to find the vertex colors over vertex inversewi
        // 3. then we need to find the pixel color over its inversew
        // 4. divide pixel color with pixel inversew

        float pixel_inw = w0 * a.vertex.z + w1 * b.vertex.z + w2 * c.vertex.z;

        // vertex inverse colors
        float v1_cr = c0r * a.vertex.z;
        float v1_cg = c0g * a.vertex.z;
        float v1_cb = c0b * a.vertex.z;

        float v2_cr = c1r * b.vertex.z;
        float v2_cg = c1g * b.vertex.z;
        float v2_cb = c1b * b.vertex.z;

        float v3_cr = c2r * c.vertex.z;
        float v3_cg = c2g * c.vertex.z;
        float v3_cb = c2b * c.vertex.z;

        float r_over_piverse = w0 * v1_cr + w1 * v2_cr + w2 * v3_cr;
        float g_over_piverse = w0 * v1_cg + w1 * v2_cg + w2 * v3_cg;
        float b_over_piverse = w0 * v1_cb + w1 * v2_cb + w2 * v3_cb;

        float red = r_over_piverse / pixel_inw;
        float green = g_over_piverse / pixel_inw;
        float blue = b_over_piverse / pixel_inw;

        uint32_t color =
            ((uint32_t)red << 16) | ((uint32_t)green << 8) | ((uint32_t)blue);

        // TODO: add zbuffer test before putting pixel
        // if () {
        //
        // }
        put_pixel(framebuffer, point.x, point.y, color);
      }
    }
  }
}

// TODO: now we need to implement the textured cube

void draw_textured(mesh *mesh, matrix proj_matrix, texture *texture,
                   float *zbuffer, uint32_t *framebuffer) {

  for (int i = 0; i < mesh->num_face_t; ++i) {
    vec3 v1 = mesh->transformend_vertices[mesh->faces[i].vertex_indices[0]];
    vec3 v2 = mesh->transformend_vertices[mesh->faces[i].vertex_indices[1]];
    vec3 v3 = mesh->transformend_vertices[mesh->faces[i].vertex_indices[2]];

    vec2 uv1 = mesh->uvs[mesh->faces[i].uvs_indices[0]];
    vec2 uv2 = mesh->uvs[mesh->faces[i].uvs_indices[1]];
    vec2 uv3 = mesh->uvs[mesh->faces[i].uvs_indices[2]];

    screen_space_vertex p1 = project_to_screen(v1, proj_matrix);
    screen_space_vertex p2 = project_to_screen(v2, proj_matrix);
    screen_space_vertex p3 = project_to_screen(v3, proj_matrix);

    int minx = fminf(p1.vertex.x, fminf(p2.vertex.x, p3.vertex.x));
    int maxx = fmaxf(p1.vertex.x, fmaxf(p2.vertex.x, p3.vertex.x));
    int miny = fminf(p1.vertex.y, fminf(p2.vertex.y, p3.vertex.y));
    int maxy = fmaxf(p1.vertex.y, fmaxf(p2.vertex.y, p3.vertex.y));

    float area = edge_function(p1.vertex, p2.vertex, p3.vertex);

    for (int x = minx; x < maxx; ++x) {
      for (int y = miny; y < maxy; ++y) {
        vec3 point = (vec3){x + 0.5, y + 0.5, 0};

        float w0 = edge_function(p2.vertex, p3.vertex, point) / area;
        float w1 = edge_function(p3.vertex, p1.vertex, point) / area;
        float w2 = edge_function(p1.vertex, p2.vertex, point) / area;

        if ((w0 >= 0 && w1 >= 0 && w2 >= 0) ||
            (w0 <= 0 && w1 <= 0 && w2 <= 0)) {

          float pixelinw = w0 * p1.inw + w1 * p2.inw + w2 * p3.inw;

          float uow_p1 = uv1.x * p1.inw;
          float vow_p1 = uv1.y * p1.inw;

          float uow_p2 = uv2.x * p2.inw;
          float vow_p2 = uv2.y * p2.inw;

          float uow_p3 = uv3.x * p3.inw;
          float vow_p3 = uv3.y * p3.inw;

          float uow_p = w0 * uow_p1 + w1 * uow_p2 + w2 * uow_p3;
          float vow_p = w0 * vow_p1 + w1 * vow_p2 + w2 * vow_p3;

          float uc = uow_p / pixelinw;
          float vc = vow_p / pixelinw;

          // compuete the depth buffer
          SDL_Color texel = sample_texel(texture, (vec2){uc, vc});
          uint32_t color = ((uint32_t)texel.r << 16 | (uint32_t)texel.g << 8 |
                            (uint32_t)texel.b);

          float z = w0 * p1.vertex.z + w1 * p2.vertex.z + w2 * p3.vertex.z;
          int index = y * WIDTH + x;

          if (z < zbuffer[index]) {
            put_pixel(framebuffer, point.x, point.y, color);
            zbuffer[index] = z;
          }
        }
      }
    }
  }
}

void draw_fill(mesh *mesh, matrix proj_matrix, uint32_t *framebuffer,
               float *zbuffer, uint32_t color1, uint32_t color2,
               uint32_t color3) {

  for (int i = 0; i < mesh->num_face_t; ++i) {
    vec3 v1 = mesh->transformend_vertices[mesh->faces[i].vertex_indices[0]];
    vec3 v2 = mesh->transformend_vertices[mesh->faces[i].vertex_indices[1]];
    vec3 v3 = mesh->transformend_vertices[mesh->faces[i].vertex_indices[2]];

    screen_space_vertex p1 = project_to_screen(v1, proj_matrix);
    screen_space_vertex p2 = project_to_screen(v2, proj_matrix);
    screen_space_vertex p3 = project_to_screen(v3, proj_matrix);

    int minx = fminf(p1.vertex.x, fminf(p2.vertex.x, p3.vertex.x));
    int maxx = fmaxf(p1.vertex.x, fmaxf(p2.vertex.x, p3.vertex.x));
    int miny = fminf(p1.vertex.y, fminf(p2.vertex.y, p3.vertex.y));
    int maxy = fmaxf(p1.vertex.y, fmaxf(p2.vertex.y, p3.vertex.y));

    float area = edge_function(p1.vertex, p2.vertex, p3.vertex);

    for (int x = minx; x < maxx; ++x) {
      for (int y = miny; y < maxy; ++y) {
        vec3 point = (vec3){x + 0.5, y + 0.5, 0};

        float w0 = edge_function(p2.vertex, p3.vertex, point) / area;
        float w1 = edge_function(p3.vertex, p1.vertex, point) / area;
        float w2 = edge_function(p1.vertex, p2.vertex, point) / area;

        float c0r = (color1 >> 16) & 0xFF;
        float c0g = (color1 >> 8) & 0xFF;
        float c0b = (color1) & 0xFF;

        float c1r = (color2 >> 16) & 0xFF;
        float c1g = (color2 >> 8) & 0xFF;
        float c1b = (color2) & 0xFF;

        float c2r = (color3 >> 16) & 0xFF;
        float c2g = (color3 >> 8) & 0xFF;
        float c2b = (color3) & 0xFF;

        if ((w0 >= 0 && w1 >= 0 && w2 >= 0) ||
            (w0 <= 0 && w1 <= 0 && w2 <= 0)) {

          float pixel_inw = w0 * p1.inw + w1 * p2.inw + w2 * p3.inw;

          // vertex inverse colors
          float v1_cr = c0r * p1.inw;
          float v1_cg = c0g * p1.inw;
          float v1_cb = c0b * p1.inw;

          float v2_cr = c1r * p2.inw;
          float v2_cg = c1g * p2.inw;
          float v2_cb = c1b * p2.inw;

          float v3_cr = c2r * p3.inw;
          float v3_cg = c2g * p3.inw;
          float v3_cb = c2b * p3.inw;

          float r_over_piverse = w0 * v1_cr + w1 * v2_cr + w2 * v3_cr;
          float g_over_piverse = w0 * v1_cg + w1 * v2_cg + w2 * v3_cg;
          float b_over_piverse = w0 * v1_cb + w1 * v2_cb + w2 * v3_cb;

          float red = r_over_piverse / pixel_inw;
          float green = g_over_piverse / pixel_inw;
          float blue = b_over_piverse / pixel_inw;

          uint32_t color =
              ((uint32_t)red << 16) | ((uint32_t)green << 8) | ((uint32_t)blue);
          float z = w0 * p1.vertex.z + w1 * p2.vertex.z + w2 * p3.vertex.z;
          int index = y * WIDTH + x;

          if (z < zbuffer[index]) {
            put_pixel(framebuffer, point.x, point.y, color);
            zbuffer[index] = z;
          }
        }
      }
    }
  }
}
