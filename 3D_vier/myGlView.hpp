#ifndef MYGLVIEW_H
#define MYGLVIEW_H

#include <QColor>
#include <QDebug>
#include <QOpenGLWidget>
#include <QString>
#include <QTimer>
#include <math.h>

extern "C" {
#include "doing.h"
}

struct objectmodel {
  QString filePath;
  float XY_rote[3];
  float **points;
  float **result;
  float pos_left;
  float up_down;
  float *points_optim;
  float *result_optim;
  int **LINES;
  int *LINES_optim;
  int sum_points;
  int sum_lines;
  int offsetX;
  int offsetY;
  int offsetZ;
  bool loadedFile;
  int flag_anim_x;
  int flag_anim_y;
  int flag_anim_z;
  int scale;
  int flag_perspective;
//  int flag_first_open;
  float perspective_point_posion_z;
  QColor colorLine;
  QColor colorBackground;
  QColor colorPoints;
  GLfloat pointSize;
  GLfloat lineWidth;
  bool dottedLine;
  int typePoints;
  bool jpegBmpSwitcher;
  int errorState = 1;
  int timerSpeed;
  QTimer *timerGIF;
};

class glView : public QOpenGLWidget {
  Q_OBJECT
 public:
  glView(QWidget *parent) : QOpenGLWidget(parent){};

  objectmodel model;

  void initializeGL() override;

  void resizeGL(int w, int h) override;

  void paintGL() override;


  QTimer on;
};

#endif  // MYGLVIEW_H
