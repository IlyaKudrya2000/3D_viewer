#include "mainwindow.hpp"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  myGlView = new glView(parent);
  initUiSettings();
  ui->verticalLayout->addWidget(myGlView, 1);
  updateLabel();
  myGlView->model.loadedFile = false;
  myGlView->model.timerGIF = new QTimer(this);
  //rotate()
}

MainWindow::~MainWindow() {
  delete myGlView;
  delete ui;
  //
}

void MainWindow::updateLabel() {
  if (!myGlView->model.errorState) {
    if (!myGlView->model.filePath.isEmpty()) {
      QFileInfo fi(myGlView->model.filePath);
      fi.fileName();
      ui->textBrowser->setText(
          fi.fileName() +
          "\nCount lines: " + QString::number(myGlView->model.sum_lines) +
          "\nCount points: " + QString::number(myGlView->model.sum_points) +
          "\nPos X: " + QString::number(myGlView->model.pos_left) +
          "\nPos Y: " + QString::number(myGlView->model.up_down));
    } else
      ui->textBrowser->setText("Use open file");
  } else if (myGlView->model.errorState == 1) {
    ui->textBrowser->setText("File not found!");
  } else if (myGlView->model.errorState == 2) {
    ui->textBrowser->setText("Error parsing file!");
  } else if (myGlView->model.errorState == 3) {
    ui->textBrowser->setText("Error load settings. File not found");
  } else if (myGlView->model.errorState == 4) {
    ui->textBrowser->setText("Error pasing settings file!");
  } else
    ui->textBrowser->setText("Unknown error");
  if (myGlView->model.loadedFile) {
    myGlView->model.errorState = 0;
  }
}

void MainWindow::on_left_clicked() {
  myGlView->model.pos_left -= 15;
  myGlView->repaint();
  updateLabel();
}

void MainWindow::on_right_clicked() {
  myGlView->model.pos_left += 15;
  myGlView->repaint();
  updateLabel();
}

void MainWindow::on_up_clicked() {
  myGlView->model.up_down += 15;
  myGlView->repaint();
  updateLabel();
}

void MainWindow::on_down_clicked() {
  myGlView->model.up_down -= 15;
  myGlView->repaint();
  updateLabel();
}

void MainWindow::on_x_animation_2_toggled(bool checked) {
  if (checked) {
    if (!(myGlView->model.flag_anim_y || myGlView->model.flag_anim_z)) {
      connect(&on, SIGNAL(timeout()), this, SLOT(animationUpdate()));
      on.start(myGlView->model.timerSpeed);
    } else {
      myGlView->model.flag_anim_z = 0;
      myGlView->model.flag_anim_y = 0;
    }
    myGlView->model.flag_anim_x = 1;
  }
}

void MainWindow::animationUpdate() {
  myGlView->repaint();
  updateUI(0);
  updateLabel();
}

void MainWindow::on_y_animation_2_toggled(bool checked) {
  if (checked) {
    if (myGlView->model.flag_anim_x == 0 && myGlView->model.flag_anim_z == 0) {
      connect(&on, SIGNAL(timeout()), this, SLOT(animationUpdate()));
      on.start(myGlView->model.timerSpeed);
    } else {
      myGlView->model.flag_anim_x = 0;
      myGlView->model.flag_anim_z = 0;
    }
    myGlView->model.flag_anim_y = 1;
  }
}

void MainWindow::on_stap_toggled(bool checked) {
  if (checked) {
    on.stop();
    disconnect(&on, SIGNAL(timeout()), this, SLOT(animationUpdate()));
    myGlView->model.flag_anim_y = 0;
    myGlView->model.flag_anim_x = 0;
    myGlView->model.flag_anim_z = 0;
    ui->stap->setChecked(true);
    animationUpdate();
  }
}

void MainWindow::on_horizontalSlider_valueChanged(int value) {
  myGlView->model.timerSpeed = value;
  on.setInterval(myGlView->model.timerSpeed);
}

