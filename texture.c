#include "texture.h"
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <SDL3_image/SDL_image.h>
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
  SDL_PixelFormat format = surface->format;
  const SDL_PixelFormatDetails* details = SDL_GetPixelFormatDetails(format);
  texture.Rmask = details->Rmask;  
  texture.Gmask = details->Gmask;  
  texture.Bmask = details->Bmask;
  SDL_DestroySurface(surface);
  return texture;  
}
