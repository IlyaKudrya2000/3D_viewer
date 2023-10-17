#include "parsing.h"

int parsing_xyz(float **points, int **LINES, int *point_counter,
                int *lines_counter, char *filePath) {
  int error = 0;
  char *buffer = calloc(256, sizeof(char));
  float maximum = 0;
  FILE *fp = fopen(filePath, "r");
  if (fp) {
    if (*point_counter != 0 || *lines_counter != 0) {
      *point_counter = 0;
      *lines_counter = 0;
    }
    while ((fgets(buffer, 256, fp)) != NULL) {
      if (buffer[0] == 'v' && buffer[1] == ' ') {
        string_to_XYZ(buffer, points[*point_counter], &maximum);
        *point_counter = *point_counter + 1;
      } else if (buffer[0] == 'f' && buffer[1] == ' ') {
        string_to_line(buffer, LINES, lines_counter);
      }
    }
    normalaze(*point_counter, points);
    fclose(fp);

  } else
    error = 1;
  free(buffer);
  if (!error && (*point_counter == 0 || *lines_counter == 0)) error = 2;
  return error;
}
void normalaze_XYZ_to_screen(int sum_points, float **points, float * maxXYZ, float * minXYZ, float max){
    float X_average = (maxXYZ[0] + minXYZ[0]) / 2;
    float Y_average = (maxXYZ[1] + minXYZ[1]) / 2;
    float Z_average = (maxXYZ[2] + minXYZ[2]) / 2;
    for(int i = 0; i < sum_points; i++){
        //X_average - 4
        points[i][0] = 4.4 * (points[i][0] -  X_average) / max;
        points[i][1] = 4.4 * (points[i][1] -  Y_average) / max;
        points[i][2] = 4.4 * (points[i][2] -  Z_average) / max;
//        printf("X %f\n", points[i][0]);
//        printf("Y %f\n", points[i][1]);
//        printf("Z %f\n", points[i][2]);
    }
}
void normalaze(int sum_points, float **points){
    //  800X, 600Y
    float max = 0;
    float maxXYZ[3] = {0,0,0};

    float min = 0;
    float minXYZ [3] = {0,0,0};

    for(int i = 0; i< sum_points; i++){
        if(points[i][0] > maxXYZ[0]){
            maxXYZ[0] = points[i][0];
        }
        if(points[i][1] > maxXYZ[1]){
            maxXYZ[1] = points[i][1];
        }
        if(points[i][2] > maxXYZ[2]){
            maxXYZ[2] = points[i][2];
        }
        if(points[i][0] < minXYZ[0]){
            minXYZ[0] = points[i][0];
        }
        if(points[i][0] < minXYZ[1]){
            minXYZ[1] = points[i][1];
        }
        if(points[i][0] < minXYZ[2]){
            minXYZ[2] = points[i][2];
        }
        for(int j = 0 ; j < 3; j ++){
            if(points[i][j] > max){
                max = points[i][j];
            }
            if(points[i][j] < min){
                min = points[i][j];
            }

        }
    }
    if(max != 0){
//        for(int i = 0; i< sum_points; i++){
//            for(int j = 0 ; j < 3; j ++){
//                    //points[i][j] = (points[i][j] / max) * 4.4;
//            }
//        }

    normalaze_XYZ_to_screen(sum_points, points, maxXYZ, minXYZ , max);
    //printf("%f\n", max);
    }
}
void string_to_XYZ(char *buffer, float *points, float *maximum) {
  char *tmp = calloc(256, sizeof(char));
  char dot = '.';
  if (atof("1,1") == 1.1) {
    dot = ',';
  }
  for (int i = 0, k = 0, j = 0; j < 3; i++) {
    if ((buffer[i] <= 57 && buffer[i] >= 48) || buffer[i] == 46 ||
        buffer[i] == '-') {
      if (buffer[i] == 46) {
        buffer[i] = dot;
      } else if (buffer[i] == '-') {
        buffer[i] = '-';
      }
      tmp[k] = buffer[i];
      tmp[k + 1] = '\0';
      k++;
    } else if (tmp[0] != '\0') {
      points[j] = atof(tmp);
      if (fabsf(points[j]) > fabsf(*maximum)) *maximum = points[j];
      j++;
      tmp[0] = '\0';
      k = 0;
    }
  }
  free(tmp);
}

void string_to_line(char *buffer, int **LINES, int *lines_counter) {
  char *tmp = calloc(256, sizeof(char));
  tmp[0] = '\0';
  int j = 0;
  int tmp_number = 0;
  int i, k;
  int skip_check = 0;
  for (i = 1, k = 0; buffer[i] != '\n' && buffer[i] != '\0'; i++) {
    if (!skip_check && buffer[i] >= '0' && buffer[i] <= '9') {
      tmp[k] = buffer[i];
      tmp[k + 1] = '\0';
      k++;
    } else if (buffer[i] == ' ') {
      if (tmp[0] != '\0') {
        if (tmp_number == 0) {
          LINES[*lines_counter][0] = atoi(tmp);
          tmp_number = LINES[*lines_counter][0];
          j = LINES[*lines_counter][0];
          k = 0;
          tmp[0] = '\0';
        } else {
          tmp_number = atoi(tmp);
          LINES[*lines_counter + 1][0] = tmp_number;
          LINES[*lines_counter][1] = tmp_number;
          *lines_counter = *lines_counter + 1;
          k = 0;
          tmp[0] = '\0';
        }
      }
      skip_check = 0;
    } else {
      skip_check = 1;
    }
  }
  if (buffer[i] == '\n' || buffer[i] == '\0') {
    if (tmp[0] != '\0') {
      tmp_number = atoi(tmp);
      LINES[*lines_counter + 1][0] = tmp_number;
      LINES[*lines_counter][1] = tmp_number;
      *lines_counter = *lines_counter + 1;
    }
    if (*lines_counter > 0) {
      LINES[*lines_counter][1] = j;
      *lines_counter = *lines_counter + 1;
    }
  }
  free(tmp);
}

int points_and_lines_counter(int *point_counter, int *lines_counter,
                             char *filePath) {
  int error = 0;
  char *buffer = calloc(256, sizeof(char));
  FILE *fp = fopen(filePath, "r");
  if (fp) {
    if (*point_counter != 0 || *lines_counter != 0) {
      *point_counter = 0;
      *lines_counter = 0;
    }
    while ((fgets(buffer, 256, fp)) != NULL) {
      if (buffer[0] == 'v' && buffer[1] == ' ') {
        *point_counter = *point_counter + 1;
      }
      if (buffer[0] == 'f' && buffer[1] == ' ') {
        for (int i = 0;
             buffer[i + 1] != '\n' && buffer[i + 1] != '\0' && i < 255; i++) {
          if (buffer[i] == ' ' &&
              (buffer[i + 1] <= 57 && buffer[i + 1] >= 48)) {
            *lines_counter = *lines_counter + 1;
          }
        }
      }
    }
    fclose(fp);
  } else
    error = 1;
  free(buffer);
  if (!error && (*point_counter == 0 || *lines_counter == 0)) error = 2;
  return error;
}
