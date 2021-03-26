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


void SerialPort::open_comport()
{
    const char mode[]={'8','N','1',0};

    if(RS232_OpenComport(CPORT_NR_BASE, BAUD_RATE, mode))
    {
        qCritical("in SerialPort::open_comport(): missing arduino. Trying backup.");
        if(RS232_OpenComport(CPORT_NR_BACKUP, BAUD_RATE, mode))
        {
            qCritical("in SerialPort::open_comport(): missing arduino on backup.");
            throw EXIT_BY_MISSING_MODULE;
        }
        else
        {
            CPORT_NR = CPORT_NR_BACKUP;
            qWarning("in SerialPort::open_comport(): Arduino found on bakcup");
        }
    }
    else
    {
        CPORT_NR = CPORT_NR_BASE;
        qInfo("in SerialPort::open_comport(): Arduino found");
    }
}

void SerialPort::send(U8 * buffer,const S16 &size)
{
    std::lock_guard<std::mutex> lock(send_mutex);
    if(!(RS232_SendBuf(CPORT_NR, buffer, size) > 0))
    {
        qCritical("in SerialPort::send(): failed on port=%d", CPORT_NR);
        this->open_comport();
        RS232_SendBuf(CPORT_NR, buffer, size);
    }
}

int SerialPort::read(U8 * buffer,const S16 &size)
{
    S16 noIfBytes = RS232_PollComport(CPORT_NR, buffer, size);
    if(noIfBytes < 0)
    {
        qWarning("in SerialPort::read(): Failed to read, ERROR= %d", noIfBytes);
    }
    return noIfBytes;
}
