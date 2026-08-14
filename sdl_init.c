//
// Created by saad on 8/3/26.
//

// we need to solve the bug 
#include "sdl_init.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_render.h>
#include <stdint.h>
#include <stdio.h>
#include "constant.h"
#include "draw.h"
#include "mesh.h"
#include "vec.h"
#include <stdlib.h>
#include <float.h>

void clean_zbuffer(float *z_buffer);
void apply_transformation(vec3* original_vert,
                          vec3* transformed_vert, 
                          int   original_vert_count,
                          matrix view_matrix
                        );

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

  vec3 rotation = {0.0,0.0,0.0};
     
  vec3 eye = (vec3){0.0,0.0,-3.0};
  vec3 target = (vec3){0.0,0.0,-1.0};
  
  mesh cube = make_cube();
  uint32_t color = 0x0000FF;
  matrix proj_matrix = make_projection_matrix(WIDTH, HIEGHT, FOV,FAR_PLANE,NEAR_PLANE);

  vec3 a;
  a.x = 0;
  a.y = 10;
  a.z = 0;

  vec3 b;
  b.x = -8;
  b.y = -6;
  b.z = 0;

  vec3 c;
  c.x = 8;
  c.y = -6;
  c.y = 0;
    
  SDL_Event event;
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

      
    matrix t_matrix = make_translation_matrix(0.0, 0.0, 0.0);
    matrix r_matrix = make_rotation_matrix(rotation.x, rotation.y,rotation.z);
    matrix s_matrix = make_scaling_matrix(1.0,1.0,1.0);
    matrix model_matrix = mat_mul_mat(t_matrix, mat_mul_mat(r_matrix,s_matrix));
    matrix view_matrix = make_view_matrix(eye,target);
    matrix model_view_matrix = mat_mul_mat(view_matrix,model_matrix);
   
    apply_transformation(cube.vertices, cube.transformed_vertices, cube.vert_count, model_view_matrix);
    clean_zbuffer(z_buffer);

    // this will draw black full in every frame
    clear_framebuffer(frame_buffer,0x000000);
    // we just need to draw traingle with our new function 
    
    // draw_wireframe(&cube, color, proj_matrix, frame_buffer);
    
    // rotation.x += 1.0;

    fill_triangle(a,  b, c, frame_buffer, color,z_buffer);    
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

void apply_transformation(vec3* original_vert,
                          vec3* transformed_vert, 
                          int   original_vert_count,
                          matrix view_matrix
                        ) 
{
  for (int i = 0; i < original_vert_count; ++i) 
  {
    transformed_vert[i] = matrix_mul_vec3(view_matrix,original_vert[i]);
  }
}
