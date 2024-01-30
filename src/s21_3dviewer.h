#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define X 0
#define Y 1
#define Z 2

#define MIN 0
#define MAX 1

#define MULT 2
#define INIT_SIZE 100

typedef struct polygon {
  // size = number of cooddinates in a 'p' line
  int *vertexes;
  int number_of_vertexes_in_facets;
  int storageSize;
} polygon_t;

typedef struct matrix {
  // size = number of v-es x 3
  float **matrix;
  int rows;
  float minMax[3][2];
  int storageSize;
} matrix_t;

typedef struct data {
  int number_of_vertexes;
  int number_of_facets;
  // rows = number of vertexes.
  matrix_t matrix_3d;
  // each polygon has a number of vertexes in a facet and an array of the v-es
  // that connect
  // size of polygons = number of lines starting with p
  polygon_t *polygons;
  int storageSize;
} data_t;

typedef struct transformCoord {
  float xShift;
  float yShift;
  float zShift;
  float xScale;
  float yScale;
  float zScale;
  float xAngle;
  float yAngle;
  float zAngle;
} transform_t;

void init_data(data_t* data);
void parser(char *file_path, data_t *obj_data);
void add_vertex(data_t* obj_data, char* line, int *currentVertex);
void add_polygon(data_t* obj_data, char* line, int currentPoligon);

void clearData(data_t *obj_data);

// opens file and fills in structures
data_t read_obj_file(char *file_path);

void shift(data_t *obj_data, float shift, int i, int coordinate);
void rotate(data_t *data, float angle, int i, int coordinate);
void scale(data_t *obj_data, float scale, int i, int coordinate);
void puttingInOrder(data_t *data);
void allTransforms(data_t *data, transform_t tr);
