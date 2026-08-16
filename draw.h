//
// Created by saad on 8/4/26.
//

#ifndef C_DRAW_H
#define C_DRAW_H
#include <stdint.h>
#include "mesh.h"
#include "mat.h"

uint32_t *create_framebuff();

void put_pixel(uint32_t *framebuffer, int x, int y, uint32_t color);

void clear_framebuffer(uint32_t *framebuffer, uint32_t color);

void draw_line(uint32_t *framebuffer, float x1, float y1,
               float x2, float y2, uint32_t color);

void draw_triangle(float x1, float y1, float x2, float y2,
                   float x3, float y3, uint32_t *framebuffer, uint32_t color);
void draw_wireframe(mesh *mesh, uint32_t color, matrix proj_matrix, uint32_t *framebuffer);

void fill_triangle(vec3 a, vec3 b, vec3 c,
                   uint32_t *framebuffer, float *zbuffer,
                   uint32_t color1,
                   uint32_t color2,
                   uint32_t color3
                   );

#endif // C_DRAW_H
