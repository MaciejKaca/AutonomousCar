#include <inc/serialport.h>
#include <inc/common.h>

#include <RS-232/rs232.h>

SerialPort::SerialPort() : CPORT_NR_BASE(RS232_GetPortnr((const char *)LIGHTS_AND_SERVO_PORT_BASE)),
    CPORT_NR_BACKUP(RS232_GetPortnr((const char *)LIGHTS_AND_SERVO_PORT_BACKUP))
{
    qInfo("in SerialPort, initializing constructor");
    open_comport();
}

SerialPort::~SerialPort()
{
    qInfo("in SerialPort::~SerialPort, destructor called");
    serialStatus = false;
}

SerialPortBase::~SerialPortBase(){}

const bool &SerialPort::isSerialOpen()
{
    return serialStatus;
}

void SerialPort::open_comport()
{
    const char mode[]={'8','N','1',0};

    if(RS232_OpenComport(CPORT_NR_BASE, BAUD_RATE, mode))
    {
        qCritical("in SerialPort::open_comport(): missing arduino. Trying backup.");
        if(RS232_OpenComport(CPORT_NR_BACKUP, BAUD_RATE, mode))
        {
            qCritical("in SerialPort::open_comport(): missing arduino on backup.");
            serialStatus = false;
            throw EXIT_BY_MISSING_MODULE;
        }
        else
        {
            CPORT_NR = CPORT_NR_BACKUP;
            serialStatus = true;
            qWarning("in SerialPort::open_comport(): Arduino found on bakcup");
        }
    }
    else
    {
        CPORT_NR = CPORT_NR_BASE;
        serialStatus = true;
        qInfo("in SerialPort::open_comport(): Arduino found");
    }
}

void SerialPort::send(U8 * buffer,const S16 &size)
{
    if(!(RS232_SendBuf(CPORT_NR, buffer, size) > 0))
    {
        qCritical("in SerialPort::send(): failed");
        this->open_comport();
        RS232_SendBuf(CPORT_NR, buffer, size);
    }
}

void SerialPort::read(U8 * buffer,const S16 &size)
{
    qInfo("in SerialPort::read()");
    RS232_PollComport(CPORT_NR, buffer, size);
}
