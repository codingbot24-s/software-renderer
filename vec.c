
#include "mesh.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>

vec3 v3_init(float x, float y, float z) {
  return (vec3){
      .x = x,
      .y = y,
      .z = z,
  };
}

vec3 v3_sub(vec3 a, vec3 b) {
  vec3 res;
  res.x = a.x - b.x;
  res.y = a.y - b.y;
  res.z = a.z - b.z;

  return res;
}

vec3 v3_normalize(vec3 v) {
  vec3 res;
  float len = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);

  res.x = v.x / len;
  res.y = v.y / len;
  res.z = v.z / len;

  return res;
}

vec3 v3_cross(vec3 a, vec3 b) {
  vec3 result;
  result.x = a.y * b.z - a.z * b.y;
  result.y = a.z * b.x - a.x * b.z;
  result.z = a.x * b.y - a.y * b.x;
  return result;
}

float v3_dot(vec3 a, vec3 b) {
  // the bug was here
  float result = (a.x * b.x + a.y * b.y + a.z * b.z);
  return result;
}

vec4 v4_init(float x, float y, float z, float w) {
  return (vec4){
      .x = x,
      .y = y,
      .z = z,
      .w = w,
  };
}

// NOTE: call to this will allocate a 8 * size of type like  8 * int
vector *make_vector(data_type type, int how_much) {

  if (how_much == 0) {
    how_much = 8;
  }
  size_t size_of_type = 0;
  if (type == v2_type) {
    size_of_type = sizeof(vec2);
  } else if (type == v3_type) {
    size_of_type = sizeof(vec3);
  } else if (type == v4_type) {
    size_of_type = sizeof(vec4);
  } else if (type == face_type) {
    size_of_type = sizeof(face_t);
  }

  vector *v = malloc(sizeof(vector));
  v->type = type;
  v->len = 8;
  v->count = 0;
  v->elem_type = size_of_type;

  v->data = malloc(how_much * size_of_type);
  return v;
}

void append(vector *vec, void *elem) {
  if (vec->count >= vec->len) {
    vec->len = vec->len * 2;
    vec->data = realloc(vec->data, vec->len * vec->elem_type);
  }
  if (vec->type == v2_type) {
    vec2 *v2 = (vec2 *)elem;
    ((vec2 *)vec->data)[vec->count] = *v2;
    vec->count++;
  } else if (vec->type == v3_type) {
    vec3 *v3 = (vec3 *)elem;
    ((vec3 *)vec->data)[vec->count] = *v3;
    vec->count++;
  } else if (vec->type == v4_type) {
    vec4 *v4 = (vec4 *)elem;
    ((vec4 *)vec->data)[vec->count] = *v4;
    vec->count++;
  } else if (vec->type == face_type) {
    face_t *face = (face_t *)elem;
    ((face_t *)vec->data)[vec->count] = *face;
    vec->count++;
  }
}

// TODO: Implement Free for vector
