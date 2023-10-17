#ifndef PARSING_H
#define PARSING_H
#include <locale.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int parsing_xyz(float **points, int **LINES, int *point_counter,
                int *lines_counter, char *filePath);
void string_to_XYZ(char *buffer, float *point_3D, float *maximum);
void string_to_line(char *buffer, int **LINES, int *lines_counter);
int points_and_lines_counter(int *point_counter, int *lines_counter,
                             char *filePath);
void normalaze(int point_counter, float **points);
void normalaze_XYZ_to_screen(int sum_points, float **points, float * maxXYZ, float * minXYX, float max);
//void normalaze_Y_to_screen(int sum_points, float **points);
//void normalaze_Z_to_screen(int sum_points, float **points);

#endif  // PARSING_H
