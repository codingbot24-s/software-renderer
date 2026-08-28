

/*
  1. Add a normal vector to every 3D vertex and ensure your mesh pipeline
  transforms normals correctly when the model is transformed.
  2. Implement a basic directional-light calculation using the surface normal
  and a configurable light direction, producing a lighting intensity for each
  rendered surface.
  3. Integrate the lighting calculation into your triangle rasterizer so the
  final pixel color is modulated by the interpolated surface lighting across
  each triangle.
  4. Render your rotating textured cube with both texture mapping and
  directional lighting enabled simultaneously, with depth testing and back-face
  culling still active.
*/

#include "sdl_init.h"
int main() { create_sdl_window(); }