void MainWindow::on_z_up_clicked() {
  if (myGlView->model.offsetZ < 50 && myGlView->model.loadedFile) {
    for (int i = 0; i < myGlView->model.sum_points; i++) {
      myGlView->model.points_optim[i * 4 + 2] =
          1 + myGlView->model.points_optim[i * 4 + 2];
    }
    myGlView->model.offsetZ++;
    ui->zFieldOffsetSB->setValue(myGlView->model.offsetZ);
    myGlView->repaint();
    updateLabel();
  }
}

void MainWindow::on_z_down_clicked() {
  if (myGlView->model.offsetZ > -50 && myGlView->model.loadedFile) {
    for (int i = 0; i < myGlView->model.sum_points; i++) {
      myGlView->model.points_optim[i * 4 + 2] =
          myGlView->model.points_optim[i * 4 + 2] - 1;
    }
    myGlView->model.offsetZ--;
    ui->zFieldOffsetSB->setValue(myGlView->model.offsetZ);
    myGlView->repaint();
    updateLabel();
  }
}

void MainWindow::on_x_up_clicked() {
  if (myGlView->model.offsetX < 50 && myGlView->model.loadedFile) {
    for (int i = 0; i < myGlView->model.sum_points; i++) {
      myGlView->model.points_optim[i * 4] =
          1 + myGlView->model.points_optim[i * 4];
    }
    myGlView->model.offsetX++;
    ui->xFieldOffsetSB->setValue(myGlView->model.offsetX);
    myGlView->repaint();
    updateLabel();
  }
}

void MainWindow::on_x_down_clicked() {
  if (myGlView->model.offsetX > -50 && myGlView->model.loadedFile) {
    for (int i = 0; i < myGlView->model.sum_points; i++) {
      myGlView->model.points_optim[i * 4] =
          myGlView->model.points_optim[i * 4] - 1;
    }
    myGlView->model.offsetX--;
    ui->xFieldOffsetSB->setValue(myGlView->model.offsetX);
    myGlView->repaint();
    updateLabel();
  }
}

void MainWindow::on_y_up_clicked() {
  if (myGlView->model.offsetY < 50 && myGlView->model.loadedFile) {
    for (int i = 0; i < myGlView->model.sum_points; i++) {
      myGlView->model.points_optim[i * 4 + 1] =
          myGlView->model.points_optim[i * 4 + 1] + 1;
    }
    myGlView->model.offsetY++;
    ui->yFieldOffsetSB->setValue(myGlView->model.offsetY);
    myGlView->repaint();
    updateLabel();
  }
}

void MainWindow::on_y_down_clicked() {
  if (myGlView->model.offsetY > -50 && myGlView->model.loadedFile) {
    for (int i = 0; i < myGlView->model.sum_points; i++) {
      myGlView->model.points_optim[i * 4 + 1] =
          myGlView->model.points_optim[i * 4 + 1] - 1;
    }
    myGlView->model.offsetY--;
    ui->yFieldOffsetSB->setValue(myGlView->model.offsetY);
    myGlView->repaint();
    updateLabel();
  }
}

void MainWindow::on_perspective_switch_clicked() {
  myGlView->model.flag_perspective = (1 + myGlView->model.flag_perspective) % 2;
  myGlView->repaint();
  updateLabel();
}

void MainWindow::on_selectFileButton_clicked() {
   //myGlView->model.flag_first_open = 0;
  on_stap_toggled(true);
  QString file = QFileDialog::getOpenFileName(
      this, tr("Open file"), QDir::homePath(), tr("Object files (*.obj)"));
  openFile(file);
  if (!myGlView->model.errorState) {
    initUiSettings();
    updateUI(0);
  } else
    qDebug() << "Error";
  updateLabel();
  myGlView->repaint();
  //myGlView->model.flag_first_open = 1;
}

