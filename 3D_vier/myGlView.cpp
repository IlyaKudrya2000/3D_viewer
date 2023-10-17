#include "myGlView.hpp"

void glView ::initializeGL() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, 800, 600, 0, 0, 1);

}

void glView ::resizeGL(int w, int h) { glViewport(0, 0, w, h); }



void glView ::paintGL() {
  if (model.loadedFile) {
    bool recordCheck = false;
    int remaining = 0;
//    float tmp_fix_rotate[3] {0,0,M_PI / 2};
    if (model.timerGIF->isActive()) {
      remaining = model.timerGIF->remainingTime();
      recordCheck = true;
      model.timerGIF->stop();
    }
    rotate(model.points_optim, model.result_optim, model.XY_rote,
           model.sum_points, model.flag_perspective,
           model.perspective_point_posion_z, model.LINES, model.sum_lines);
//    if(model.flag_first_open == 0){
//        rotate(model.points_optim, model.result_optim, tmp_fix_rotate,
//               model.sum_points, model.flag_perspective,
//               model.perspective_point_posion_z, model.LINES, model.sum_lines);
//        printf("123\n");
//    }
    glClearColor(model.colorBackground.redF(), model.colorBackground.greenF(),
                 model.colorBackground.blueF(), model.colorBackground.alphaF());

    if (model.flag_anim_x == 1) model.XY_rote[0] += (M_PI / 32);
    if (model.flag_anim_y == 1) model.XY_rote[1] += (M_PI / 32);
    if (model.flag_anim_z == 1) model.XY_rote[2] += (M_PI / 32);

    if (model.XY_rote[0] >= M_PI) model.XY_rote[0] = -M_PI + (M_PI / 360);
    if (model.XY_rote[0] <= -M_PI) model.XY_rote[0] = M_PI - (M_PI / 360);
    if (model.XY_rote[1] >= M_PI) model.XY_rote[1] = -M_PI + (M_PI / 360);
    if (model.XY_rote[1] <= -M_PI) model.XY_rote[1] = M_PI - (M_PI / 360);
    if (model.XY_rote[2] >= M_PI) model.XY_rote[2] = -M_PI + (M_PI / 360);
    if (model.XY_rote[2] <= -M_PI) model.XY_rote[2] = M_PI - (M_PI / 360);

    glLineWidth(model.lineWidth);
    if (model.dottedLine) {
      glEnable(GL_LINE_STIPPLE);
      glLineStipple(1, 0b0000000011111111);
    } else
      glDisable(GL_LINE_STIPPLE);

    glBegin(GL_LINES);
    glColor3d(model.colorLine.redF(), model.colorLine.greenF(),
              model.colorLine.blueF());  // Цвет линий
    for (int i = 0; i < model.sum_lines; i++) {
      glVertex2f(
          model.result_optim[(model.LINES_optim[i * 2 + 0] - 1) * 4] *
                  model.scale +
              model.pos_left,
          model.result_optim[(model.LINES_optim[i * 2 + 0] - 1) * 4 + 1] *
                  model.scale +
              model.up_down);
      glVertex2f(
          model.result_optim[(model.LINES_optim[i * 2 + 1] - 1) * 4] *
                  model.scale +
              model.pos_left,
          model.result_optim[(model.LINES_optim[i * 2 + 1] - 1) * 4 + 1] *
                  model.scale +
              model.up_down);
    }
    glEnd();

    if (model.typePoints) {
      glColor3d(model.colorPoints.redF(), model.colorPoints.greenF(),
                model.colorPoints.blueF());
      glPointSize(model.pointSize * 2);
      if (model.typePoints == 1) glEnable(GL_POINT_SMOOTH);
      glBegin(GL_POINTS);
      int sum = 0;
      if (model.sum_points * 4 < model.sum_lines)
        sum = model.sum_points * 4;
      else
        sum = model.sum_lines;
      for (int i = 0; i < sum; i++) {
        float x1 = model.result_optim[(model.LINES_optim[i * 2 + 0] - 1) * 4] *
                       model.scale +
                   model.pos_left;
        float y1 =
            model.result_optim[(model.LINES_optim[i * 2 + 0] - 1) * 4 + 1] *
                model.scale +
            model.up_down;
        glVertex2f(x1, y1);
      }
      glEnd();
      glDisable(GL_POINT_SMOOTH);
    }
    if (recordCheck) {
      model.timerGIF->start(remaining);
    }
  }
}
