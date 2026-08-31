
/*
  1 Create a general-purpose 3D mesh representation that stores vertices
  (position, normal, UV) and indexed triangles, independent of your cube.
  2 Implement an OBJ file loader that reads vertex positions, texture
  coordinates, normals, and face indices and converts them into your mesh
  representation.
  3 Load a real OBJ model at runtime and render it through your
  complete software-rendering pipeline.
  4 add material and texture support to
  your OBJ model loader, then render the model using its own textures instead of
  a hardcoded texture.

  5. Implement a movable 3D camera with position and orientation controls, and
  use it to freely move around and inspect your rendered model in real time.
*/

#include "sdl_init.h"

int main() { create_sdl_window(); }
