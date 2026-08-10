


/*
    Implement the perspective divide to convert clip-space vertices into normalized device coordinates, 
    then map them to screen coordinates in your framebuffer.
*/

#include "mat.h"
#include "vec.h"
#include "constant.h"
#include "sdl_init.h"

int main()
{
  vec4 point = {-1.0, -1.0, -1.0};
  vec3 eye =  {0,0,-3};
  vec3 target =  {0,0,-1};

  float scale = 1.0;
  matrix m_scaled = make_scaling_matrix(scale, scale,scale);
  matrix m_rotation = make_rotation_matrix(0, 0, 0);
  matrix m_tranlation = make_translation_matrix(0, 0, 0); 
  matrix model_matrix = mat_mul_mat(m_tranlation,mat_mul_mat(m_rotation,m_scaled));
  matrix view_matrix = make_view_matrix(eye,target); 
  matrix fin_matrix  = mat_mul_mat(view_matrix,model_matrix);
  vec4   view_space_point = matrix_mul_vec4(fin_matrix, point);
  matrix proj_matrix  = make_projection_matrix(WIDTH, HIEGHT, FOV, FAR_PLANE,NEAR_PLANE);
  vec4   clip_space   = matrix_mul_vec4(proj_matrix,view_space_point);


  float inw = 1.0 / clip_space.w;
  
  float ndcx = clip_space.x * inw;
  float ndcy = clip_space.y * inw;

  float screen_x = (ndcx * 0.5 + 0.5) * WIDTH;
  float screen_y = (-ndcy * 0.5 + 0.5) * HIEGHT;
  // we will test this another stream
  // we need to test till here
  // now we need to map them into the framebuffer
  
  create_sdl_window();
}
