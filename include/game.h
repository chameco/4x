#pragma once

#include <stdbool.h>

#include <SDL2/SDL.h>

#include "utils.h"

void initialize_game();
void initGL();
SDL_Window *get_screen();
int get_screen_width();
int get_screen_height();
void define_mode(char *name, thunk init, thunk update, thunk draw);
void set_current_dialog(char *text);
char *get_current_dialog();
void take_screenshot(char *path);
void set_running(bool b);
void main_game_loop();
