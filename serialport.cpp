#include "serialport.h"
#include "common.h"

#include "RS-232/rs232.h"

#include <QDebug>

SerialPort::SerialPort(const S8 * device) : CPORT_NR( RS232_GetPortnr( (const char *) device) )
{
    open_comport();
}

void SerialPort::open_comport()
{
    const char mode[]={'8','N','1',0};

    if(RS232_OpenComport(CPORT_NR, BAUD_RATE, mode))
    {
        qCritical( "Arduino not found");
        exit(EXIT_BY_MISSING_MODULE);
    }
}

bool SerialPort::send(U8 * buffer,const S16 size)
{
    if( RS232_SendBuf(CPORT_NR, buffer, size) > 0)
    {
        return 1; //Succes
    }
    else
    {
        return 0; //Sending failed
    }
}

void SerialPort::read(U8 * buffer,const S16 size)
{
    RS232_PollComport(CPORT_NR, buffer, size);
}
