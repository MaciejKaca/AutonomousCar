#ifndef ACCELGYROCALIBRATIONWINDOW_H
#define ACCELGYROCALIBRATIONWINDOW_H

#include <QDockWidget>

namespace Ui {
class AccelGyroCalibrationWindow;
}

class AccelGyroCalibrationWindow : public QDockWidget
{
    Q_OBJECT

public:
    explicit AccelGyroCalibrationWindow(QWidget *parent = nullptr);
    ~AccelGyroCalibrationWindow();

private:
    Ui::AccelGyroCalibrationWindow *ui;
};

#endif // ACCELGYROCALIBRATIONWINDOW_H
