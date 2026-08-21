//
// Created by saad on 8/10/26.
//

#ifndef C_MESH_H
#define C_MESH_H

#include "vec.h"

/// face of triangle
typedef struct face_t {
  int vertex_idices[3];
  int uvs_indices [3];
} face_t;


typedef struct mesh {
  /// vertices of this mesh 
  vec3 *vertices;
  /// uvs of the meshes
  vec2* uvs;
  /// face_t of this mesh
  face_t* faces;    
  /// vert count
  int vert_count;
  /// uvs count
  int uvs_count;
  int num_face_t;
} mesh;

mesh make_cube();
#endif // C_MESH_H
