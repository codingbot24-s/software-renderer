
#include "vec.h"
#include <math.h>
#include <stdexcept>
#include <stdlib.h>

// WE WILL DEBUG THIS IN ANOTHER STREAM
vec3 v3_init(float x, float y, float z)
{
  return (vec3){
      .x = x,
      .y = y,
      .z = z,
  };
}

vec3 v3_sub(vec3 a, vec3 b)
{
  vec3 res;
  res.x = a.x - b.x;
  res.y = a.y - b.y;
  res.z = a.z - b.z;

  return res;
}

vec3 v3_normalize(vec3 v)
{
  vec3 res;
  float len = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);

  res.x = v.x / len;
  res.y = v.y / len;
  res.z = v.z / len;

  return res;
}

vec3 v3_cross(vec3 a, vec3 b)
{
  vec3 result;
  result.x = a.y * b.z - a.z * b.y;
  result.y = a.z * b.x - a.x * b.z;
  result.z = a.x * b.y - a.y * b.x;
  return result;
}

float v3_dot(vec3 a, vec3 b)
{
  // the bug was here
  float result = (a.x * b.x + a.y * b.y + a.z * b.z);
  return result;
}

vec4 v4_init(float x, float y, float z, float w)
{
  return (vec4){
      .x = x,
      .y = y,
      .z = z,
      .w = w,
  };
}

// NOTE: check for null otherwise we will get the crash
vector make_vector(data_type type, int count) {
  vector vector = {
      .type = type,
      // initialy we can push 8 element
      .len = 8,
      .count = 0,
      .data = malloc(8 * sizeof(data_type)),
  };
  return vector;
}

int append(vector *vec, void *elem) {
	if (vec->count >= vec->len) {
		vec->len = vec->len * 2;
		vec->data = realloc(vec->data,vec->len * sizeof(vec->type));
	}
	if (vec->type == v2_type) {
	  vec2* v2 =  (vec2*)elem;
	  ((vec2*)vec->data)[vec->count] = *v2;
	  vec->count++;
	} else if (vec->type == v3_type) {
	  vec3 *v3 = (vec3 *)elem;
	  ((vec3 *)vec->data)[vec->count] = *v3;
	  vec->count++;
	} else {
	  vec4 *v4 = (vec4 *)elem;
	  ((vec4*)vec->data)[vec->count] = *v4;
	  vec->count++;
  }
}
