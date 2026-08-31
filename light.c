//
// Created by saad on 8/28/26.
//

#include "light.h"
#include "vec.h"

light make_light(vec3 pos, matrix view_matrix, vec3 direction,
                 float intensity) {
  light light;
  light.position = matrix_mul_vec3(view_matrix, pos);
  light.direction = v3_normalize(direction);
  light.intensity = intensity;

  return light;
}
