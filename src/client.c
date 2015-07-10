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
#include "camera.h"
#include "texture.h"
#include "hex.h"
#include "map.h"
#include "action.h"

static SDL_Window *SCREEN;
static bool RUNNING = false;
static int SCREEN_WIDTH = 0;
static int SCREEN_HEIGHT = 0;
static int LAST_TIME = 0;
static int CURRENT_TIME = 0;

static network_context CONTEXT;
static network_connection CONN;

static entity_selector CURRENTLY_SELECTED = {0, 0, 0};
static texture *CURSOR;

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
	initialize_camera(SCREEN_WIDTH, SCREEN_HEIGHT);

	CURSOR = load_texture("assets/textures/cursor.png", HEX_DIM, HEX_DIM);
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

	glClearColor(0, 0, 0.2, 0);

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
	map m;
	action_message login = {{0, 0, 0}, ACTION_LOGIN, 0};
	push_output(CONN, &login, sizeof(login));
	pull_input(CONN, &m, sizeof(m));
	RUNNING = true;
	while (RUNNING) {
		SDL_Delay(20);
		CURRENT_TIME = SDL_GetTicks();
		if (CURRENT_TIME - LAST_TIME > 40) {
			pull_input_noblock(CONN, &m, sizeof(m));
			SDL_Event event;
			while (SDL_PollEvent(&event)) {
				switch (event.type) {
					case SDL_QUIT:
						RUNNING = false;
						break;
					case SDL_KEYDOWN:
						if (event.key.keysym.sym == SDLK_LEFT) {
							move_camera(get_camera_x() - 16, get_camera_y());
						} else if (event.key.keysym.sym == SDLK_RIGHT) {
							move_camera(get_camera_x() + 16, get_camera_y());
						} else if (event.key.keysym.sym == SDLK_UP) {
							move_camera(get_camera_x(), get_camera_y() - 16);
						} else if (event.key.keysym.sym == SDLK_DOWN) {
							move_camera(get_camera_x(), get_camera_y() + 16);
						}
				}
			}
			LAST_TIME = CURRENT_TIME;
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		translate_camera();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		draw_map(&m);
		draw_texture(CURSOR, CURRENTLY_SELECTED.x*(HEX_DIM-32), CURRENTLY_SELECTED.y*(HEX_DIM-17) + (CURRENTLY_SELECTED.x % 2 == 0 ? 0 : 64 - 8));

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		SDL_GL_SwapWindow(SCREEN);
	}
	SDL_Quit();
}

int main(int argc, char *argv[])
{
	initialize_client();
	main_client_loop();
}
