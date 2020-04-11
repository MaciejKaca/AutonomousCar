#pragma once

#include "common.h"
#include "Base/serialPortBase.h"

class SerialPort : SerialPortBase
{
public:
    SerialPort();
    ~SerialPort();
    void send(U8 *buffer,const S16 &size) override;
    void read(U8 *buffer,const S16 &size) override;
    const bool &isSerialOpen();

private:
    bool serialStatus;
    S16 CPORT_NR;
    const S16 CPORT_NR_BASE;
    const S16 CPORT_NR_BACKUP;
    const S16 BAUD_RATE = 9600;
    void open_comport();
};