void MainWindow::openFile(QString file) {
  if (!file.isEmpty()) {
    int lines = 0;
    int points = 0;

    myGlView->model.errorState =
        points_and_lines_counter(&points, &lines, file.toLocal8Bit().data());
    if (!myGlView->model.errorState) {
      myGlView->model.filePath = file;
      myGlView->model.sum_lines = lines + 1;
      myGlView->model.sum_points = points;
      myGlView->model.points = new float*[myGlView->model.sum_points];
      myGlView->model.result = new float*[myGlView->model.sum_points];
      myGlView->model.LINES = new int*[myGlView->model.sum_lines];
      myGlView->model.LINES_optim = new int[myGlView->model.sum_lines * 2];
      for (int i = 0; i < myGlView->model.sum_points; i++) {
        myGlView->model.result[i] = new float[4]{};
        myGlView->model.points[i] = new float[4]{};
      }
      for (int i = 0; i < myGlView->model.sum_lines; i++)
        myGlView->model.LINES[i] = new int[2]{};
      parseFile();

      if (!myGlView->model.errorState) myGlView->model.loadedFile = true;
    } else {
      qDebug() << "Error points or lines not found ";
      myGlView->model.errorState = 2;
    }
  } else {
    myGlView->model.errorState = 1;
    qDebug() << "File not found";
  }
}

void MainWindow::parseFile() {

  myGlView->model.errorState =
      parsing_xyz(myGlView->model.points, myGlView->model.LINES,
                  &(myGlView->model.sum_points), &(myGlView->model.sum_lines),
                  myGlView->model.filePath.toLocal8Bit().data());
  if (!myGlView->model.errorState) {
    myGlView->model.points_optim = new float[myGlView->model.sum_points * 4];
    myGlView->model.result_optim = new float[myGlView->model.sum_points * 4];
    for (int i = 0; i < myGlView->model.sum_points; i++) {
      for (int j = 0; j < 4; j++) {
        myGlView->model.points_optim[i * 4 + j] = myGlView->model.points[i][j];
      }
    }
    for (int i = 0; i < myGlView->model.sum_lines; i++) {
      for (int j = 0; j < 2; j++) {
        myGlView->model.LINES_optim[i * 2 + j] = myGlView->model.LINES[i][j];
      }
    }

  } else {
    qDebug() << "Error parsing file at lines and points";
    myGlView->model.errorState = 2;
  }
}

void MainWindow::on_selectLineColorButton_clicked() {
  on_stap_toggled(true);
  QColor colorLine = QColorDialog::getColor(myGlView->model.colorLine);
  if (colorLine.isValid()) myGlView->model.colorLine = colorLine;
  myGlView->repaint();
  updateLabel();
}

void MainWindow::on_selectLineWidth_valueChanged(int value) {
  myGlView->model.lineWidth = value;
  myGlView->repaint();
  updateLabel();
}

void MainWindow::on_selectPointColorButton_clicked() {
  on_stap_toggled(true);
  QColor colorPoints = QColorDialog::getColor(myGlView->model.colorPoints);
  if (colorPoints.isValid()) myGlView->model.colorPoints = colorPoints;
  myGlView->repaint();
  updateLabel();
}

void MainWindow::on_selectPointsSizeSlider_valueChanged(int value) {
  myGlView->model.pointSize = value;
  myGlView->repaint();
  updateLabel();
}

void MainWindow::on_selectTypePointsCBox_currentIndexChanged(int index) {
  myGlView->model.typePoints = index;
  myGlView->repaint();
  updateLabel();
}

void MainWindow::on_selectTypeLinesCBox_currentIndexChanged(int index) {
  if (!index) {
    myGlView->model.dottedLine = false;
  } else {
    myGlView->model.dottedLine = true;
  }
  myGlView->repaint();
  updateLabel();
}

void MainWindow::on_selectBGColorButton_clicked() {
  on_stap_toggled(true);
  QColor colorBackground =
      QColorDialog::getColor(myGlView->model.colorBackground);
  if (colorBackground.isValid())
    myGlView->model.colorBackground = colorBackground;
  myGlView->repaint();
  myGlView->repaint();
  updateLabel();
}

