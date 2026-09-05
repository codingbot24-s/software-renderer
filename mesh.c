//
//
// Created by saad on 8/10/26.
//
//
#include "mesh.h"
#include "vec.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int is_space(const char chr) {
  if (chr == ' ' || chr == '\t') {
    return 1;
  } else {
    return 0;
  }
}

int parse_line(char *line, mesh *m) {
  char *token = line;
  if (token[0] == '\0') {
    return 0;
  }
  if (token[0] == '#') {
    return 0;
  }
  if (token[0] == 'v' && is_space(token[1])) {
    float x, y, z;
    if (sscanf(token, "v %f %f %f", &x, &y, &z) == 3) {

      vec3 vertex = (vec3){x, y, z};
      append(m->vertices, &vertex);
      m->vert_count = m->vertices->count;
    }
  }
  if (token[0] == 'v' && token[1] == 'n' && is_space(token[2])) {
    float x, y, z;
    if (sscanf(token, "vn %f %f %f", &x, &y, &z) == 3) {
      vec3 normal = (vec3){x, y, z};
      append(m->normals, &normal);
      m->normals_count = m->normals->count;
    }
  }
  if (token[0] == 'v' && token[1] == 't' && is_space(token[2])) {

    float u, v;
    if (sscanf(token, "vt %f %f", &u, &v) == 2) {
      vec2 uvs = (vec2){u, v};
      append(m->uvs, &uvs);
      m->uvs_count = m->uvs->count;
    }
  }

  // NOTE: currently our renderer can only load a faces in this format no other
  // format f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3

  if (token[0] == 'f' && is_space(token[1])) {
    int v1, vt1, vn1;
    int v2, vt2, vn2;
    int v3, vt3, vn3;
    if (sscanf(token, "f %d/%d/%d %d/%d/%d %d/%d/%d", &v1, &vt1, &vn1, &v2,
               &vt2, &vn2, &v3, &vt3, &vn3) == 9) {
      face_t face = {
          .vertex_indices = {v1 - 1, v2 - 1, v3 - 1},
          .uvs_indices = {vt1 - 1, vt2 - 1, vt3 - 1},
          .normal_indices = {vn1 - 1, vn2 - 1, vn3 - 1},
      };
      append(m->faces, &face);
      m->num_face_t = m->faces->count;
    } else if (sscanf(token, "f %d %d %d", &v1, &v2, &v3) == 3) {

      face_t face = {
          .vertex_indices = {v1 - 1, v2 - 1, v3 - 1},
          .uvs_indices = {-1, -1, -1},
          .normal_indices = {-1, -1, -1},
      };

      append(m->faces, &face);
      m->num_face_t = m->faces->count;
    }
  }
  return 1;
}

// NOTE: always check before using this function that
// this is not returning null else we will get the crash
mesh *load_mesh(const char *path) {
  FILE *f = fopen(path, "r");
  if (f == NULL) {
    fprintf(stderr, "cant open the file \n");
    return NULL;
  }

  if (fseek(f, 0, SEEK_END) != 0) {
    fprintf(stderr, "seeking error \n");
    return NULL;
  }
  ssize_t fsize = ftell(f);
  if (fsize < 0) {
    fclose(f);
    return NULL;
  }
  rewind(f);

  char *buffer = malloc(fsize + 1);
  if (buffer == NULL) {
    fprintf(stderr, "Cant allocate the buffer for reading \n");
    return NULL;
  }

  size_t bytes_read = fread(buffer, 1, fsize, f);
  if (bytes_read < fsize) {
    fprintf(stderr, "cant read the file \n");
    free(buffer);
    return NULL;
  }

  mesh *mesh = malloc(sizeof(*mesh));
  mesh->vert_count = 0;
  mesh->uvs_count = 0;
  mesh->normals_count = 0;
  mesh->vertices = make_vector(v3_type, 0);
  mesh->uvs = make_vector(v2_type, 0);
  mesh->normals = make_vector(v3_type, 0);
  mesh->faces = make_vector(face_type, 0);

  buffer[bytes_read] = '\0';
  char *line = strtok(buffer, "\n");
  while (line != NULL) {
    parse_line(line, mesh);
    line = strtok(NULL, "\n");
  }

  mesh->transformed_vertices = make_vector(v3_type, mesh->vert_count);
  mesh->transformed_normals = make_vector(v3_type, mesh->normals_count);

  free(buffer);
  fclose(f);
  return mesh;
}

void delete_mesh() {}
