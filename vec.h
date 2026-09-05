#pragma once

#include <stddef.h>
typedef struct vec2 {
  float x;
  float y;
} vec2;

typedef struct vec3 {
  float x;
  float y;
  float z;
} vec3;

vec3 v3_init(float x, float y, float z);
vec3 v3_sub(vec3 a, vec3 b);
vec3 v3_normalize(vec3 v);
vec3 v3_cross(vec3 a, vec3 b);
float v3_dot(vec3 a, vec3 b);

typedef struct vec4 {
  float x;
  float y;
  float z;
  float w;
} vec4;

vec4 v4_init(float x, float y, float z, float w);

typedef enum {
  v2_type,
  v3_type,
  v4_type,
  face_type,
} data_type;

typedef struct vector {
  void *data;
  int len;
  int count;
  size_t elem_type;
  data_type type;
} vector;

vector *make_vector(data_type type, int how_much);
void append(vector *vec, void *elem);
