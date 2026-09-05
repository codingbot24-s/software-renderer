

// NOTE: currently our renderer can only load a faces in this format no other
// format f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3 and f v1 v2 v3
// one way is to use the existing loader so we can load any model
// because some model we are testing dont have normals we cant use phong shading
// on those we will get the crash if we try to use them with phong shaded

// TODO: add input for making model big and small and moving camera first

/*
  4 add material and texture support to
  your OBJ model loader, then render the model using its own textures instead of
  a hardcoded texture.

  5. Implement a movable 3D camera with position and orientation controls, and
  use it to freely move around and inspect your rendered model in real time.
*/

#include "sdl_init.h"

int main() { create_sdl_window(); }
