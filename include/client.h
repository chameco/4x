#pragma once

#include <stdbool.h>

#include <SDL2/SDL.h>

#include "utils.h"

void initialize_client();
void initGL();
SDL_Window *get_screen();
int get_screen_width();
int get_screen_height();
void take_screenshot(char *path);
void set_client_running(bool b);
void main_client_loop();
