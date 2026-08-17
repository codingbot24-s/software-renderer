
// Created by saad on 8/4/26.
//

#include "draw.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include "constant.h"
#include "mesh.h"
#include "vec.h"
#include <stdbool.h>
#include <sys/types.h>

bool is_back_face(vec3 v1, vec3 v2, vec3 v3);

uint32_t *create_framebuff()
{
  uint32_t *framebuff = (uint32_t *)malloc(WIDTH * HIEGHT * sizeof(uint32_t));
  return framebuff;
}

// function will put a single pixel with specified color in the
// framebuffer
void put_pixel(uint32_t *framebuffer, int x, int y, uint32_t color)
{
  if (x >= WIDTH || x <= 0 || y >= HIEGHT || y <= 0)
  {
    return;
  }

  framebuffer[y * WIDTH + x] = color;
}

// clears the framebuffer with given color
void clear_framebuffer(uint32_t *framebuffer, uint32_t color)
{
  for (int r = 0; r < HIEGHT; ++r)
  {
    for (int c = 0; c < WIDTH; ++c)
    {
      framebuffer[r * WIDTH + c] = color;
    }
  }
}

void draw_line(uint32_t *framebuffer, float x1, float y1,
               float x2, float y2, uint32_t color)
{
  float xdiff = x2 - x1;
  float ydiff = y2 - y1;

  if (xdiff == 0.0f && ydiff == 0.0f)
  {
    put_pixel(framebuffer, x1, y1, color);
    return;
  }

  //
  if (fabsf(xdiff) > fabsf(ydiff))
  {
    float xmin;
    float xmax;
    if (x2 > x1)
    {
      xmax = x2;
      xmin = x1;
    }
    else
    {
      xmin = x2;
      xmax = x1;
    }

    float slop = ydiff / xdiff;
    for (float x = xmin; x <= xmax; x += 1.0f)
    {
      // calculate how much we need to increase y
      float y = y1 + (x - x1) * slop;
      put_pixel(framebuffer, x, y, color);
    }
  }
  else
  {
    float ymin;
    float ymax;
    if (y2 > y1)
    {
      ymax = y2;
      ymin = y1;
    }
    else
    {
      ymax = y1;
      ymin = y2;
    }

    float slop = xdiff / ydiff;
    for (float y = ymin; y <= ymax; y += 1.0f)
    {
      float x = x1 + (y - y1) * slop;
      put_pixel(framebuffer, x, y, color);
    }
  }
}

void draw_triangle(float x1, float y1, float x2, float y2, float x3, float y3,
                   uint32_t *framebuffer, uint32_t color)
{

  draw_line(framebuffer, x1, y1, x2, y2, color);
  draw_line(framebuffer, x2, y2, x3, y3, color);
  draw_line(framebuffer, x3, y3, x1, y1, color);
}

void swap(float *a, float *b)
{
  float temp = *a;
  *a = *b;
  *b = temp;
}

void draw_wireframe(mesh *mesh, uint32_t color, matrix proj_matrix, uint32_t *framebuffer)
{
  for (int i = 0; i < mesh->triangle_count; ++i)
  {
    // TODO: the odin renderer is using orignal vertices here but we are using
    //  transformend
    //  check why ?
    vec3 v1 = mesh->transformed_vertices[mesh->triangles[i].vertices[0]];
    vec3 v2 = mesh->transformed_vertices[mesh->triangles[i].vertices[1]];
    vec3 v3 = mesh->transformed_vertices[mesh->triangles[i].vertices[2]];

    vec3 p1 = project_to_screen(v1, proj_matrix);
    vec3 p2 = project_to_screen(v2, proj_matrix);
    vec3 p3 = project_to_screen(v3, proj_matrix);

    // we should check here for back face culling
    if (is_back_face(v1, v2, v3))
    {
      continue;
    }
    draw_line(framebuffer, p1.x, p1.y, p2.x, p2.y, color);
    draw_line(framebuffer, p2.x, p2.y, p3.x, p3.y, color);
    draw_line(framebuffer, p3.x, p3.y, p1.x, p1.y, color);
  }
}

bool is_back_face(vec3 v1, vec3 v2, vec3 v3)
{
  vec3 edge_1 = v3_sub(v2, v1);
  vec3 edge_2 = v3_sub(v3, v1);

  vec3 cross = v3_cross(edge_1, edge_2);
  vec3 cross_norm = v3_normalize(cross);
  vec3 to_camera = v3_normalize(v1);

  float dot = v3_dot(cross_norm, to_camera);

  return dot >= 0.0;
}

float edge_function(vec3 a, vec3 b, vec3 p)
{
  return (p.x - a.x) * (b.y - a.y) - (p.y - a.y) * (b.x - a.x);
}

// pass three color here
// and calculate by bc cordinate the color of triangles

void fill_triangle(vec3 a, vec3 b, vec3 c,
                   uint32_t *framebuffer, float *zbuffer,
                   uint32_t color1,
                   uint32_t color2,
                   uint32_t color3)
{

  // float to interger conversion
  int minx = fminf(a.x, fminf(b.x, c.x));
  int maxx = fmaxf(a.x, fmaxf(b.x, c.x));

  int miny = fminf(a.y, fminf(b.y, c.y));
  int maxy = fmaxf(a.y, fmaxf(b.y, c.y));

  float bt_area = edge_function(a, b, c);

  // w0 * c0[0] + w1 * c1[0] + w2 * c2[0]

  for (int i = miny; i < maxy; i++)
  {
    for (int j = minx; j < maxx; j++)
    {
      vec3 point = (vec3){j + 0.5, i + 0.5, 0.0};

      float w0 = edge_function(a, b, point);
      float w1 = edge_function(b, c, point);
      float w2 = edge_function(c, a, point);

      float c0r = (color1 >> 16) & 0xFF;
      float c0g = (color1 >> 8) & 0xFF;
      float c0b = (color1) & 0xFF;

      float c1r = (color2 >> 16) & 0xFF;
      float c1g = (color2 >> 8) & 0xFF;
      float c1b = (color2) & 0xFF;

      float c2r = (color3 >> 16) & 0xFF;
      float c2g = (color3 >> 8) & 0xFF;
      float c2b = (color3) & 0xFF;

      if ((w0 >= 0 && w1 >= 0 && w2 >= 0) || (w0 <= 0 && w1 <= 0 && w2 <= 0))
      {
        w0 /= bt_area;
        w1 /= bt_area;
        w2 /= bt_area;

        float r = w0 * c0r + w1 * c1r + w2 * c2r;
        float g = w0 * c0g + w1 * c1g + w2 * c2g;
        float b = w0 * c0b + w1 * c1b + w2 * c2b;

        uint32_t color = ((uint32_t)r << 16) | ((uint32_t)g << 8) | ((uint32_t)b);

        //TODO: add zbuffer test before putting pixel
        if () {
        
        }
        put_pixel(framebuffer, point.x, point.y, color);
      }
    }
  }
}
