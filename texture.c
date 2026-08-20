#include "texture.h"
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_surface.h>
#include <SDL3_image/SDL_image.h>
#include <stdint.h>
#include <stdio.h>


texture load_texture_from_file(char* filename)
{
  texture texture;
  SDL_Surface* surface = IMG_Load(filename);
  if (!surface) {
    fprintf(stderr, "Cant load the image in sdl surface \n");
    return texture;
  }
  
  texture.width = surface->w;
  texture.height = surface->h;
  
  SDL_Surface* rgba = SDL_ConvertSurface(surface,SDL_PIXELFORMAT_RGBA32);
  if (!rgba) {
    fprintf(stderr, "Cant load the image in sdl surface \n");
    return texture;
  }
  
  uint32_t* raw_pixels = (uint32_t*)rgba->pixels;
  
  // and extract every color by bit shift
  int pixel_count = texture.height * texture.width;
  for (int i = 0; i < pixel_count; ++i) {
   texture.pixels->r = ((uint32_t)raw_pixels[i] >> 24) & 0xFF;
   texture.pixels->g = ((uint32_t)raw_pixels[i] >> 16) & 0xFF;
   texture.pixels->b = ((uint32_t)raw_pixels[i] >> 4)  & 0xFF;
   texture.pixels->a = raw_pixels[i] & 0xFF;
  }
  SDL_DestroySurface(surface);
  return texture;  
}