void MainWindow::on_saveScreenButton_clicked() {
  if (myGlView->model.loadedFile) {
    on_stap_toggled(true);
    QString filter;
    if (myGlView->model.jpegBmpSwitcher)
      filter = "*.bmp";
    else
      filter = "*.jpeg";
    QString file = QFileDialog::getSaveFileName(this, "Сохранить снимок",
                                                QDir::homePath(), filter);
    if (!file.isEmpty()) {
      QImage pic = myGlView->grabFramebuffer();
      QPixmap pix = QPixmap::fromImage(pic);
      if (myGlView->model.jpegBmpSwitcher)
        if (file.endsWith(".bmp"))
          pix.save(file, "BMP", 100);
        else
          pix.save(file + QString::fromStdString(".bmp"), "BMP", 100);
      else if (file.endsWith(".jpeg"))
        pix.save(file, "JPEG", 100);
      else
        pix.save(file + QString::fromStdString(".jpeg"), "JPEG", 100);
    } else
      qDebug() << "Canceled save file";
  }
}

void MainWindow::on_jpegRButton_clicked() {
  myGlView->model.jpegBmpSwitcher = false;
}

void MainWindow::on_bmpRButton_clicked() {
  myGlView->model.jpegBmpSwitcher = true;
}

void MainWindow::on_actionSave_settings_triggered() {
  if (!myGlView->model.filePath.isEmpty() && myGlView->model.loadedFile) {
    std::ofstream file;
    QString filter = "*.cfg";
    QString file_path = QFileDialog::getSaveFileName(
        this, "Сохранить файл конфигурации", QDir::homePath(), filter);
    if (file_path.endsWith(".cfg"))
      file.open(file_path.toStdString());
    else
      file.open(file_path.toStdString() + ".cfg");
    if (file.is_open()) {
      file << myGlView->model.filePath.toLocal8Bit().data() << ":"
           << ui->xFieldRotateSB->value() << ":" << ui->yFieldRotateSB->value()
           << ":" << myGlView->model.pos_left << ":" << myGlView->model.up_down
           << ":" << myGlView->model.flag_anim_x << ":"
           << myGlView->model.flag_anim_y << ":" << myGlView->model.scale << ":"
           << myGlView->model.flag_perspective << ":"
           << myGlView->model.perspective_point_posion_z << ":"
           << myGlView->model.colorLine.name().toStdString() << ":"
           << myGlView->model.lineWidth << ":" << myGlView->model.dottedLine
           << ":" << myGlView->model.colorBackground.name().toStdString() << ":"
           << myGlView->model.colorPoints.name().toStdString() << ":"
           << myGlView->model.pointSize << ":" << myGlView->model.typePoints
           << ":" << myGlView->model.jpegBmpSwitcher << ":"
           << myGlView->model.offsetX << ":" << myGlView->model.offsetY << ":"
           << myGlView->model.offsetZ << ":" << ui->zFieldRotateSB->value();
    } else
      qDebug() << "File not exist";
    file.close();
    qDebug() << "File written";
  } else {
    qDebug() << "Open file first";
    ui->textBrowser->setText("Open file first");
  }
}

void MainWindow::on_actionLoad_settings_triggered() {
  on_stap_toggled(true);
  QString file = QFileDialog::getOpenFileName(
      this, tr("Open file"), QDir::homePath(), tr("Config files (*.cfg)"));
  if (!file.isEmpty()) {
    myGlView->model.errorState = 0;
    char** settings = new char*[22];
    for (int i = 0; i < 22; i++) settings[i] = new char[256];
    openSettingFile(file, settings);
    if (!myGlView->model.errorState) {
      openFile(settings[0]);
      if (!myGlView->model.errorState) {
        acceptSettings(settings);
        updateXYZcord();
        updateUI(1);
        myGlView->repaint();
      }
    }
  } else {
    qDebug() << "Error load settings. File not found";
    myGlView->model.errorState = 3;
  }
  updateLabel();
}

void MainWindow::openSettingFile(QString file, char** tmp) {
  QFile fi(file);
  if (fi.open(QIODevice::ReadOnly)) {
    QTextStream buffer(&fi);
    QString temp = buffer.readLine();
    if (buffer.atEnd()) {
      qDebug() << "Read line: " << temp;
      parseSettingFile(temp.toLocal8Bit().data(), tmp);
    }
  } else
    qDebug() << "Unable to open file";
}

