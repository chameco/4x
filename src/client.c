#include "client.h"

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
#include "network.h"
#include "texture.h"
#include "hex.h"
#include "map.h"

static SDL_Window *SCREEN;
static bool RUNNING = false;
//static const GLfloat CAMERA_SCALE = 1.0;
static int SCREEN_WIDTH = 0;
static int SCREEN_HEIGHT = 0;
static int LAST_TIME = 0;
static int CURRENT_TIME = 0;

static network_context CONTEXT;
static network_connection CONN;

void initialize_client()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		log_err("Failed to initialize SDL");    
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	if ((SCREEN = SDL_CreateWindow("fracture", 
					SDL_WINDOWPOS_UNDEFINED,
					SDL_WINDOWPOS_UNDEFINED,
					1600, 900,
					SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN))
			== NULL ) {
		log_err("Failed to initialize window");    
		exit(1);
	}

	SDL_MaximizeWindow(SCREEN);
	SDL_GL_CreateContext(SCREEN);

	SDL_DisplayMode dmode;
	if (SDL_GetWindowDisplayMode(SCREEN, &dmode) != 0) {
		log_err("Unable to fetch display mode");
		exit(1);
	}
	SCREEN_WIDTH = dmode.w;
	SCREEN_HEIGHT = dmode.h;
	
	initGL();
	IMG_Init(IMG_INIT_PNG);
	Mix_Init(MIX_INIT_OGG);
	Mix_OpenAudio(22050, AUDIO_S16, 2, 4096);

	CONTEXT = make_context();
	CONN = connect_to_server(CONTEXT, "127.0.0.1");

	initialize_utils();
	initialize_texture();
}

void initGL()
{
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK) {
		log_err("Initializing GLEW failed with the following error: %s", glewGetErrorString(glewError));
		exit(1);
	}

	if (!GLEW_VERSION_2_1) {
		log_err("OpenGL 2.1 not supported");
		exit(1);
	}

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClearColor(0, 0, 0, 0);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearStencil(0);
}

SDL_Window *get_screen()
{
	return SCREEN;
}

int get_screen_width()
{
	return SCREEN_WIDTH;
}

int get_screen_height()
{
	return SCREEN_HEIGHT;
}

void take_screenshot(char *path)
{
	SDL_Surface *image = SDL_CreateRGBSurface(SDL_SWSURFACE, SCREEN_WIDTH, SCREEN_HEIGHT, 24, 0x000000FF, 0x0000FF00, 0x00FF0000, 0);
	glReadBuffer(GL_FRONT);
	glReadPixels(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
	int index;
	void *temp_row;
	 
	temp_row = malloc(image->pitch);
	for (index = 0; index < image->h/2; index++) {
		memcpy((Uint8 *) temp_row, (Uint8 *) image->pixels + image->pitch * index, image->pitch);
		memcpy((Uint8 *) image->pixels + image->pitch * index, (Uint8 *) image->pixels + image->pitch * (image->h - index-1), image->pitch);
		memcpy((Uint8 *) image->pixels + image->pitch * (image->h - index-1), temp_row, image->pitch);
	}
	free(temp_row);
	IMG_SavePNG(image, path);
	SDL_FreeSurface(image);
}

void set_client_running(bool b)
{
	RUNNING = b;
}

void main_client_loop()
{
	//texture *t = load_texture("assets/textures/hex.png", 128, 128);
	map m;
	RUNNING = true;
	while (RUNNING) {
		SDL_Delay(5);
		CURRENT_TIME = SDL_GetTicks();
		if (CURRENT_TIME - LAST_TIME > 40) {
			pull_input(CONN, &m, sizeof(m));
			SDL_Event event;
			while (SDL_PollEvent(&event)) {
				switch (event.type) {
					case SDL_QUIT:
						RUNNING = false;
						break;
				}
			}
			LAST_TIME = CURRENT_TIME;
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		draw_map(&m);
		SDL_GL_SwapWindow(SCREEN);
	}
	SDL_Quit();
}

int main(int argc, char *argv[])
{
	initialize_client();
	main_client_loop();
}
