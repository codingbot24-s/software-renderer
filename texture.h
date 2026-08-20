#pragma once

#include <SDL3/SDL_pixels.h>
#include <stdint.h>

typedef struct texture {
  int width;
  int height;
  SDL_Color* pixels;
} texture;


texture load_texture_from_file(char* filename);
