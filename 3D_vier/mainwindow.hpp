#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <math.h>

#include <QColorDialog>
#include <QDebug>
#include <QFileDialog>
#include <QGraphicsScene>
#include <QLabel>
#include <QMainWindow>
#include <QOpenGLFunctions>
#include <QWidget>
#include <QtOpenGL>
#include <fstream>
#include <iostream>
#include <string>

#include "lib/gifimage/qgifimage.h"
#include "myGlView.hpp"

extern "C" {
#include "../parsing.h"
}

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT
 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  glView *myGlView;
  QTimer on;
  int frame;
  QGifImage *gif;
  QFile *saveFile;
  void updateLabel();
  void parseSettingFile(char *line, char **temp);
  void updateUI(bool loadSettings);
  void startTimerGif();
  void recordGif();
  void openFile(QString file);
  void parseFile();
  void updateXYZcord();
  void initUiSettings();
  void acceptSettings(char **temp);
  void openSettingFile(QString file, char **temp);

 public slots:

 private slots:

  void on_left_clicked();

  void on_right_clicked();

  void on_up_clicked();

  void on_down_clicked();

  void on_x_animation_2_toggled(bool checked);

  void on_y_animation_2_toggled(bool checked);

  void on_stap_toggled(bool checked);

  void on_z_up_clicked();

  void on_z_down_clicked();

  void on_x_up_clicked();

  void on_x_down_clicked();

  void on_y_up_clicked();

  void on_y_down_clicked();

  void on_perspective_switch_clicked();

  void on_selectFileButton_clicked();

  void on_selectLineColorButton_clicked();

  void on_selectLineWidth_valueChanged(int value);

  void on_selectPointColorButton_clicked();

  void on_selectPointsSizeSlider_valueChanged(int value);

  void on_selectTypePointsCBox_currentIndexChanged(int index);

  void on_selectTypeLinesCBox_currentIndexChanged(int index);

  void on_selectBGColorButton_clicked();

  void on_saveScreenButton_clicked();

  void on_jpegRButton_clicked();

  void on_bmpRButton_clicked();

  void on_actionSave_settings_triggered();

  void on_actionLoad_settings_triggered();

  void on_horizontalSlider_valueChanged(int value);

  void on_gifButton_clicked();

  void on_perspectivePointPos_valueChanged(int value);

  void on_xRotateUpButton_clicked();

  void on_xRotateDownButton_clicked();

  void on_yRotateDownButton_clicked();

  void on_yRotateUpButton_clicked();

  void on_zRotateUpButton_clicked();

  void on_zRotateDownButton_clicked();

  void on_xFieldOffsetSB_valueChanged(int arg1);

  void on_zFieldOffsetSB_valueChanged(int arg1);

  void on_yFieldOffsetSB_valueChanged(int arg1);

  void on_xFieldRotateSB_valueChanged(int arg1);

  void on_zFieldRotateSB_valueChanged(int arg1);

  void on_yFieldRotateSB_valueChanged(int arg1);

  void on_scaleUpButton_clicked();

  void on_scaleDownButton_clicked();

  void on_scaleSB_valueChanged(int arg1);

  void on_z_animation_2_toggled(bool checked);

  void animationUpdate();

 private:
  Ui::MainWindow *ui;
};

#endif  // MAINWINDOW_H
