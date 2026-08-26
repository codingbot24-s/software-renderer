
#include "vec.h"

#define DAG_TO_RADIANS 0.01745329251

typedef struct matrix {
  float mat[4][4];
} matrix;

typedef struct screen_space_vertex {
  vec3 vertex;
  float inw;
} screen_space_vertex;

matrix make_translation_matrix(float tx, float ty, float tz);
vec4 matrix_mul_vec4(matrix matrix, vec4 vec);
matrix make_rotation_matrix(float yaw, float pitch, float roll);
matrix make_scaling_matrix(float sx, float sy, float sz);
matrix mat_mul_mat(matrix a, matrix b);
matrix make_view_matrix(vec3 eye, vec3 target);
matrix make_projection_matrix(int screen_width, int screen_height, float fov,
                              float far, float near);
screen_space_vertex project_to_screen(vec3 point, matrix proj_matrix);
vec3 matrix_mul_vec3(matrix matrix, vec3 vec);
