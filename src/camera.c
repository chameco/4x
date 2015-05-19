#include "camera.h"

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#include <cuttle/debug.h>
#include <cuttle/utils.h>

#include "utils.h"

static int SCREEN_WIDTH;
static int SCREEN_HEIGHT;

static double CAMERA_X = 0;
static double CAMERA_Y = 0;

static double CAMERA_SCALE = 1.0;

void initialize_camera(int w, int h)
{
	SCREEN_WIDTH = w;
	SCREEN_HEIGHT = h;
}

int get_screen_width()
{
	return SCREEN_WIDTH;
}

int get_screen_height()
{
	return SCREEN_HEIGHT;
}

void move_camera(double x, double y)
{
	CAMERA_X = x;
	CAMERA_Y = y;
}

double get_camera_x()
{
	return CAMERA_X;
}

double get_camera_y()
{
	return CAMERA_Y;
}

void set_camera_scale(double s)
{
	CAMERA_SCALE = s;
}

void translate_camera()
{
	glTranslatef(SCREEN_WIDTH/2 - CAMERA_X * CAMERA_SCALE, SCREEN_HEIGHT/2 - CAMERA_Y * CAMERA_SCALE, 0);
	glScalef(CAMERA_SCALE, CAMERA_SCALE, 1.0);
}
