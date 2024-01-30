#include <QApplication>

#include "glview.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  glView w;
  w.show();
  return a.exec();
}
