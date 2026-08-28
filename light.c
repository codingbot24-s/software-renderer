//
// Created by saad on 8/28/26.
//

#include "light.h"
#include "vec.h"

light make_light(vec3 direction, float intensity) {
  light light;
  // we need to normalize this vector
  light.direction = v3_normalize(direction);
  light.intensity = intensity;

  return light;
}
