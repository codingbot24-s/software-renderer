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
  vec3 *vertices;
  vec3 *transformend_vertices;
  /// uvs of the meshes
  vec2 *uvs;
  /// normals of the meshes
  vec3 *normals;
  vec3 *transformed_normals;
  /// face_t of this mesh
  face_t *faces;
  /// vert count
  int vert_count;
  /// uvs count
  int uvs_count;
  int normals_count;
  int num_face_t;
} mesh;

mesh *make_mesh(vec3 *vertices, int vert_count, vec2 *uvs, int uvs_count,
                face_t *triangles, int face_t_count, int normals_count,
                vec3 *normals);
#endif // C_MESH_H
