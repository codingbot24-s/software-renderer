


/*
  Implement a 4×4 matrix system and use it to transform your 3D vertices through model, 
  view, and projection transformations into clip space.  
*/


#include <stdio.h>

#include "constant.h"
#include "sdl_init.h"
#include "mat.h"
#include "vec.h"


int main()
{
  
  vec3 eye = {0,0,10};
  vec3 target = {0,0,0,};
  vec4 point = {0, 0, 0, 1};
  matrix scale = make_scaling_matrix(2, 3, 4);
  matrix rotation = make_rotation_matrix(0, 0, 90);
  matrix res = mat_mul_mat(rotation, scale);
  matrix translation = make_translation_matrix(5, 6, 7);
  matrix model_matrix = mat_mul_mat(translation, res);
  matrix view_matrix = make_view_matrix(eye, target);
  matrix model_view_matrix = mat_mul_mat(view_matrix, model_matrix);
  vec4 view_space_point = matrix_mul_vec4(model_view_matrix,point);
  matrix projection_matrix = make_projection_matrix(WIDTH,HIEGHT,FOV,FAR_PLANE,NEAR_PLANE);
  vec4 clip_space = matrix_mul_vec4(projection_matrix,view_space_point);
 
  create_sdl_window();
}
