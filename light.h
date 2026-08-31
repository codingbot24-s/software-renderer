//
// Created by saad on 8/28/26.
//

#ifndef SRENDERER_LIGHT_H
#define SRENDERER_LIGHT_H

#include "mat.h"
#include "vec.h"

typedef struct light {
  vec3 position;
  vec3 direction;
  float intensity;
} light;

light make_light(vec3 pos, matrix view_matrix, vec3 direction, float intensity);

#endif // SRENDERER_LIGHT_H
