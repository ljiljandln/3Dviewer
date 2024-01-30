#include "glview.h"

#include "ui_glview.h"

glView::glView(QWidget* parent) : QOpenGLWidget(parent), ui(new Ui::glView) {
  ui->setupUi(this);
  setWindowTitle("3D Viewer");
  // window size
  setGeometry(900, 300, 800, 800);
}

glView::~glView() { delete ui; }

void glView::initializeGL() {
  // enable depth
  glEnable(GL_DEPTH_TEST);
}

void glView::resizeGL(int w, int h) {
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-1, 1, -1, 1, 1, 10);
}

void glView::paintGL() {
  //  background colour
  glClearColor(ui->bc1->value(), ui->bc2->value(), ui->bc3->value(), 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0, 0, -2);
  glRotatef(xRot, 1, 0, 0);
  glRotatef(yRot, 0, 1, 0);
  // drawing
  draw();
}

void glView::mousePressEvent(QMouseEvent* m) {
  // save mouse position
  mPos = m->pos();
}

void glView::mouseMoveEvent(QMouseEvent* m) {
  xRot = 5 / M_PI * (m->pos().y() - mPos.y());
  yRot = 5 / M_PI * (m->pos().x() - mPos.x());
  update();
}

void glView::on_pushButtonFile_clicked() {
  QString fileName =
      QFileDialog::getOpenFileName(this, "Open File", "~/", "*.obj");
  ui->lineEditFileName->setText(fileName);
}

void glView::draw() {
  glClearColor(ui->bc1->value(), ui->bc2->value(), ui->bc3->value(), 0);
  QString fileName = ui->lineEditFileName->text();
  if (fileName != "") {
    QByteArray temp = fileName.toLocal8Bit();
    char* filePath = temp.data();

    data_t obj = read_obj_file(filePath);
    transform_t tr = {0};

    tr.xShift = ui->shiftX->value();
    tr.yShift = ui->shiftY->value();
    tr.zShift = ui->shiftZ->value();

    tr.xScale = ui->scaleX->value();
    tr.yScale = ui->scaleY->value();
    tr.zScale = ui->scaleZ->value();

    tr.xAngle = ui->rotateX->value();
    tr.yAngle = ui->rotateY->value();
    tr.zAngle = ui->rotateZ->value();
    allTransforms(&obj, tr);

    int countV = obj.number_of_vertexes;
    int countP = obj.number_of_facets;
    float** vertexes = obj.matrix_3d.matrix;
    polygon_t* polygons = obj.polygons;

    QString polygons_string = QString::number(countP);
    ui->p_count->setText(polygons_string);
    QString vertexes_string = QString::number(countV);
    ui->v_count->setText(vertexes_string);

    // vertex colout
    glColor3d(ui->vc->value(), ui->vc2->value(), ui->vc3->value());
    // vertex size
    glPointSize(ui->vsize->value());
    glBegin(GL_POINTS);
    draw_vertexes(vertexes, countV);
    // polygon colour
    glColor3d(ui->pc1->value(), ui->pc2->value(), ui->pc3->value());
    draw_polygons(vertexes, polygons, countP);
  }
}

void glView::draw_vertexes(float** vertexes, int countV) {
  for (int i = 0; i < countV; i++) {
    glVertex3f(vertexes[i][X], vertexes[i][Y], vertexes[i][Z]);
  }
  glEnd();
}

void glView::draw_polygons(float** vertexes, polygon_t* polygons, int countP) {
  for (int i = 0; i < countP; i++) {
    if (ui->quads->isChecked()) {
      glBegin(GL_QUADS);
    } else {
      glBegin(GL_LINE_LOOP);
    }
    for (int j = 0; j < polygons->number_of_vertexes_in_facets; j++) {
      glVertex3d(vertexes[polygons[i].vertexes[j]][X],
                 vertexes[polygons[i].vertexes[j]][Y],
                 vertexes[polygons[i].vertexes[j]][Z]);
    }
    // connecting last to first
    int last = polygons->number_of_vertexes_in_facets - 1;
    glVertex3d(vertexes[polygons[i].vertexes[last]][X],
               vertexes[polygons[i].vertexes[last]][Y],
               vertexes[polygons[i].vertexes[last]][Z]);
    glVertex3d(vertexes[polygons[i].vertexes[0]][X],
               vertexes[polygons[i].vertexes[0]][Y],
               vertexes[polygons[i].vertexes[0]][Z]);

    glEnd();
  }
}

void glView::on_pushButton_clicked() { update(); }
