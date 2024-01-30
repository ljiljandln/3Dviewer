#include "s21_3dviewer.h"

void shift(data_t *data, float shift, int i, int coordinate) {
  data->matrix_3d.matrix[i][coordinate] += shift;
}

void rotate(data_t *data, float angle, int i, int coordinate) {
  int coordinate1 = (coordinate + 1) % 3;
  int coordinate2 = (coordinate + 2) % 3;
  float value1 = data->matrix_3d.matrix[i][coordinate1];
  float value2 = data->matrix_3d.matrix[i][coordinate2];
  data->matrix_3d.matrix[i][coordinate1] =
      value1 * cos(angle) - value2 * sin(angle);
  data->matrix_3d.matrix[i][coordinate2] =
      value1 * sin(angle) + value2 * cos(angle);
}

void scale(data_t *data, float scale, int i, int coordinate) {
  data->matrix_3d.matrix[i][coordinate] *= scale;
}

void getShiftCoord(data_t data, float *coord) {
  for (int i = X; i <= Z; i++) {
    coord[i] =
        data.matrix_3d.minMax[i][MIN] +
        (data.matrix_3d.minMax[i][MAX] - data.matrix_3d.minMax[i][MIN]) / 2;
  }
}

float getScale(data_t data) {
  float dmax = data.matrix_3d.minMax[X][MAX] - data.matrix_3d.minMax[X][MIN];
  for (int i = Y; i <= Z; i++) {
    float onStep =
        data.matrix_3d.minMax[i][MAX] - data.matrix_3d.minMax[i][MIN];
    if (onStep > dmax) dmax = onStep;
  }
  float scal = 1 / dmax;
  return scal;
}

void puttingInOrder(data_t *data) {
  float coord[3] = {0};
  getShiftCoord(*data, coord);
  float scal = getScale(*data);
  transform_t tr = {-coord[X], -coord[Y], -coord[Z], scal, scal, scal, 0, 0, 0};
  allTransforms(data, tr);
}

int isTransformNotNull(transform_t tr) {
  return (tr.xScale != 0 && tr.xScale != 1) ||
         (tr.yScale != 0 && tr.yScale != 1) ||
         (tr.zScale != 0 && tr.zScale != 1) || tr.xShift != 0 ||
         tr.yShift != 0 || tr.zShift != 0 || tr.xAngle != 0 || tr.yAngle != 0 ||
         tr.zAngle != 0;
}

void changeAnglesToRad(transform_t *tr) {
  float coef = M_PI / 180;
  tr->xAngle *= coef;
  tr->yAngle *= coef;
  tr->zAngle *= coef;
}

void allTransforms(data_t *data, transform_t tr) {
  if (isTransformNotNull(tr)) {
    if (tr.xAngle != 0 || tr.yAngle != 0 || tr.zAngle != 0)
      changeAnglesToRad(&tr);
    for (int i = 0; i < data->number_of_vertexes; i++) {
      if (tr.xShift != 0) shift(data, tr.xShift, i, X);
      if (tr.yShift != 0) shift(data, tr.yShift, i, Y);
      if (tr.zShift != 0) shift(data, tr.zShift, i, Z);
      if (tr.xScale != 0 && tr.xScale != 1) scale(data, tr.xScale, i, X);
      if (tr.yScale != 0 && tr.yScale != 1) scale(data, tr.yScale, i, Y);
      if (tr.zScale != 0 && tr.zScale != 1) scale(data, tr.zScale, i, Z);
      if (tr.xAngle != 0) rotate(data, tr.xAngle, i, X);
      if (tr.yAngle != 0) rotate(data, tr.yAngle, i, Y);
      if (tr.zAngle != 0) rotate(data, tr.zAngle, i, Z);
    }
  }
}