void MainWindow::parseSettingFile(char* line, char** temp) {
  int countSettings = 0;
  for (int i = 0, k = 0;
       line[i] != '\n' && line[i] != '\0' && i < 256 && countSettings < 22;
       i++) {
    if (line[i] == ':') {
      countSettings++;
      k = i + 1;
    } else {
      temp[countSettings][i - k] = line[i];
      temp[countSettings][i - k + 1] = '\0';
    }
    if (i == 255 && line[i] != '\n' && line[i] != '\0') {
      myGlView->model.errorState = 4;
      qDebug() << "Error pasing settings file";
      updateLabel();
    }
  }
  if (countSettings != 21) {
    myGlView->model.errorState = 4;
    qDebug() << "Incorrect count settings" << QString::number(countSettings);
  }
}

void MainWindow::acceptSettings(char** temp) {
  int tmp = 0;
  QString colorLine = QString::fromStdString(temp[10]);
  QString colorBackground = QString::fromStdString(temp[13]);
  QString colorPoints = QString::fromStdString(temp[14]);

  if (QColor::isValidColor(colorLine) &&
      QColor::isValidColor(colorBackground) &&
      QColor::isValidColor(colorPoints)) {
    myGlView->model.colorBackground.setNamedColor(colorBackground);
    myGlView->model.colorPoints.setNamedColor(colorPoints);
    myGlView->model.colorLine.setNamedColor(colorLine);
  }

  myGlView->model.filePath = temp[0];
  sscanf(temp[1], "%d", &(tmp));
  myGlView->model.XY_rote[0] = M_PI / 360 * tmp;
  sscanf(temp[2], "%d", &(tmp));
  myGlView->model.XY_rote[1] = M_PI / 360 * tmp;

  sscanf(temp[3], "%f", &(myGlView->model.pos_left));
  sscanf(temp[4], "%f", &(myGlView->model.up_down));
  sscanf(temp[5], "%d", &(myGlView->model.flag_anim_x));
  sscanf(temp[6], "%d", &(myGlView->model.flag_anim_y));
  sscanf(temp[7], "%d", &(myGlView->model.scale));
  sscanf(temp[8], "%d", &(myGlView->model.flag_perspective));
  sscanf(temp[9], "%f", &(myGlView->model.perspective_point_posion_z));
  sscanf(temp[11], "%f", &(myGlView->model.lineWidth));
  sscanf(temp[12], "%d", &(tmp));
  if (tmp)
    myGlView->model.dottedLine = true;
  else
    myGlView->model.dottedLine = false;
  sscanf(temp[15], "%f", &(myGlView->model.pointSize));
  sscanf(temp[16], "%d", &(myGlView->model.typePoints));
  sscanf(temp[17], "%d", &(tmp));
  if (tmp)
    myGlView->model.jpegBmpSwitcher = true;
  else
    myGlView->model.jpegBmpSwitcher = false;
  sscanf(temp[18], "%d", &(myGlView->model.offsetX));
  sscanf(temp[19], "%d", &(myGlView->model.offsetY));
  sscanf(temp[20], "%d", &(myGlView->model.offsetZ));
  sscanf(temp[21], "%d", &(tmp));
  myGlView->model.XY_rote[2] = M_PI / 360 * tmp;
  if (myGlView->model.flag_perspective > 0)
    myGlView->model.flag_perspective = 1;
  else
    myGlView->model.flag_perspective = 0;
  if (myGlView->model.perspective_point_posion_z > 20 ||
      myGlView->model.perspective_point_posion_z < 2)
    myGlView->model.perspective_point_posion_z = 0;
  if (myGlView->model.lineWidth > 10 || myGlView->model.lineWidth < 1)
    myGlView->model.lineWidth = 0;
  if (myGlView->model.pointSize > 10 || myGlView->model.pointSize < 1)
    myGlView->model.pointSize = 0;
  if (myGlView->model.typePoints > 2 || myGlView->model.typePoints < 0)
    myGlView->model.typePoints = 0;
  if (myGlView->model.offsetX > 50 || myGlView->model.offsetX < -50)
    myGlView->model.offsetX = 0;
  if (myGlView->model.offsetY > 50 || myGlView->model.offsetY < -50)
    myGlView->model.offsetY = 0;
  if (myGlView->model.offsetZ > 50 || myGlView->model.offsetZ < -50)
    myGlView->model.offsetZ = 0;
  if (myGlView->model.scale > 500 || myGlView->model.scale < 1)
    myGlView->model.scale = 0;
  if (myGlView->model.XY_rote[2] > M_PI || myGlView->model.XY_rote[2] < -M_PI)
    myGlView->model.XY_rote[2] = 0;
  if (myGlView->model.XY_rote[1] > M_PI || myGlView->model.XY_rote[1] < -M_PI)
    myGlView->model.XY_rote[1] = 0;
  if (myGlView->model.XY_rote[0] > M_PI || myGlView->model.XY_rote[0] < -M_PI)
    myGlView->model.XY_rote[0] = 0;
}

