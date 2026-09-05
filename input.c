
#include "input.h"
#include <SDL3/SDL_keyboard.h>
#include <stdbool.h>
#include <stdio.h>

void handle_input(vec3* translation,vec3* rotation, float* scale, float delta_time) {
  const bool* key_state = SDL_GetKeyboardState(NULL);
  const float linear_step = (key_state[SDL_SCANCODE_LSHIFT] != 0) ? 0.25f * delta_time : 1.0f * delta_time;
    const float angular_step = (key_state[SDL_SCANCODE_LSHIFT] != 0) ? 24.0f * delta_time : 48.0f * delta_time;

    if (key_state[SDL_SCANCODE_W]) {
      translation->z += linear_step;
    }
    if (key_state[SDL_SCANCODE_S]) translation->z -= linear_step;
    if (key_state[SDL_SCANCODE_A]) translation->x += linear_step;
    if (key_state[SDL_SCANCODE_D]) translation->x -= linear_step;
    if (key_state[SDL_SCANCODE_E]) translation->y += linear_step;
    if (key_state[SDL_SCANCODE_Q]) translation->y -= linear_step;

    if (key_state[SDL_SCANCODE_J]) rotation->x += angular_step;
    if (key_state[SDL_SCANCODE_L]) rotation->x -= angular_step;
    if (key_state[SDL_SCANCODE_O]) rotation->y += angular_step;
    if (key_state[SDL_SCANCODE_U]) rotation->y -= angular_step;
    if (key_state[SDL_SCANCODE_I]) rotation->z -= angular_step;
    if (key_state[SDL_SCANCODE_K]) rotation->z += angular_step;

    if (key_state[SDL_SCANCODE_KP_PLUS]) *scale += linear_step;
    if (key_state[SDL_SCANCODE_KP_MINUS]) *scale -= linear_step;
}
