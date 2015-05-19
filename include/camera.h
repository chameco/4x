#pragma once

void initialize_camera(int w, int h);

int get_screen_width();
int get_screen_height();

void move_camera(double x, double y);
double get_camera_x();
double get_camera_y();
void translate_camera();