void MainWindow::updateUI(bool loadSettings) {
  if (loadSettings) {
    if (myGlView->model.flag_anim_x) {
      ui->x_animation_2->setChecked(true);
      connect(&on, SIGNAL(timeout()), this, SLOT(animationUpdate()));
      on.start(myGlView->model.timerSpeed);
    } else if (myGlView->model.flag_anim_y) {
      ui->y_animation_2->setChecked(true);
      connect(&on, SIGNAL(timeout()), this, SLOT(animationUpdate()));
      on.start(myGlView->model.timerSpeed);
    } else if (myGlView->model.flag_anim_z) {
      connect(&on, SIGNAL(timeout()), this, SLOT(animationUpdate()));
      on.start(myGlView->model.timerSpeed);
    } else {
      ui->stap->setChecked(true);
    }
    if (myGlView->model.jpegBmpSwitcher)
      ui->bmpRButton->setChecked(true);
    else
      ui->jpegRButton->setChecked(true);
  }
  ui->xFieldRotateSB->setValue(
      std::round(myGlView->model.XY_rote[0] * 360 / M_PI));
  ui->yFieldRotateSB->setValue(
      std::round(myGlView->model.XY_rote[1] * 360 / M_PI));
  ui->zFieldRotateSB->setValue(
      std::round(myGlView->model.XY_rote[2] * 360 / M_PI));
  ui->scaleSB->setValue(myGlView->model.scale);
  ui->perspectivePointPos->setValue(myGlView->model.perspective_point_posion_z);
  ui->selectTypePointsCBox->setCurrentIndex(myGlView->model.typePoints);
  ui->selectPointsSizeSlider->setValue(myGlView->model.pointSize);
  ui->selectTypeLinesCBox->setCurrentIndex(myGlView->model.dottedLine);
  ui->selectLineWidth->setValue(myGlView->model.lineWidth);
  ui->zFieldOffsetSB->setValue(myGlView->model.offsetZ);
  ui->yFieldOffsetSB->setValue(myGlView->model.offsetY);
  ui->xFieldOffsetSB->setValue(myGlView->model.offsetX);
}

void MainWindow::on_gifButton_clicked() {
  on_stap_toggled(true);
  QString filter = "*.gif";
  QString filePath = QFileDialog::getSaveFileName(this, "Сохранить GIF",
                                                  QDir::homePath(), filter);
  if (filePath.endsWith(".gif"))
    saveFile = new QFile(filePath);
  else
    saveFile = new QFile(filePath + QString::fromStdString(".gif"));
  if (!filePath.isEmpty() && saveFile->open(QIODevice::WriteOnly)) {
    gif = new QGifImage(QSize(640, 480));
    connect(myGlView->model.timerGIF, &QTimer::timeout, this,
            &MainWindow::recordGif);
    myGlView->model.timerGIF->setInterval(100);
    myGlView->model.timerGIF->start();
    frame = 0;
    ui->gifButton->setEnabled(false);
  } else
    qDebug() << "Error record gif file";
}

