#include "s21_3dviewer.h"

void init_data(data_t* data) {
  data->number_of_vertexes = 0;
  data->number_of_facets = 0;
  data->polygons = calloc(INIT_SIZE, sizeof(polygon_t));
  data->storageSize = INIT_SIZE;
  data->matrix_3d.storageSize = INIT_SIZE;
  data->matrix_3d.matrix = calloc(INIT_SIZE, sizeof(float*));
}

data_t read_obj_file(char* file_path) {
  data_t data = {0};
  init_data(&data);
  parser(file_path, &data);
  puttingInOrder(&data);
  return data;
}

void clearData(data_t* data) {
  for (int i = 0; i < data->matrix_3d.rows; i++) {
    free(data->matrix_3d.matrix[i]);
  }
  free(data->matrix_3d.matrix);
  for (int i = 0; i < data->number_of_facets; i++) {
    free(data->polygons[i].vertexes);
  }
  free(data->polygons);
}

void parser(char* file_path, data_t* data) {
  FILE* f = fopen(file_path, "r");
  char line[1024] = {0};
  int currentVertex = -1, currentPolygon = -1;

  while (fgets(line, 1024, f) != NULL) {
    if (line[0] == 'v') {
      add_vertex(data, line, &currentVertex);
    } else if (line[0] == 'f') {
      add_polygon(data, line, ++currentPolygon);
    }
  }
  fclose(f);
}

void checkMinMax(data_t* data, float x, float y, float z) {
  float cord[3] = {x, y, z};
  if (data->number_of_vertexes == 1) {
    for (int i = X; i <= Z; i++) {
      data->matrix_3d.minMax[i][MIN] = cord[i];
      data->matrix_3d.minMax[i][MAX] = cord[i];
    }
  } else {
    for (int i = X; i <= Z; i++) {
      if (cord[i] < data->matrix_3d.minMax[i][MIN])
        data->matrix_3d.minMax[i][MIN] = cord[i];
      if (cord[i] > data->matrix_3d.minMax[i][MAX])
        data->matrix_3d.minMax[i][MAX] = cord[i];
    }
  }
}

void changeMatrix(data_t* data) {
  if (data->number_of_vertexes++ == data->matrix_3d.storageSize) {
    data->matrix_3d.storageSize *= MULT;
    data->matrix_3d.matrix = realloc(
        data->matrix_3d.matrix, data->matrix_3d.storageSize * sizeof(float*));
  }
  data->matrix_3d.matrix[data->number_of_vertexes - 1] =
      calloc(3, sizeof(float));
  data->matrix_3d.rows = data->number_of_vertexes;
}

void add_vertex(data_t* data, char* line, int* currentVertex) {
  float x, y, z;
  char ch;
  if (sscanf(line, "%c%f%f%f", &ch, &x, &y, &z) == 4) {
    *currentVertex += 1;
    changeMatrix(data);
    data->matrix_3d.matrix[*currentVertex][X] = x;
    data->matrix_3d.matrix[*currentVertex][Y] = y;
    data->matrix_3d.matrix[*currentVertex][Z] = z;
    checkMinMax(data, x, y, z);
  }
}

void changePolygons(data_t* data) {
  if (data->number_of_facets++ == data->storageSize) {
    data->storageSize *= MULT;
    data->polygons =
        realloc(data->polygons, data->storageSize * sizeof(polygon_t));
  }
  data->polygons[data->number_of_facets - 1].vertexes =
      calloc(INIT_SIZE, sizeof(int));
  data->polygons[data->number_of_facets - 1].storageSize = INIT_SIZE;
}

void add_polygon(data_t* data, char* line, int currentPolygon) {
  changePolygons(data);
  char* current = line;
  int step = -1, tmp = 0;
  while (strchr(current, ' ')) {
    current = strchr(current, ' ') + 1;
    if (sscanf(current, "%d", &tmp) == 1) {
      if (++step == data->polygons[currentPolygon].storageSize) {
        data->polygons[currentPolygon].storageSize *= MULT;
        data->polygons[currentPolygon].vertexes =
            realloc(data->polygons[currentPolygon].vertexes,
                    data->polygons[currentPolygon].storageSize * sizeof(int));
      }
      data->polygons[currentPolygon].vertexes[step] = tmp - 1;
    }
  }
  data->polygons[currentPolygon].number_of_vertexes_in_facets = step + 1;
}
