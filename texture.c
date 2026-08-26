
#include "texture.h"
#include "vec.h"
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_video.h>
#include <SDL3_image/SDL_image.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

texture load_texture_from_file(char *filename) {
  texture texture;
  SDL_Surface *surface = IMG_Load(filename);
  if (!surface) {
    fprintf(stderr, "Cant load the image in sdl surface error : %s\n",
            SDL_GetError());
    return texture;
  }

  SDL_Surface *rgba = SDL_ConvertSurface(surface, SDL_PIXELFORMAT_RGBA32);

  if (!rgba) {
    fprintf(stderr, "Cant convert to format: %s\n", SDL_GetError());
    SDL_DestroySurface(surface);
    return texture;
  }

  texture.width = rgba->w;
  texture.height = rgba->h;

  texture.pixels = malloc(texture.width * texture.height * sizeof(SDL_Color));
  for (int y = 0; y < texture.height; ++y) {
    // get the row
    // pitch is row in bytes
    // rgba pixels is first byte image
    uint8_t *row = (uint8_t *)rgba->pixels + y * rgba->pitch;
    for (int x = 0; x < texture.width; ++x) {
      // pixel is 4 byte so x * 4 equal to first pixel
      uint8_t *pixel = row + x * 4;

      texture.pixels[y * texture.width + x].r = pixel[0];
      texture.pixels[y * texture.width + x].g = pixel[1];
      texture.pixels[y * texture.width + x].b = pixel[2];
      texture.pixels[y * texture.width + x].a = pixel[3];
    }
  }
  SDL_DestroySurface(surface);
  return texture;
}

SDL_Color sample_texel(texture *texture, vec2 uv_cor) {
  float u = uv_cor.x - (float)floorf(uv_cor.x);
  float v = uv_cor.y - (float)floorf(uv_cor.y);

  int x = (int)((texture->width - 1) * u);
  int y = (int)((texture->height - 1) * v);

  return texture->pixels[y * texture->width + x];
}
