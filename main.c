


// in the previous stream there was a 2 to 3 bug
// one was incorrect rgb values in fill_triangle
// 2 was the points that we were given from scratch pixel
// that was already in screen space thats why our pipeline
// wasnt correctly working on that
// 3 we need to do project to screen in triangle if we are not using wireframe function because the nexts step requires point in the screen space for working 

/*
  TODO:Add perspective-correct interpolation for per-vertex attributes across your triangles, 
  and verify it using a smoothly varying vertex color across a 3D triangle.
*/

/*
  TODO:Implement a texture system 
  that can load an image into CPU memory and sample a texel using UV coordinates.
*/

#include "sdl_init.h"

int main()
{
  create_sdl_window();
}
