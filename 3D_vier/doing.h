#ifndef DOING_H
#define DOING_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void rotate(float *xyz, float *result, float *XY_rote, int sum_points,
            int flag_perspective, float perspective_point_posion_z, int **LINES,
            int sum_lines);
void multm_matrix(float *A, int M, float *B, float *C);
void cut_to_screen_z(float *xyz, float perspective_point_posion_z,
                     int sum_lines, int **LINES);

#endif  // DOING_H
