#ifndef SERIALPORT_H
#define SERIALPORT_H

class SerialPort
{
public:
    SerialPort(const char * device);
    bool send(unsigned char * buffer, int size);
    void read(unsigned char *buffer, int size);

private:
    const int CPORT_NR;
    const int BAUD_RATE = 9600;
    void open_comport();
};

#endif // SERIALPORT_H
