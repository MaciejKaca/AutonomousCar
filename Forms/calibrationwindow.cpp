#include "calibrationwindow.h"
#include "ui_calibrationwindow.h"

calibrationWindow::calibrationWindow(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::calibrationWindow)
{
    ui->setupUi(this);
}

calibrationWindow::~calibrationWindow()
{
    delete ui;
}

void calibrationWindow::on_acceptButton_accepted()
{
    this->close();
}

void calibrationWindow::changeCalibrationType(const CalibrationType& type)
{
    switch(type)
    {
        ACCEL_GYRO_CALIBRATION:
            ui->messageText->setText(accelGyroCalibrationMsg);
            break;

        MAG_CALIBRATION:
            ui->messageText->setText(magnCalibrationMsg);
            break;

        CALIBRATION_DONE:
            ui->messageText->setText(calDoneMsg);
            break;

        default:
            break;
    }
}