void MainWindow::recordGif() {
  myGlView->model.timerGIF->start(100);
  QImage myImage = myGlView->grabFramebuffer();
  myImage = myImage.scaled(640, 480);
  frame++;
  gif->addFrame(myImage, 100);
  if (frame == 50) {
    myGlView->model.timerGIF->stop();
    disconnect(myGlView->model.timerGIF, &QTimer::timeout, this,
               &MainWindow::recordGif);
    gif->save(saveFile);
    saveFile->close();
    delete saveFile;
    saveFile = nullptr;
    delete gif;
    gif = nullptr;
    qDebug() << "Gif saved";
    ui->gifButton->setEnabled(true);
  }
}

void MainWindow::on_perspectivePointPos_valueChanged(int value) {
  myGlView->model.perspective_point_posion_z = value;
  myGlView->repaint();
  updateLabel();
}

void MainWindow::updateXYZcord() {
  for (int i = 0; i < myGlView->model.sum_points; i++) {
    myGlView->model.points_optim[i * 4 + 2] =
        myGlView->model.points_optim[i * 4 + 2] + myGlView->model.offsetZ;
    myGlView->model.points_optim[i * 4] =
        myGlView->model.points_optim[i * 4] + myGlView->model.offsetX;
    myGlView->model.points_optim[i * 4 + 1] =
        myGlView->model.points_optim[i * 4 + 1] + myGlView->model.offsetY;
  }
}

void MainWindow::initUiSettings() {
  myGlView->model.flag_perspective = 0;
  myGlView->model.perspective_point_posion_z = 2;
  myGlView->model.pos_left = 400;
  myGlView->model.up_down = 250;
  myGlView->model.XY_rote[0] = 0;
  myGlView->model.XY_rote[1] = 0;
  myGlView->model.XY_rote[2] = 0;
  myGlView->model.scale = 50;
  myGlView->model.colorLine = Qt::white;
  myGlView->model.colorBackground = Qt::black;
  myGlView->model.colorPoints = Qt::white;
  myGlView->model.pointSize = 1.0f;
  myGlView->model.lineWidth = 1.0f;
  myGlView->model.dottedLine = false;
  myGlView->model.typePoints = 0;
  myGlView->model.jpegBmpSwitcher = false;
  myGlView->model.flag_anim_x = 0;
  myGlView->model.flag_anim_y = 0;
  myGlView->model.flag_anim_z = 0;
  myGlView->model.errorState = 0;
  myGlView->model.offsetX = 0;
  myGlView->model.offsetY = 0;
  myGlView->model.offsetZ = 0;

  myGlView->model.timerSpeed = 35;
}

void MainWindow::on_xRotateDownButton_clicked() {
  if (ui->xFieldRotateSB->value() > -360)
    ui->xFieldRotateSB->setValue(ui->xFieldRotateSB->value() - 1);
  else
    ui->xFieldRotateSB->setValue(359);
  myGlView->model.XY_rote[0] = M_PI / 360 * ui->xFieldRotateSB->value();
  myGlView->repaint();
  updateLabel();
}

void MainWindow::on_xRotateUpButton_clicked() {
  if (ui->xFieldRotateSB->value() < 360)
    ui->xFieldRotateSB->setValue(ui->xFieldRotateSB->value() + 1);
  else
    ui->xFieldRotateSB->setValue(-359);
  myGlView->model.XY_rote[0] = M_PI / 360 * ui->xFieldRotateSB->value();
  myGlView->repaint();
  updateLabel();
}

void MainWindow::on_yRotateDownButton_clicked() {
  if (ui->yFieldRotateSB->value() > -360)
    ui->yFieldRotateSB->setValue(ui->yFieldRotateSB->value() - 1);
  else
    ui->yFieldRotateSB->setValue(359);
  myGlView->model.XY_rote[1] = M_PI / 360 * ui->yFieldRotateSB->value();
  myGlView->repaint();
  updateLabel();
}

void MainWindow::on_yRotateUpButton_clicked() {
  if (ui->yFieldRotateSB->value() < 360)
    ui->yFieldRotateSB->setValue(ui->yFieldRotateSB->value() + 1);
  else
    ui->yFieldRotateSB->setValue(-359);
  myGlView->model.XY_rote[1] = M_PI / 360 * ui->yFieldRotateSB->value();
  myGlView->repaint();
  updateLabel();
}

