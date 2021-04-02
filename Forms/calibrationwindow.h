#ifndef CALIBRATIONWINDOW_H
#define CALIBRATIONWINDOW_H

#include <QDockWidget>

namespace Ui {
class calibrationWindow;
}

class calibrationWindow : public QDockWidget
{
    Q_OBJECT

public:
    enum CalibrationType
    {
        ACCEL_GYRO_CALIBRATION,
        MAG_CALIBRATION,
        CALIBRATION_DONE
    };

    explicit calibrationWindow(QWidget *parent = nullptr);
    void changeCalibrationType(const CalibrationType& type);
    ~calibrationWindow();

private slots:
    void on_acceptButton_accepted();

private:
    Ui::calibrationWindow *ui;
    const QString accelGyroCalibrationMsg = "Accelerometer and Gyrometer calibration. Put the car on the ground, and don't move it";
    const QString magnCalibrationMsg = "Magnetometer calibration. Make 8 figure until done";
    const QString calDoneMsg = "Calibration done";
};

#endif // CALIBRATIONWINDOW_H
