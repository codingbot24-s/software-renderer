
#include "texture.h"
#include "vec.h"
#include <SDL3/SDL_pixels.h>
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

  texture.width = surface->w;
  texture.height = surface->h;
  texture.pixels = malloc(texture.width * texture.height * sizeof(SDL_Color));

  SDL_Surface *rgba = SDL_ConvertSurface(surface, SDL_PIXELFORMAT_RGBA32);
  if (!rgba) {
    fprintf(stderr, "Cant convert to format \n");
    return texture;
  }

  uint32_t *raw_pixels = (uint32_t *)rgba->pixels;

  int pixel_count = texture.height * texture.width;
  for (int i = 0; i < pixel_count; ++i) {
    texture.pixels[i].r = ((uint32_t)raw_pixels[i] >> 24) & 0xFF;
    texture.pixels[i].g = ((uint32_t)raw_pixels[i] >> 16) & 0xFF;
    texture.pixels[i].b = ((uint32_t)raw_pixels[i] >> 8) & 0xFF;
    texture.pixels[i].a = raw_pixels[i] & 0xFF;
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
