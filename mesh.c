//
// Created by saad on 8/10/26.
//

#include "mesh.h"
#include "vec.h"
#include <stdlib.h>
#include <string.h>

mesh make_cube() 
{
  mesh cube; 
  vec3* vertices = malloc(8 *sizeof(vec3));
  vertices[0] = (vec3){-1.0, -1.0, -1.0};
  vertices[1] = (vec3){-1.0,  1.0, -1.0};
  vertices[2] = (vec3){1.0,  1.0, -1.0};
  vertices[3] = (vec3){1.0, -1.0, -1.0};
  vertices[4] = (vec3){1.0,  1.0,  1.0};
  vertices[5] = (vec3){1.0, -1.0,  1.0};
  vertices[6] = (vec3){-1.0, 1.0, 1.0};
  vertices[7] = (vec3){-1.0, -1.0, 1.0};
  
  triangle* triangles = malloc(12 * sizeof(triangle));
  int vert_indices_1[3] = {0,1,2};
  memcpy(triangles[0].vertices,vert_indices_1,sizeof(vert_indices_1));
  
  int vert_indices_2[3] = {0,2,3};
  memcpy(triangles[1].vertices,vert_indices_2,sizeof(vert_indices_1));

  int vert_indices_3[3] = {3,2,4};
  memcpy(triangles[2].vertices,vert_indices_3,sizeof(vert_indices_1));   

  int vert_indices_4[3] = {3,4,5};
  memcpy(triangles[3].vertices,vert_indices_4,sizeof(vert_indices_1));

  int vert_indices_5[3] = {5,4,6};
  memcpy(triangles[4].vertices,vert_indices_5,sizeof(vert_indices_1));

  int vert_indices_6[3] = {5,6,7};
  memcpy(triangles[5].vertices,vert_indices_6,sizeof(vert_indices_1));

  int vert_indices_7[3] = {7,6,1};
  memcpy(triangles[6].vertices,vert_indices_7,sizeof(vert_indices_1));

  int vert_indices_8[3] = {7,1,0};
  memcpy(triangles[7].vertices,vert_indices_8,sizeof(vert_indices_1));

  int vert_indices_9[3] = {1,6,4};
  memcpy(triangles[8].vertices,vert_indices_9,sizeof(vert_indices_1));

  int vert_indices_10[3] = {1,4,2};
  memcpy(triangles[9].vertices,vert_indices_10,sizeof(vert_indices_1));

  int vert_indices_11[3] = {5,7,0};
  memcpy(triangles[10].vertices,vert_indices_11,sizeof(vert_indices_1));
  // there was a bug here previous indcies was wrong
  int vert_indices_12[3] = {5,0,3};
  memcpy(triangles[11].vertices,vert_indices_12,sizeof(vert_indices_1));

  cube.vertices = vertices;
  cube.triangles = triangles;
  cube.triangle_count = 12;
  cube.vert_count = 8;
  cube.transformed_vertices = malloc(8 * sizeof(vec3));

  return cube;
}
