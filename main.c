


/*
  TODO:Implement a texture system 
  that can load an image into CPU memory and sample a texel using UV coordinates.
*/

#include "sdl_init.h"
#include "texture.h"


int main()
{
  texture texture = load_texture_from_file("./image/uv_checker_512.png");
   create_sdl_window();
}
