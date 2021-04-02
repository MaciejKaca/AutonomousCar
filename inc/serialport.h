#pragma once

#include "common.h"
#include "Base/serialPortBase.h"

class SerialPort : SerialPortBase
{
public:
    SerialPort(QString _port);
    ~SerialPort();
    void send(U8 *buffer,const S16 &size) override;
    int read(U8 *buffer,const S16 &size) override;

private:
    bool serialStatus;
    //S16 CPORT_NR;
    const QString PORT;
    const S16 CPORT_NR;
    const S16 BAUD_RATE = 9600;

    void open_comport();
    std::mutex send_mutex;
};
