#include <inc/serialport.h>
#include <inc/common.h>

#include <RS-232/rs232.h>

SerialPort::SerialPort(const S8 * device) : CPORT_NR( RS232_GetPortnr( (const char *) device) )
{
    qInfo("in SerialPort, initializing constructor");
    open_comport();
}

void SerialPort::open_comport()
{
    const char mode[]={'8','N','1',0};

    if(RS232_OpenComport(CPORT_NR, BAUD_RATE, mode))
    {
        qCritical("in SerialPort::open_comport(): missing arduino. Trying backup.");
        const S16 BACKUP_CPORT_NR = RS232_GetPortnr( (const char *) LIGHTS_AND_SERVO_PORT_BACKUP);

        if(RS232_OpenComport(BACKUP_CPORT_NR, BAUD_RATE, mode))
        {
            qCritical("in SerialPort::open_comport(): missing arduino on backup.");
            exit(EXIT_BY_MISSING_MODULE);
        }
        else
        {
            qWarning("in SerialPort::open_comport(): Arduino found on bakcup");
        }
    }
    else
    {
        qInfo("in SerialPort::open_comport(): Arduino found");
    }
}

bool SerialPort::send(U8 * buffer,const S16 size)
{
    if( RS232_SendBuf(CPORT_NR, buffer, size) > 0)
    {
        qInfo("in SerialPort::send(): success");
        return 1; //Succes
    }
    else
    {
        qWarning("in SerialPort::send(): failed");
        return 0; //Sending failed
    }
}

void SerialPort::read(U8 * buffer,const S16 size)
{
    qInfo("in SerialPort::read()");
    RS232_PollComport(CPORT_NR, buffer, size);
}
