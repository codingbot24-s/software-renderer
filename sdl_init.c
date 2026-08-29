//
// Created by saad on 8/3/26.
//

#include "sdl_init.h"
#include "constant.h"
#include "draw.h"
#include "light.h"
#include "mesh.h"
#include "texture.h"
#include "vec.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_gpu.h>
#include <float.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void clean_zbuffer(float *z_buffer);
void apply_transformation(mesh *mesh, matrix view_matrix);

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
  uint32_t color1 = 0x00FF00;
  uint32_t color2 = 0xFFFFFF;
  uint32_t color3 = 0xFFFFFF;
  // TODO: remove all mallocs from here there should be no malloc in this means
  // whole lifetime

  // TRIANGLE Vertices
  // vec3 *t_vertices = malloc(3 * sizeof(vec3));
  // t_vertices[0] = (vec3){-20, -15, 40};
  // t_vertices[1] = (vec3){15, -10, 80};
  // t_vertices[2] = (vec3){5, 20, 120};
  //  vertex 0 = (-20, -15, 40)  → UV (0.0,0.0)
  //  vertex 1 = ( 15, -10, 80)  → UV (1.0, 0.0)
  //  vertex 2 = (  5,  20,120)  → UV (0.5, 1.0)

  // vec2 *uvs = malloc(3 * sizeof(vec2));
  // uvs[0] = (vec2){0.0, 0.0};
  // uvs[1] = (vec2){1.0, 0.0};
  // uvs[2] = (vec2){0.5, 1.0};

  vec3 *c_vertices = malloc(8 * sizeof(vec3));
  c_vertices[0] = (vec3){-1.0, -1.0, -1.0};
  c_vertices[1] = (vec3){-1.0, 1.0, -1.0};
  c_vertices[2] = (vec3){1.0, 1.0, -1.0};
  c_vertices[3] = (vec3){1.0, -1.0, -1.0};
  c_vertices[4] = (vec3){1.0, 1.0, 1.0};
  c_vertices[5] = (vec3){1.0, -1.0, 1.0};
  c_vertices[6] = (vec3){-1.0, 1.0, 1.0};
  c_vertices[7] = (vec3){-1.0, -1.0, 1.0};

  vec2 *uvs = malloc(4 * sizeof(vec2));
  uvs[0] = (vec2){1.0, 1.0};
  uvs[1] = (vec2){1.0, 0.0};
  uvs[2] = (vec2){0.0, 0.0};
  uvs[3] = (vec2){0.0, 1.0};

  // normals for our hardcoded cube
  vec3 *normals = malloc(6 * sizeof(vec3));
  normals[0] = (vec3){0.0, 0.0, -1.0};
  normals[1] = (vec3){1.0, 0.0, 0.0};
  normals[2] = (vec3){0.0, 0.0, 1.0};
  normals[3] = (vec3){-1.0, 0.0, 0.0};
  normals[4] = (vec3){0.0, 1.0, 0.0};
  normals[5] = (vec3){0.0, -1.0, 0.0};

  // add indices for cubes
  face_t *triangles_indices = malloc(12 * sizeof(face_t));
  triangles_indices[0] = (face_t){.vertex_indices = {0, 1, 2},
                                  .uvs_indices = {0, 1, 2},
                                  .normal_indices = {0, 0, 0}};

  triangles_indices[1] = (face_t){
      .vertex_indices = {0, 2, 3},
      .uvs_indices = {0, 2, 3},
      .normal_indices =
          {
              0,
              0,
              0,
          },

  };

  triangles_indices[2] = (face_t){
      .vertex_indices = {3, 2, 4},
      .uvs_indices = {0, 1, 2},
      .normal_indices =
          {
              1,
              1,
              1,
          },
  };

  triangles_indices[3] = (face_t){.vertex_indices = {3, 4, 5},
                                  .uvs_indices = {0, 2, 3},
                                  .normal_indices = {
                                      1,
                                      1,
                                      1,
                                  }};

  triangles_indices[4] = (face_t){.vertex_indices = {5, 4, 6},
                                  .uvs_indices = {0, 1, 2},
                                  .normal_indices = {
                                      2,
                                      2,
                                      2,
                                  }};

  triangles_indices[5] = (face_t){.vertex_indices = {5, 6, 7},
                                  .uvs_indices = {0, 2, 3},
                                  .normal_indices = {
                                      2,
                                      2,
                                      2,
                                  }};

  triangles_indices[6] = (face_t){.vertex_indices = {7, 6, 1},
                                  .uvs_indices = {0, 1, 2},
                                  .normal_indices = {
                                      3,
                                      3,
                                      3,
                                  }};
  triangles_indices[7] = (face_t){.vertex_indices = {7, 1, 0},
                                  .uvs_indices = {0, 2, 3},
                                  .normal_indices = {3, 3, 3}};

  triangles_indices[8] = (face_t){.vertex_indices = {1, 6, 4},
                                  .uvs_indices = {0, 1, 2},
                                  .normal_indices = {4, 4, 4}};

  triangles_indices[9] = (face_t){.vertex_indices = {1, 4, 2},
                                  .uvs_indices = {0, 2, 3},
                                  .normal_indices = {4, 4, 4}};

  triangles_indices[10] = (face_t){.vertex_indices = {5, 7, 0},
                                   .uvs_indices = {0, 1, 2},
                                   .normal_indices = {
                                       5,
                                       5,
                                       5,
                                   }};
  triangles_indices[11] = (face_t){.vertex_indices = {5, 0, 3},
                                   .uvs_indices = {0, 2, 3},
                                   .normal_indices = {5, 5, 5}};

  mesh *mesh =
      make_mesh(c_vertices, 8, uvs, 4, triangles_indices, 12, 6, normals);
  struct texture loaded_texture = load_texture_from_file(
  
      "./image/uv_checker_512.png");

  light light = make_light((vec3){0.0,1.0,0.0,},1.0);

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
    apply_transformation(mesh, model_view_matrix);

    clean_zbuffer(z_buffer);

    // this will draw black full in every frame
    clear_framebuffer(frame_buffer, 0x000000);

    //  draw_textured(mesh, proj_matrix, &loaded_texture, z_buffer,
    // frame_buffer);

    draw_flatshaded(mesh, proj_matrix,frame_buffer,z_buffer,color1,0.2,light);
    rotation.z += 1.0;
    //draw_fill(mesh, proj_matrix, frame_buffer, z_buffer, color1, color2,
    //          color3);

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

void apply_transformation(mesh *mesh, matrix view_matrix) {
  // NOTE: we changed from tt_vert to original vert
  //  check this works

  // try changing this thing or search in other renderes how points been
  // transformed in camera space
  for (int i = 0; i < mesh->vert_count; ++i) {
    mesh->transformend_vertices[i] =
        matrix_mul_vec3(view_matrix, mesh->vertices[i]);
  }
}
