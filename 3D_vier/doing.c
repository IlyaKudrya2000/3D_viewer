#include "doing.h"

void rotate(float *xyz, float *result, float *XY_rote, int sum_points,
            int flag_perspective, float perspective_point_posion_z, int **LINES,
            int sum_lines) {
  float *XYZRot = calloc(4 * 4, sizeof(float));
  float *TPM = calloc(4 * 4, sizeof(float));
  float *Prom_result = calloc(sum_points * 4, sizeof(float));
  float englY = XY_rote[1] * 2;
  float englX = XY_rote[0] * 2;
  float englZ = XY_rote[2] * 2 + M_PI; // все модели изначально перевёрнуты на 180 градусов

  for (int i = 0; i < 4 * 4; i++) {
    XYZRot[i] = 0;
    TPM[i] = 0;
  }

  XYZRot[0] = cos(englZ) * cos(englY);
  XYZRot[1] = -sin(englZ) * cos(englY);
  XYZRot[2] = sin(englY);
  XYZRot[4] = sin(englY) * sin(englX) * cos(englZ) + sin(englZ) * cos(englX);
  XYZRot[5] = -sin(englY) * sin(englX) * sin(englZ) + cos(englZ) * cos(englX);
  XYZRot[6] = -sin(englX) * cos(englY);
  XYZRot[8] = sin(englX) * sin(englZ) - sin(englY) * cos(englX) * cos(englZ);
  XYZRot[9] = sin(englX) * cos(englZ) + sin(englY) * sin(englZ) * cos(englX);
  XYZRot[10] = cos(englX) * cos(englY);
  XYZRot[15] = 1;

  TPM[0 * 4 + 0] = 1;
  TPM[1 * 4 + 1] = 1;
  TPM[3 * 4 + 3] = 1;
  TPM[2 * 4 + 3] = -1 / perspective_point_posion_z;

  for (int i = 0; i < sum_points; i++) {
    xyz[i * 4 + 3] = 1;
    result[i * 4 + 3] = 1;
  }

  multm_matrix(xyz, sum_points, XYZRot, result);

  if (flag_perspective == 1) {
    cut_to_screen_z(result, perspective_point_posion_z, sum_lines, LINES);
    multm_matrix(result, sum_points, TPM, Prom_result);

    for (int i = 0; i < sum_points; i++) {
      for (int j = 0; j < 4; j++) {
        result[i * 4 + j] = Prom_result[i * 4 + j] / Prom_result[i * 4 + 3];
      }
    }
  }
  free(Prom_result);
  free(XYZRot);
  free(TPM);
}
void cut_to_screen_z(float *xyz, float perspective_point_posion_z,
                     int sum_lines, int **LINES) {
  float a = perspective_point_posion_z;
  for (int i = 0; i < sum_lines; i++) {
    if (xyz[(LINES[i][0] - 1) * 4 + 2] > a &&
        xyz[(LINES[i][1] - 1) * 4 + 2] > a) {
      xyz[(LINES[i][0] - 1) * 4] = 0;
      xyz[(LINES[i][0] - 1) * 4 + 1] = 0;
      xyz[(LINES[i][1] - 1) * 4] = 0;
      xyz[(LINES[i][1] - 1) * 4 + 1] = 0;
    } else if (xyz[(LINES[i][0] - 1) * 4 + 2] < a &&
               xyz[(LINES[i][1] - 1) * 4 + 2] > a) {
      xyz[(LINES[i][1] - 1) * 4 + 2] = a;
    } else if (xyz[(LINES[i][0] - 1) * 4 + 2] > a &&
               xyz[(LINES[i][1] - 1) * 4 + 2] < a) {
      xyz[(LINES[i][0] - 1) * 4 + 2] = a;
    }
    //          ; xyz[(LINES[i][1] - 1) * 4 + 2];
  }
}
void multm_matrix(float *A, int M, float *B, float *C) {
  float a0 = B[0 * 4 + 0];
  float a1 = B[1 * 4 + 0];
  float a2 = B[2 * 4 + 0];
  float a3 = B[3 * 4 + 0];

  float a4 = B[0 * 4 + 1];
  float a5 = B[1 * 4 + 1];
  float a6 = B[2 * 4 + 1];
  float a7 = B[3 * 4 + 1];

  float a8 = B[0 * 4 + 2];
  float a9 = B[1 * 4 + 2];
  float a10 = B[2 * 4 + 2];
  float a11 = B[3 * 4 + 2];

  float a12 = B[0 * 4 + 3];
  float a13 = B[1 * 4 + 3];
  float a14 = B[2 * 4 + 3];
  float a15 = B[3 * 4 + 3];
  for (int i = 0; i < M; ++i) {
    C[i * 4 + 0] = 0;
    C[i * 4 + 0] = 4 * C[i * 4 + 0] + A[i * 4 + 0] * a0 + A[i * 4 + 1] * a1 +
                   A[i * 4 + 2] * a2 + A[i * 4 + 3] * a3;
    //                C[i*4 + 0] = C[i*4 + 0] + A[i*4 + 1] * a1;
    //                C[i*4 + 0] = C[i*4 + 0] + A[i*4 + 2] * a2;
    //                C[i*4 + 0] = C[i*4 + 0] + A[i*4 + 3] * a3;

    C[i * 4 + 1] = 0;
    C[i * 4 + 1] = 4 * C[i * 4 + 1] + A[i * 4 + 0] * a4 + A[i * 4 + 1] * a5 +
                   A[i * 4 + 2] * a6 + A[i * 4 + 3] * a7;
    //                C[i*4 + 1] = C[i*4 + 1] + A[i*4 + 1] * a5;
    //                C[i*4 + 1] = C[i*4 + 1] + A[i*4 + 2] * a6;
    //                C[i*4 + 1] = C[i*4 + 1] + A[i*4 + 3] * a7;

    C[i * 4 + 2] = 0;
    C[i * 4 + 2] = 4 * C[i * 4 + 2] + A[i * 4 + 0] * a8 + A[i * 4 + 1] * a9 +
                   A[i * 4 + 2] * a10 + A[i * 4 + 3] * a11;
    //                C[i*4 + 2] = C[i*4 + 2] + A[i*4 + 1] * a9;
    //                C[i*4 + 2] = C[i*4 + 2] + A[i*4 + 2] * a10;
    //                C[i*4 + 2] = C[i*4 + 2] + A[i*4 + 3] * a11;

    C[i * 4 + 3] = 0;
    C[i * 4 + 3] = 4 * C[i * 4 + 3] + A[i * 4 + 0] * a12 + A[i * 4 + 1] * a13 +
                   A[i * 4 + 2] * a14 + A[i * 4 + 3] * a15;
    //                C[i*4 + 3] = C[i*4 + 3] + A[i*4 + 1] * a13;
    //                C[i*4 + 3] = C[i*4 + 3] + A[i*4 + 2] * a14;
    //                C[i*4 + 3] = C[i*4 + 3] + A[i*4 + 3] * a15;
  }
}
