#pragma once

#include "vec.h"
#include <SDL3/SDL_pixels.h>
#include <stdint.h>

typedef struct texture {
  int width;
  int height;
  SDL_Color *pixels;
} texture;

texture load_texture_from_file(char *filename);
SDL_Color sample_texel(texture *texture, vec2 uv_cor);
