//
// 1. if all the weights are positive
// Created by saad on 8/10/26.
//

#include "mesh.h"
#include "vec.h"
#include <stdlib.h>

mesh *make_mesh(vec3 *vertices, int vert_count, vec2 *uvs, int uvs_count,
                face_t *triangles, int face_t_count, int normals_count,
                vec3 *normals) {

  mesh *mesh = malloc(sizeof(mesh));
  mesh->vertices = malloc(vert_count * sizeof(vec3));
  mesh->transformend_vertices = malloc(vert_count * sizeof(vec3));
  mesh->normals = malloc(normals_count * sizeof(vec3));
  mesh->transformed_normals = malloc(normals_count * sizeof(vec3));
  mesh->uvs = malloc(uvs_count * sizeof(vec2));
  mesh->faces = malloc(face_t_count * sizeof(face_t));

  for (int i = 0; i < vert_count; ++i) {
    mesh->vertices[i] = vertices[i];
  }

  for (int i = 0; i < normals_count; ++i) {
    mesh->normals[i] = normals[i];
  }

  for (int i = 0; i < uvs_count; ++i) {
    mesh->uvs[i] = uvs[i];
  }

  for (int i = 0; i < face_t_count; ++i) {
    mesh->faces[i] = triangles[i];
  }

  mesh->normals_count = normals_count;
  mesh->num_face_t = face_t_count;
  mesh->uvs_count = uvs_count;
  mesh->vert_count = vert_count;

  return mesh;
}

void free_mesh(mesh *mesh) {}
