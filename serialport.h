#ifndef SERIALPORT_H
#define SERIALPORT_H

#include "common.h"

class SerialPort
{
public:
    SerialPort(const S8 * device);
    bool send(U8 * buffer,const S16 size);
    void read(U8 *buffer,const S16 size);

private:
    const S16 CPORT_NR;
    const S16 BAUD_RATE = 9600;
    void open_comport();
};

#endif // SERIALPORT_H
