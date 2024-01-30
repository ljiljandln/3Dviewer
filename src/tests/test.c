#include <check.h>

#include "../s21_3dviewer.h"

START_TEST(counts) {
  char* path = "obj/pyramid.obj";
  data_t data = read_obj_file(path);
  ck_assert_int_eq(5, data.number_of_vertexes);
  ck_assert_int_eq(8, data.number_of_facets);
  clearData(&data);
}
END_TEST

START_TEST(polygons) {
  char* path = "obj/pyramid.obj";
  data_t data = read_obj_file(path);
  ck_assert_int_eq(0, data.polygons[0].vertexes[0]);
  ck_assert_int_eq(1, data.polygons[0].vertexes[1]);
  ck_assert_int_eq(2, data.polygons[0].vertexes[2]);
  clearData(&data);
}
END_TEST

START_TEST(shifts_check) {
  char* path = "obj/cube.obj";
  data_t data = read_obj_file(path);
  transform_t tr = {1, 2, 3, 0, 0, 0, 0, 0, 0};
  allTransforms(&data, tr);
  ck_assert_int_eq(0.5, data.matrix_3d.matrix[0][X]);
  ck_assert_int_eq(1.5, data.matrix_3d.matrix[0][Y]);
  ck_assert_int_eq(2.5, data.matrix_3d.matrix[0][Z]);
  clearData(&data);
}
END_TEST

START_TEST(scales_check) {
  char* path = "obj/teapot.obj";
  data_t data = read_obj_file(path);
  transform_t tr = {0, 0, 0, 3, 2, 1, 0, 0, 0};
  allTransforms(&data, tr);
  ck_assert_int_eq(-1.5, data.matrix_3d.matrix[0][X]);
  ck_assert_int_eq(0.0699409, data.matrix_3d.matrix[0][Y]);
  ck_assert_int_eq(0, data.matrix_3d.matrix[0][Z]);
  clearData(&data);
}
END_TEST


START_TEST(rotates_check) {
  char* path = "obj/pyramid.obj";
  data_t data = read_obj_file(path);
  transform_t tr = {0, 0, 0, 0, 0, 0, 90, 90, 90};
  ck_assert_int_eq(0, data.matrix_3d.matrix[0][X]);
  ck_assert_int_eq(0.034970, data.matrix_3d.matrix[0][Y]);
  ck_assert_int_eq(0.500000, data.matrix_3d.matrix[0][Z]);
  clearData(&data);
}
END_TEST

START_TEST(rotate_y) {
  char* path = "obj/pyramid.obj";
  data_t data = read_obj_file(path);
  rotate(&data, Y, 90);
  ck_assert_int_eq(-1, data.matrix_3d.matrix[2][X]);
  ck_assert_int_eq(-0.5, data.matrix_3d.matrix[2][Y]);
  ck_assert_int_eq(0, data.matrix_3d.matrix[2][Z]);
}
END_TEST

START_TEST(rotate_z) {
  char* path = "obj/pyramid.obj";
  data_t data = read_obj_file(path);
  rotate(&data, Z, 90);
  ck_assert_int_eq(0.5, data.matrix_3d.matrix[2][X]);
  ck_assert_int_eq(1, data.matrix_3d.matrix[2][Y]);
  ck_assert_int_eq(-1, data.matrix_3d.matrix[2][Z]);
}
END_TEST

int main(void) {
  Suite* s1 = suite_create("Core");
  TCase* tc1 = tcase_create("Core");
  SRunner* sr = srunner_create(s1);
  int nf = 0;

  suite_add_tcase(s1, tc1);
  tcase_add_test(tc1, counts);
  tcase_add_test(tc1, polygons);
  tcase_add_test(tc1, shifts_check);
  tcase_add_test(tc1, scales_check);
  tcase_add_test(tc1, rotates_check);

  srunner_run_all(sr, CK_ENV);
  nf = srunner_ntests_failed(sr);
  srunner_free(sr);

  return nf == 0 ? 0 : 1;
}