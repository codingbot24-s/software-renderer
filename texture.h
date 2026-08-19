#pragma once

typedef struct texture {
  int width;
  int height;
  //TODO: store pixel here
} texture;


texture load_texture_from_file(char* filename);
