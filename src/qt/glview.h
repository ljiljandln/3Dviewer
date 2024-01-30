#ifndef GLVIEW_H
#define GLVIEW_H

#include <QWidget>
#include <QtOpenGL>
#include <QOpenGLWidget>
#include <QFileDialog>

extern "C" {
#include "../s21_3dviewer.h"
}

QT_BEGIN_NAMESPACE
namespace Ui { class glView; }
QT_END_NAMESPACE

class glView : public QOpenGLWidget
{
    Q_OBJECT
private:
    float xRot, yRot, zRot;
    QPoint mPos;
    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
public:
    glView(QWidget *parent = nullptr);
    ~glView();
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void draw();
    void draw_vertexes(float **vertexes, int countV);
    void draw_polygons(float **vertexes, polygon_t *polygons, int countP);

private slots:
    void on_pushButtonFile_clicked();

    void on_pushButton_clicked();

private:
    Ui::glView *ui;
    QString fileName;
};
#endif // GLVIEW_H
