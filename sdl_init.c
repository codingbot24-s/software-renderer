//
// Created by saad on 8/3/26.
//

#include "sdl_init.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <stdint.h>
#include <stdio.h>
#include "constant.h"
#include "draw.h"
#include <stdlib.h>
#include <float.h>
#include <time.h>
#include "vec.h"
#include "mat.h"

void clean_zbuffer(float *z_buffer);


static bool running = true;

int create_sdl_window()
{
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    printf("sdl init failed %s\n", SDL_GetError());
    return -1;
  }
  // here using sdl window resizable will cancel
  // our width and hieght
  SDL_Window *window = SDL_CreateWindow("renderer window", WIDTH, HIEGHT, 0);
  if (!window)
  {
    printf("window creation error %s\n", SDL_GetError());
    return -1;
  }

  SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
  if (!renderer)
  {
    printf("renderer creation error %s\n", SDL_GetError());
    return -1;
  }

  SDL_SetWindowResizable(window, true);
  uint32_t *frame_buffer = create_framebuff();
  SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_XRGB8888, SDL_TEXTUREACCESS_STREAMING, WIDTH, HIEGHT);

  float z_buffer[WIDTH * HIEGHT];

  vec3  eye    = {0, 0, -3};
  vec3 target = {0, 0, -5};

  vec4 point  = {0, 0, -5, 1};
  float scale = 1.0;
  matrix m_scaled = make_scaling_matrix(scale, scale,scale);
  matrix m_rotation = make_rotation_matrix(0, 0, 0);
  matrix m_tranlation = make_translation_matrix(0, 0, 0); 
  matrix model_matrix = mat_mul_mat(m_tranlation,mat_mul_mat(m_rotation,m_scaled));
  matrix view_matrix = make_view_matrix(eye,target); 
  matrix fin_matrix  = mat_mul_mat(view_matrix,model_matrix);
  vec4   view_space_point = matrix_mul_vec4(fin_matrix, point);
  matrix proj_matrix  = make_projection_matrix(WIDTH, HIEGHT, FOV, FAR_PLANE,NEAR_PLANE);
  vec4   clip_space   = matrix_mul_vec4(proj_matrix,view_space_point);
  vec3 projected = project_to_screen(clip_space);  SDL_Event event;
  while (running == true)
  {
    while (SDL_PollEvent(&event))
    {
      switch (event.type)
      {
      case SDL_EVENT_QUIT:
        running = false;
        break;
      case SDL_EVENT_WINDOW_RESIZED:
      case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
        break;
      }
    }

    clean_zbuffer(z_buffer);
    
    put_pixel(frame_buffer,projected.x,projected.y,0x0000FF);

    SDL_UpdateTexture(texture, NULL, frame_buffer, WIDTH * sizeof(uint32_t));
    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);

  }
  return 0;
}

void clean_zbuffer(float *z_buffer)
{
  // the len of z buffer
  int len = WIDTH * HIEGHT;
  if (z_buffer == NULL)
  {
    return;
  }
  for (int i = 0; i < len; ++i)
  {
    z_buffer[i] = FLT_MAX;
  }
}
