#pragma once

typedef struct vec3
{
  float x;
  float y;
  float z;
} vec3;

vec3 v3_init(float x, float y, float z);
vec3 v3_sub(vec3 a, vec3 b);
vec3 v3_normalize(vec3 v);
vec3  v3_cross(vec3 a,vec3 b);
float  v3_dot(vec3 a,vec3 b);

typedef struct vec4
{
  float x;
  float y;
  float z;
  float w;
} vec4;

vec4 v4_init(float x, float y, float z, float w);


