#pragma once

#include "common.h"
#include "Base/serialPortBase.h"

class SerialPort : SerialPortBase
{
public:
    SerialPort();
    bool send(U8 *buffer,const S16 &size) override;
    void read(U8 *buffer,const S16 &size) override;

private:
    const S16 CPORT_NR;
    const S16 CPORT_NR_BACKUP;
    const S16 BAUD_RATE = 9600;
    void open_comport();
};
