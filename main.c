


/*
  Implement a 4×4 matrix system and use it to transform your 3D vertices through model, 
  view, and projection transformations into clip space.  
*/

// TODO: test by applying transformation on the points 0101

// TODO: implement view matrix
// TODO: implement projection matrix

#include "sdl_init.h"
#include "mat.h"


int main()
{
  vec4 point = {1, 2, 3, 1};
  matrix scale = make_scaling_matrix(2, 3, 4);
  matrix rotation = make_rotation_matrix(0, 0, 90);
  matrix res = mat_mul_mat(rotation, scale);
  matrix translation = make_translation_matrix(5, 6, 7);
  matrix model_matrix = mat_mul_mat(translation, res);

  create_sdl_window();
}
