
#include "vec.h"
#include <math.h>


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


float  v3_dot(vec3 a,vec3 b) 
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
