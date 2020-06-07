#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QMainWindow>
#include <QCommandLinkButton>
#include <future>
#include <joystick/joystick.hh>
#include <vector>

#include "common.h"

QT_BEGIN_NAMESPACE
namespace Ui { class SettingsWindow; }
QT_END_NAMESPACE

class SettingsWindow : public QMainWindow
{
    Q_OBJECT

public:
    SettingsWindow(QWidget* parent = nullptr);
    ~SettingsWindow();

private slots:
    void on_exitButton_button_clicked();
    void on_brakeButton_button_clicked();
    void on_leftBlinkerButton_button_clicked();
    void on_rightBlinkerButton_button_clicked();

    void on_save_button_clicked();

private:
    const std::string GAMEPAD_PATH = "/dev/input/js0";
    const U8 REFRESH_TIME = 100; //milliseconds
    const U8 NUMBER_OF_BUTTONS = 8;
    const U8 NUMBER_OF_AXIS = 8;

    Ui::SettingsWindow* ui;
    std::future<void> inputThread;
    QCommandLinkButton* focusedButton;
    Joystick* joystick;
    bool isInputThreadActive;
    std::vector<QCommandLinkButton*> buttons;

    inline bool isGamepadConnected();
    void assignButton();
    void clearInput();
};
#endif // SETTINGSWINDOW_H
