
// Created by saad on 8/4/26.
//

#include "draw.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include "constant.h"
#include "mesh.h"
#include "vec.h"


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

void fill_triangle(float x1, float y1, float z1,
                   float x2, float y2, float z2,
                   float x3, float y3, float z3,
                   uint32_t *framebuffer, uint32_t color,
                   float *zbuffer)
{
  // y1 > it means a.y if its is more then y2 == b.y
  // this means a is more down in the space
  // this is cofusing we should we implement vectors

  // TODO: test this sort with swap
  if (y1 > y2)
  {
    swap(&x1, &x2);
    swap(&y1, &y2);
    swap(&z1, &z2);
  }

  if (y1 > y3)
  {
    swap(&x1, &x3);
    swap(&y1, &y3);
    swap(&z1, &z3);
  }

  if (y2 > y3)
  {
    swap(&y2, &y3);
    swap(&x2, &x3);
    swap(&z2, &z3);
  }
  // full distance of c to a in y
  // NOTE: possible division by 0 if all of them on solve this
  float distance_c_to_a = y3 - y1;
  // not the flat top
  if (y2 != y1)
  {
    // distance of b ----> a in y
    // one segment hieght
    float distance_b_to_a = y2 - y1;
    for (int y = y1; y < y2; y++)
    {
      float f_x = x1 + ((x3 - x1) * (y - y1)) / distance_c_to_a;
      float s_x = x1 + ((x2 - x1) * (y - y1)) / distance_b_to_a;
      float left_z = z1 + ((z2 - z1) * (y - y1)) / distance_b_to_a;
      float right_z = z1 + ((z3 - z1) * (y - y1)) / distance_c_to_a;
      for (int x = fmin(f_x, s_x); x < fmax(f_x, s_x); x++)
      {
        float z = right_z + (left_z - right_z) * (x - f_x) / (s_x - f_x);
        int zindex = y * WIDTH + x;
        if (z < zbuffer[zindex])
        {
          zbuffer[zindex] = z;
          put_pixel(framebuffer, x, y, color);
        }
      }
    }
  }
  // not the flat bottom
  if (y2 != y3)
  {
    // distance of c ----> b in y
    // one segment hieght
    int distance_c_to_b = y3 - y2;
    for (int y = y2; y <= y3; y++)
    {
      float f_x = x1 + ((x3 - x1) * (y - y1)) / distance_c_to_a;
      float left_z = z2 + ((z3 - z2) * (y - y2)) / distance_c_to_b;
      float right_z = z1 + ((z3 - z1) * (y - y1)) / distance_c_to_a;
      float s_x = x2 + ((x3 - x2) * (y - y2)) / distance_c_to_b;
      for (int x = fmin(f_x, s_x); x < fmax(f_x, s_x); x++)
      {
        float z = right_z + (left_z - right_z) * (x - f_x) / (s_x - f_x);
        int zindex = y * WIDTH + x;
        if (z < zbuffer[zindex])
        {
          zbuffer[zindex] = z;
          put_pixel(framebuffer, x, y, color);
        }
      }
    }
  }
}

void draw_wireframe(mesh* mesh, uint32_t color, matrix proj_matrix, uint32_t* framebuffer)
{

  for (int i = 0; i < mesh->triangle_count; ++i) 
  {
    // the bug is here we need to pass our transformed vertices 
    // not original one
    vec3 v1 = mesh->vertices[mesh->triangles[i].vertices[0]];
    vec3 v2 = mesh->vertices[mesh->triangles[i].vertices[1]];
    vec3 v3 = mesh->vertices[mesh->triangles[i].vertices[2]];

    vec3 p1 = project_to_screen(v1,proj_matrix);
    vec3 p2 = project_to_screen(v2,proj_matrix);
    vec3 p3 = project_to_screen(v3,proj_matrix);


    draw_triangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y,framebuffer ,color);
  }

}
