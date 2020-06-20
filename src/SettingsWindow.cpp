#include <inc/SettingsWindow.h>
#include "ui_SettingsWindow.h"

#include <sys/stat.h>

SettingsWindow::SettingsWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);    

    buttons.push_back(ui->exitButton_button);
    buttons.push_back(ui->brakeButton_button);
    buttons.push_back(ui->leftBlinkerButton_button);
    buttons.push_back(ui->rightBlinkerButton_button);

    ui->exitButton_button->setDescription(QString::number(file.getExitButtonId()));
    ui->brakeButton_button->setDescription(QString::number(file.getBrakeButtonId()));
    ui->leftBlinkerButton_button->setDescription(QString::number(file.getLeftBlinkerButtonId()));
    ui->rightBlinkerButton_button->setDescription(QString::number(file.getRightBlinkerButtonId()));

    focusedButton = nullptr;
    isInputThreadActive = true;
    joystick = new Joystick();
    inputThread = std::async(std::launch::async, &SettingsWindow::assignButton, this);
}

SettingsWindow::~SettingsWindow()
{
}

void SettingsWindow::on_exitButton_button_clicked()
{
    focusedButton = ui->exitButton_button;
}

void SettingsWindow::on_brakeButton_button_clicked()
{
    focusedButton = ui->brakeButton_button;
}

void SettingsWindow::on_leftBlinkerButton_button_clicked()
{
    focusedButton = ui->leftBlinkerButton_button;
}

void SettingsWindow::on_rightBlinkerButton_button_clicked()
{
    focusedButton = ui->rightBlinkerButton_button;
}

inline bool SettingsWindow::isGamepadConnected()
{
    struct stat buffer;
    return (stat (GAMEPAD_PATH.c_str(), &buffer) == 0);
}

void SettingsWindow::assignButton()
{
    if(!isGamepadConnected())
    {
        qCritical( "SettingsWindow::assignButton");
        throw EXIT_BY_MISSING_GAMEPAD;
    }

    clearInput();

    while(isInputThreadActive)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(REFRESH_TIME));

        if(focusedButton != nullptr)
        {
            JoystickEvent event;
            if(joystick->sample(&event))
            {
                if(event.isButton())
                {
                    U8 buttonId = event.number;
                    QString buttonId_string = QString::number(buttonId);

                    for(auto& button : buttons)
                    {
                        if(button != focusedButton)
                        {
                            if(button->description() == buttonId_string)
                            {
                                button->setDescription(focusedButton->description());
                            }
                        }
                    }

                    focusedButton->setDescription(buttonId_string);
                }
            }
        }
    }
}

void SettingsWindow::clearInput()
{
    qInfo("in SettingsWindow::clearInput, clearing input");
    for(U8 i = 0; i < NUMBER_OF_BUTTONS*2; i++)
    {
        JoystickEvent event;
        joystick->sample(&event);
    }

    for(U8 i = 0; i < NUMBER_OF_AXIS*2; i++)
    {
        JoystickEvent event;
        joystick->sample(&event);
    }
}

void SettingsWindow::on_save_button_clicked()
{
    file.setExitButtonId(ui->exitButton_button->description().toLong());
    file.setBrakeButtonId(ui->brakeButton_button->description().toLong());
    file.setLeftBlinkerButtonId(ui->leftBlinkerButton_button->description().toLong());
    file.setRightBlinkerButtonId(ui->rightBlinkerButton_button->description().toLong());

    isInputThreadActive = false;
    inputThread.get();
    delete ui;
    delete joystick;

    this->close();
}
