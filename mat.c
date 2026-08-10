

#include "mat.h"
#include "vec.h"
#include <math.h>



matrix make_translation_matrix(float tx, float ty, float tz)
{
  return (matrix){
      .mat = {
          {1.0, 0.0, 0.0, tx},
          {0.0, 1.0, 0.0, ty},
          {0.0, 0.0, 1.0, tz},
          {0.0, 0.0, 0.0, 1},
      }};
}



matrix make_rotation_matrix(float pitch, float yaw, float roll)
{

  float alpha = yaw * DAG_TO_RADIANS;
  float beta = pitch * DAG_TO_RADIANS;
  float gamma = roll * DAG_TO_RADIANS;

  float ca = cosf(alpha);
  float sa = sinf(alpha);

  float cb = cosf(beta);
  float sb = sinf(beta);

  float cg = cosf(gamma);
  float sg = sinf(gamma);

  return (matrix){
      .mat = {
          {ca * cb, ca * sb * sg - sa * cg, ca * sb * cg + sa * sg, 0.0},
          {sa * cb, sa * sb * sg + ca * cg, sa * sb * cg - ca * sg, 0.0},
          {-sb, cb * sg, cb * cg, 0.0},
          {0.0, 0.0, 0.0, 1.0},
      }};
}

matrix make_scaling_matrix(float sx, float sy, float sz)
{
  return (matrix){
      .mat = {
          {sx, 0.0, 0.0, 0.0},
          {0.0, sy, 0.0, 0.0},
          {0.0, 0.0, sz, 0.0},
          {0.0, 0.0, 0.0, 1.0}}};
}

vec4 matrix_mul_vec4(matrix matrix, vec4 vec)
{
  vec4 res;
  res.x = matrix.mat[0][0] * vec.x + matrix.mat[0][1] * vec.y + matrix.mat[0][2] * vec.z + matrix.mat[0][3] * vec.w;
  res.y = matrix.mat[1][0] * vec.x + matrix.mat[1][1] * vec.y + matrix.mat[1][2] * vec.z + matrix.mat[1][3] * vec.w;
  res.z = matrix.mat[2][0] * vec.x + matrix.mat[2][1] * vec.y + matrix.mat[2][2] * vec.z + matrix.mat[2][3] * vec.w;
  res.w = matrix.mat[3][0] * vec.x + matrix.mat[3][1] * vec.y + matrix.mat[3][2] * vec.z + matrix.mat[3][3] * vec.w;

  return res;
}

matrix mat_mul_mat(matrix a, matrix b)
{
  matrix result;
  for (int i = 0; i < 4; ++i)
  {
    for (int j = 0; j < 4; ++j)
    {
      result.mat[i][j] = a.mat[i][0] * b.mat[0][j] +
                         a.mat[i][1] * b.mat[1][j] +
                         a.mat[i][2] * b.mat[2][j] +
                         a.mat[i][3] * b.mat[3][j];
    }
  }

  return result;
}


matrix make_view_matrix(vec3 eye, vec3 target) 
{
  vec3 forward = v3_normalize(v3_sub(eye, target));
  vec3 right   = v3_cross((vec3){0.0,1.0,0.0},forward);
  vec3 up      = v3_cross(forward,right);


  return (matrix) {
    .mat = {
        {   right.x,   right.y,   right.z,  -v3_dot(right, eye)},
        {      up.x,      up.y,      up.z,  -v3_dot(up, eye)},
        { forward.x, forward.y, forward.z,  -v3_dot(forward, eye)},
        {       0.0,       0.0,       0.0,   1.0}
    }
  };
}


matrix make_projection_matrix(int screen_width, int screen_height,
                              float fov, float far, float near
                              ) 
{
  float f = 1.0f / tanf(fov * 0.5f * DAG_TO_RADIANS);
  float aspect = (float)screen_width / (float)screen_height;

  return (matrix) {
    .mat = {
        { f / aspect, 0.0,                        0.0,  0.0},
        {        0.0,   f,                        0.0,  0.0},
        {        0.0, 0.0,        -far / (far - near), -1.0},
        {        0.0, 0.0, -far * near / (far - near),  0.0},
    }
  };
}
