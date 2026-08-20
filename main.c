

/*
  TODO1:Implement a texture system
  that can load an image into CPU memory and sample a texel using UV
  coordinates.

  TODO2:Assign UV coordinates
  to your triangle's vertices and render a textured triangle using your
  texture sampler.

  TODO3 :Apply texture mapping to your rotating 3D cube,
  with each cube face displaying the appropriate region of the texture.

  TODO4 :Implement texture coordinate wrapping and clamping,
  and verify that both behaviors work correctly when UV coordinates extend
  outside the [0, 1] range.
*/
#include "sdl_init.h"
#include "texture.h"

int main() {
  texture texture = load_texture_from_file(
      "/home/saad/code/c/software-renderer/image/uv_checker_512.png");
  create_sdl_window();
}
