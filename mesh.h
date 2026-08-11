//
// Created by saad on 8/10/26.
//

#ifndef C_MESH_H
#define C_MESH_H

#include "vec.h"

#define triangle_size 3


typedef struct triangle {
  int vertices[3];  
} triangle;

typedef struct mesh {
  vec3* vertices;
  vec3* transformed_vertices;
  triangle* triangles;
  int triangle_count;
  int   vert_count;
} mesh; 

mesh make_cube();
#endif //C_MESH_H
