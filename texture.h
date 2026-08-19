#pragma once

#include <stdint.h>

typedef struct texture {
  int width;
  int height;
  uint32_t Rmask;
  uint32_t Gmask;
  uint32_t Bmask;
  
} texture;


texture load_texture_from_file(char* filename);
