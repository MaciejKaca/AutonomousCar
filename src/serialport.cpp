#include <inc/serialport.h>
#include <inc/common.h>

#include <RS-232/rs232.h>

SerialPort::SerialPort(QString _port) : CPORT_NR(RS232_GetPortnr(_port.toStdString().c_str())), PORT(_port)
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

    if(RS232_OpenComport(CPORT_NR, BAUD_RATE, mode))
    {
        qCritical("in SerialPort::open_comport(): missing module on port %s", PORT.toStdString().c_str());
        throw EXIT_BY_MISSING_MODULE;
    }
    else
    {
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
