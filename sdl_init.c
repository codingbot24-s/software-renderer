//
// Created by saad on 8/3/26.
//

// TODO: we can add aspect ratio and correct width and hieght in window opening
// TODO: check is ligh working correctly



#include "sdl_init.h"
#include "constant.h"
#include "draw.h"
#include "input.h"
#include "light.h"
#include "mesh.h"
#include "texture.h"
#include "vec.h"
#include <SDL3/SDL.h>
#include <float.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void clean_zbuffer(float *z_buffer);
void apply_transformation(mesh *mesh, matrix view_matrix);
void apply_transformation_normals(mesh *mesh, matrix view_matrix);

static bool running = true;

int create_sdl_window() {

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("sdl init failed %s\n", SDL_GetError());
    return -1;
  }
  // here using sdl window resizable will cancel
  // our width and hieght
  SDL_Window *window = SDL_CreateWindow("renderer window", WIDTH, HIEGHT, 0);
  if (!window) {
    printf("window creation error %s\n", SDL_GetError());
    return -1;
  }

  SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
  if (!renderer) {
    printf("renderer creation error %s\n", SDL_GetError());
    return -1;
  }

  SDL_SetWindowResizable(window, true);

  uint32_t *frame_buffer = create_framebuff();
  SDL_Texture *texture =
      SDL_CreateTexture(renderer, SDL_PIXELFORMAT_XRGB8888,
                        SDL_TEXTUREACCESS_STREAMING, WIDTH, HIEGHT);

  float *z_buffer = malloc(WIDTH * HIEGHT * sizeof(float));

  vec3 rotation = {0.0, 0.0, 0.0};
  vec3 translation = {0.0, 0.0, 0.0};
  //NOTE with huge object use the scale of 001
  float scale = 0.1;

  
  vec3 eye = (vec3){0.0, 0.0, -3.0};
  vec3 target = (vec3){0.0, 0.0, 0.0};

  matrix proj_matrix =
      make_projection_matrix(WIDTH, HIEGHT, FOV, FAR_PLANE, NEAR_PLANE);

  matrix l_v_m = make_view_matrix(eye, target);
  light l =
      make_light((vec3){0.0, 0.0, 0.0}, l_v_m, (vec3){0.0, 1.0, 0.0}, 1.0);
 //mesh *mesh =
 //    load_mesh("/home/saad/code/c/software-renderer/max-planck.obj");

  mesh *mesh = load_mesh("/home/saad/code/c/software-renderer/crab.obj");
  if (mesh == NULL) {
    fprintf(stderr, "Cant load the mesh \n");
    exit(EXIT_FAILURE);
  }
  struct texture loaded_texture = load_texture_from_file(
      "/home/saad/code/c/software-renderer/image/uv_checker_512.png");

  uint32_t light_color = 0x000000;
  uint32_t blue = 0x0000FF;
  uint32_t green = 0x00FF00;
  uint32_t red = 0xFF0000;

Uint64 last_time = SDL_GetPerformanceCounter();


  SDL_Event event;
  while (running == true) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_EVENT_QUIT:
        running = false;
        break;
      case SDL_EVENT_WINDOW_RESIZED:
      case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
        break;
      }
    }

    const Uint64 current_time = SDL_GetPerformanceCounter();
    const float delta_time = (float)(current_time - last_time)/(float)SDL_GetPerformanceFrequency();
        last_time = current_time;
    handle_input(&translation, &rotation,&scale, delta_time);

    
    matrix t_matrix = make_translation_matrix(translation.x,translation.y, translation.z);
    matrix r_matrix = make_rotation_matrix(rotation.x, rotation.y, rotation.z);
    matrix s_matrix = make_scaling_matrix(scale,scale,scale);
    matrix model_matrix =
        mat_mul_mat(t_matrix, mat_mul_mat(r_matrix, s_matrix));
    matrix view_matrix = make_view_matrix(eye, target);
    matrix model_view_matrix = mat_mul_mat(view_matrix, model_matrix);

    apply_transformation(mesh, model_view_matrix);
    apply_transformation_normals(mesh, model_view_matrix);
    clean_zbuffer(z_buffer);
    //
    // this will draw black full in every frame
    clear_framebuffer(frame_buffer, 0x000000);

    // draw_textured(mesh, proj_matrix, &loaded_texture, z_buffer,
    // frame_buffer);

     draw_textured_phong_shaded(mesh, proj_matrix, frame_buffer, z_buffer,
     0.2,
                                l, &loaded_texture);

    //draw_fill(mesh, proj_matrix, frame_buffer, z_buffer, red,
              //red,red);

    // draw_flatshaded(mesh, proj_matrix, frame_buffer, z_buffer, color1, 0.2,
    //                 light);

    // draw_phong_shaded(mesh, proj_matrix, frame_buffer, z_buffer, light_color,
    //                    0.2, l);

    // we will fix this
    SDL_UpdateTexture(texture, NULL, frame_buffer, WIDTH * sizeof(uint32_t));
    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
  }
  return 0;
}

void clean_zbuffer(float *z_buffer) {
  // the len of z buffer
  int len = WIDTH * HIEGHT;
  if (z_buffer == NULL) {
    return;
  }
  for (int i = 0; i < len; ++i) {
    z_buffer[i] = FLT_MAX;
  }
}

void apply_transformation_normals(mesh *mesh, matrix view_matrix) {
  vec3 *t_normals = mesh->transformed_normals->data;
  vec3 *normals = mesh->normals->data;
  for (int i = 0; i < mesh->normals_count; ++i) {

    t_normals[i] = matrix_mul_vec3(view_matrix, normals[i]);
  }
}

void apply_transformation(mesh *mesh, matrix view_matrix) {
  vec3 *transformed_vertices = mesh->transformed_vertices->data;
  vec3 *vertices = mesh->vertices->data;
  for (int i = 0; i < mesh->vert_count; ++i) {
    transformed_vertices[i] = matrix_mul_vec3(view_matrix, vertices[i]);
  }
}