void MainWindow::on_zRotateUpButton_clicked() {
  if (ui->zFieldRotateSB->value() < 360)
    ui->zFieldRotateSB->setValue(ui->zFieldRotateSB->value() + 1);
  else
    ui->zFieldRotateSB->setValue(-359);
  myGlView->model.XY_rote[2] = M_PI / 360 * ui->zFieldRotateSB->value();
  myGlView->repaint();
  updateLabel();
}

void MainWindow::on_zRotateDownButton_clicked() {
  if (ui->zFieldRotateSB->value() > -360)
    ui->zFieldRotateSB->setValue(ui->zFieldRotateSB->value() - 1);
  else
    ui->zFieldRotateSB->setValue(359);
  myGlView->model.XY_rote[2] = M_PI / 360 * ui->zFieldRotateSB->value();
  myGlView->repaint();
  updateLabel();
}

void MainWindow::on_xFieldOffsetSB_valueChanged(int arg1) {
  if (myGlView->model.loadedFile) {
    for (int i = 0; i < myGlView->model.sum_points; i++)
      myGlView->model.points_optim[i * 4] =
          myGlView->model.points_optim[i * 4] +
          (arg1 - myGlView->model.offsetX);
  }
  myGlView->model.offsetX = arg1;
  myGlView->repaint();
  updateLabel();
}

void MainWindow::on_zFieldOffsetSB_valueChanged(int arg1) {
  if (myGlView->model.loadedFile) {
    for (int i = 0; i < myGlView->model.sum_points; i++)
      myGlView->model.points_optim[i * 4 + 2] =
          myGlView->model.points_optim[i * 4 + 2] +
          (arg1 - myGlView->model.offsetZ);
  }
  myGlView->model.offsetZ = arg1;
  myGlView->repaint();
  updateLabel();
}

void MainWindow::on_yFieldOffsetSB_valueChanged(int arg1) {
  if (myGlView->model.loadedFile) {
    for (int i = 0; i < myGlView->model.sum_points; i++)
      myGlView->model.points_optim[i * 4 + 1] =
          myGlView->model.points_optim[i * 4 + 1] +
          (arg1 - myGlView->model.offsetY);
    myGlView->model.offsetY = arg1;
  }
  myGlView->repaint();
  updateLabel();
}

void MainWindow::on_xFieldRotateSB_valueChanged(int arg1) {
  myGlView->model.XY_rote[0] = M_PI / 360 * arg1;
  myGlView->repaint();
  updateLabel();
}

void MainWindow::on_yFieldRotateSB_valueChanged(int arg1) {
  myGlView->model.XY_rote[1] = M_PI / 360 * arg1;
  myGlView->repaint();
  updateLabel();
}

void MainWindow::on_scaleUpButton_clicked() {
  if (myGlView->model.scale < 500) {
    myGlView->model.scale++;
    ui->scaleSB->setValue(myGlView->model.scale);
    myGlView->repaint();
    updateLabel();
  }
}

void MainWindow::on_scaleDownButton_clicked() {
  if (myGlView->model.scale > 1) {
    myGlView->model.scale--;
    ui->scaleSB->setValue(myGlView->model.scale);
    myGlView->repaint();
    updateLabel();
  }
}

void MainWindow::on_scaleSB_valueChanged(int arg1) {
  myGlView->model.scale = arg1;
  myGlView->repaint();
  updateLabel();
}

void MainWindow::on_z_animation_2_toggled(bool checked) {
  if (checked) {
    if (!(myGlView->model.flag_anim_x || myGlView->model.flag_anim_y)) {
      connect(&on, SIGNAL(timeout()), this, SLOT(animationUpdate()));
      on.start(myGlView->model.timerSpeed);
    } else {
      myGlView->model.flag_anim_x = 0;
      myGlView->model.flag_anim_y = 0;
    }
    myGlView->model.flag_anim_z = 1;
  }
}

void MainWindow::on_zFieldRotateSB_valueChanged(int arg1) {
  myGlView->model.XY_rote[2] = M_PI / 360 * arg1;
  myGlView->repaint();
  updateLabel();
}
