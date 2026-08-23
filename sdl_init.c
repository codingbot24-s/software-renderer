//
// Created by saad on 8/3/26.
//

// we need to solve the bug
#include "constant.h"
#include "draw.h"
#include "mesh.h"
#include "sdl_init.h"
#include "texture.h"
#include "vec.h"
#include <SDL3/SDL.h>
#include <float.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void clean_zbuffer(float *z_buffer);
void apply_transformation(vec3 *original_vert, int original_vert_count,
                          matrix view_matrix);

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

  float z_buffer[WIDTH * HIEGHT];

  vec3 rotation = {0.0, 0.0, 0.0};

  vec3 eye = (vec3){0.0, 0.0, -3.0};
  vec3 target = (vec3){0.0, 0.0, -1.0};

  matrix proj_matrix =
      make_projection_matrix(WIDTH, HIEGHT, FOV, FAR_PLANE, NEAR_PLANE);
  // triangle vertices
  uint32_t color1 = 0xFF0000;
  uint32_t color2 = 0x00FF00;
  uint32_t color3 = 0x0000FF;
  // TODO: remove all mallocs from here there should be no malloc in this means
  // whole lifetime
  //
  vec3 *t_vertices = malloc(3 * sizeof(vec3));
  t_vertices[0] = (vec3){-20, -15, 40};
  t_vertices[1] = (vec3){15, -10, 80};
  t_vertices[2] = (vec3){5, 20, 120};
  // vertex 0 = (-20, -15, 40)  → UV (0.0,0.0)
  // vertex 1 = ( 15, -10, 80)  → UV (1.0, 0.0)
  // vertex 2 = (  5,  20,120)  → UV (0.5, 1.0)

  vec2 *uvs = malloc(2 * sizeof(vec2));
  uvs[0] = (vec2){0.0, 0.0};
  uvs[1] = (vec2){1.0, 0.0};
  uvs[2] = (vec2){0.5, 1.0};

  face_t *triangles = malloc(sizeof(face_t));
  triangles->vertex_indices[0] = 0;
  triangles->vertex_indices[1] = 1;
  triangles->vertex_indices[2] = 2;

  triangles->uvs_indices[0] = 0;
  triangles->uvs_indices[1] = 1;
  triangles->uvs_indices[2] = 2;
  mesh *mesh = make_mesh(t_vertices, 3, uvs, 2, triangles, 1);
  struct texture loaded_texture = load_texture_from_file(
      "/home/saad/Projects/c/software-renderer/image/uv_checker_512.png");
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

    matrix t_matrix = make_translation_matrix(0.0, 0.0, 0.0);
    matrix r_matrix = make_rotation_matrix(rotation.x, rotation.y, rotation.z);
    matrix s_matrix = make_scaling_matrix(1.0, 1.0, 1.0);
    matrix model_matrix =
        mat_mul_mat(t_matrix, mat_mul_mat(r_matrix, s_matrix));
    matrix view_matrix = make_view_matrix(eye, target);
    matrix model_view_matrix = mat_mul_mat(view_matrix, model_matrix);

    // BUG: after uncommenting this line trinagle behave differently
    // apply_transformation(mesh->vertices, 3, model_view_matrix);

    clean_zbuffer(z_buffer);

    // this will draw black full in every frame
    clear_framebuffer(frame_buffer, 0x000000);
    draw_textured_triangle(mesh->vertices[0], mesh->vertices[1],
                           mesh->vertices[2], mesh->uvs[0], mesh->uvs[1],
                           mesh->uvs[2], &loaded_texture, frame_buffer);

    // rotation.x += 1.0;
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

void apply_transformation(vec3 *original_vert, int original_vert_count,
                          matrix view_matrix) {
  // NOTE: we changed from tt_vert to original vert
  //  check this works
  for (int i = 0; i < original_vert_count; ++i) {
    original_vert[i] = matrix_mul_vec3(view_matrix, original_vert[i]);
  }
}
