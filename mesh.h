//
// Created by saad on 8/10/26.
//

#ifndef C_MESH_H
#define C_MESH_H

#include "vec.h"

/// face of triangle
typedef struct face_t {
  int vertex_indices[3];
  int uvs_indices[3];
  int normal_indices[3];
} face_t;

typedef struct mesh {
  /// vertices of this mesh
  vector *vertices;
  vector *transformed_vertices;
  /// uvs of the meshes
  vector *uvs;
  /// normals of the meshes
  vector *normals;
  vector *transformed_normals;
  /// face_t of this mesh
  vector *faces;
  /// vert count
  int vert_count;
  /// uvs count
  int uvs_count;
  int normals_count;
  int num_face_t;
} mesh;

mesh *load_mesh(const char *path);
#endif // C_MESH_H
