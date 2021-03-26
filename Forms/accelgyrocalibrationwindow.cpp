#include "accelgyrocalibrationwindow.h"
#include "ui_accelgyrocalibrationwindow.h"

AccelGyroCalibrationWindow::AccelGyroCalibrationWindow(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::AccelGyroCalibrationWindow)
{
    ui->setupUi(this);
}

AccelGyroCalibrationWindow::~AccelGyroCalibrationWindow()
{
    delete ui;
}

void AccelGyroCalibrationWindow::on_buttonBox_accepted()
{
    this->close();
}
