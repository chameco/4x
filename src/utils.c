#include "utils.h"

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include <SDL2/SDL_image.h>

#include <cuttle/debug.h>
#include <cuttle/utils.h>

color COLOR_RED = {1.0, 0.0, 0.0, 1.0};
color COLOR_GREEN = {0.0, 1.0, 0.0, 1.0};
color COLOR_BLUE = {0.0, 0.0, 1.0, 1.0};
color COLOR_WHITE = {1.0, 1.0, 1.0, 1.0};
color COLOR_BLACK = {0.0, 0.0, 0.0, 1.0};
color COLOR_GRAY = {0.1, 0.1, 0.1, 1.0};

static GLuint STANDARD_INDICES[4] = {0, 1, 2, 3};
static GLuint STANDARD_INDICES_HANDLER = 0;

static vertex STANDARD_VERTICES[4] = {
	{0, 0, 0, 0},
	{1, 0, 1, 0},
	{1, 1, 1, 1},
	{0, 1, 0, 1}
};
static GLuint STANDARD_VERTICES_HANDLER = 0;

void initialize_utils()
{
	glGenBuffers(1, &STANDARD_INDICES_HANDLER);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, STANDARD_INDICES_HANDLER);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), STANDARD_INDICES, GL_STATIC_DRAW);

	glGenBuffers(1, &STANDARD_VERTICES_HANDLER);
	glBindBuffer(GL_ARRAY_BUFFER, STANDARD_VERTICES_HANDLER);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(vertex), STANDARD_VERTICES, GL_STATIC_DRAW);
}

GLuint get_standard_indices_handler()
{
	return STANDARD_INDICES_HANDLER;
}

GLuint get_standard_vertices_handler()
{
	return STANDARD_VERTICES_HANDLER;
}

bool check_collision(SDL_Rect A, SDL_Rect B)
{

	return !(((A.x >= B.x && A.x < B.x + B.w)  ||
				(B.x >= A.x && B.x < A.x + A.w)) && 
			((A.y >= B.y && A.y < B.y + B.h)   ||
			 (B.y >= A.y && B.y < A.y + A.h)));
}

double calculate_angle(double originx, double originy, double pointx, double pointy)
{
	double xdiff = pointx - originx;
	double ydiff = pointy - originy;
	double slope;
	double angle;
	if (xdiff == 0) {
		if (ydiff > 0) {
			slope = INFINITY;
		} else {
			slope = -INFINITY;
		}
	} else {
		slope = ydiff/xdiff;
	}
	angle = atan(slope);
	if (xdiff < 0) {
		angle += 3.141592654;
	}
	return angle;
}

line_point *bresenham_line(double x0, double y0, double x1, double y1)
{
	int temp;
	if (x1 < x0) {
		temp = x0;
		x0 = x1;
		x1 = temp;
	}
	if (y1 < y0) {
		temp = y0;
		y0 = y1;
		y1 = temp;
	}
	double xdiff = x1 - x0;
	double ydiff = y1 - y0;
	line_point *ret;
	int c = 0;
	if (xdiff >= ydiff) {
		double error = 0;
		double delta_error = fabs(ydiff/xdiff);
		ret = (line_point *) calloc(fabs(xdiff) + 1, sizeof(line_point));
		double y = y0;
		for (double x = x0; x <= x1; ++x, ++c) {
			ret[c].x = x;
			ret[c].y = y;
			error += delta_error;
			if (error > 0.5) {
				++y;
				--error;
			}
		}
	} else {
		double error = 0;
		double delta_error = fabs(xdiff/ydiff);
		ret = (line_point *) calloc(fabs(ydiff) + 1, sizeof(line_point));
		double x = x0;
		for (double y = y0; y <= y1; ++y, ++c) {
			ret[c].x = x;
			ret[c].y = y;
			error += delta_error;
			if (error > 0.5) {
				++x;
				--error;
			}
		}
	}
	ret[c].x = -1;
	return ret;
}

thunk make_thunk(void (*cfunc)())
{
	thunk ret;
	if (cfunc != NULL) {
		ret.type = THUNK_C;
		ret.data.cfunc = cfunc;
	} else {
		ret.type = THUNK_NULL;
	}
	return ret;
}

void execute_thunk(thunk cb)
{
	switch (cb.type) {
		case THUNK_NULL:
			break;
		case THUNK_C:
			cb.data.cfunc();
			break;
		default:
			log_err("Invalid callback type");
			exit(1);
			break;
	}
}

int max(int x, int y)
{
	return x > y ? x : y;
}

int min(int x, int y)
{
	return x < y ? x : y;
}

double dmax(double x, double y)
{
	return x > y ? x : y;
}

double dmin(double x, double y)
{
	return x < y ? x : y;
}
