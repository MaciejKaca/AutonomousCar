#include <inc/MPU9255.h>
#include <QDebug>

#define G_SI        9.80665
#define PI          3.14159
#define SHORT_DELAY 10000
#define LONG_DELAY  100000
#define KHz        1000

MPU9255::MPU9255()
{
    i2c = new(RTIMUHal);
    i2c->m_busIsI2C = true;
    i2c->m_I2CBus = 1;
    i2c->HALOpen();
}

MPU9255::~MPU9255()
{
    delete i2c;
}

bool MPU9255::writeRegister( uint8_t subAddress, uint8_t data )
{
    if(!i2c->HALWrite(MPU9255_ADDRESS0, subAddress, data, ""))
    {
        qCritical("MPU9255::writeRegister Failed to write Address: %X SubAddress: %X Data: %X", MPU9255_ADDRESS0, subAddress, data);
        return false;
    }
    return true;
}

bool MPU9255::readRegister(U8 subAddress, U8* data, U8 count)
{
    if(!i2c->HALRead(MPU9255_ADDRESS0, subAddress, count, data, ""))
    {
        qCritical("MPU9255::readRegister Failed to read Address: %X SubAddress: %X", MPU9255_ADDRESS0, subAddress);
        return false;
    }
    return true;
}

bool MPU9255::readAK8963Register(U8 subAddress, U8 *dest, U8 count)
{
    if (!writeRegister(MPU9255_I2C_SLV0_ADDR, AK8963_ADDRESS | MPU9255_I2C_READ_FLAG) ||
        !writeRegister(MPU9255_I2C_SLV0_REG, subAddress) ||
        !writeRegister(MPU9255_I2C_SLV0_CTRL, 0x80 | count))
    {
        usleep(SHORT_DELAY);
        if (readRegister(MPU9255_EXT_SENS_DATA_00, dest, count))
        {
            return true;
        }
    }

    qCritical("MPU9255::readAK8963Register Failed to read SubAddress: %X", subAddress);
    return false;
}

bool MPU9255::writeAK8963Register(U8 subAddress, U8 data)
{
    // set slave 0 to the AK8963 and set for write
    if(!writeRegister(MPU9255_I2C_SLV0_ADDR, AK8963_ADDRESS) ||
        !writeRegister(MPU9255_I2C_SLV0_REG, subAddress) ||
        !writeRegister(MPU9255_I2C_SLV0_DO, data) ||
        !writeRegister(MPU9255_I2C_SLV0_CTRL, 0x80 | (U8)1))
    {
        return true;
    }

    qCritical("MPU9255::writeAK8963Register Failed to read SubAddress: %X, Data: %X", subAddress, data);
    return false;
}

bool MPU9255::whoImIAK8968()
{
    U8 ak8963ID;
    readAK8963Register(AK8963_WHO_AM_I, &ak8963ID);
    if(ak8963ID != U8(AK8963_DEVICE_ID))
    {
        qCritical("MPU9255::testConnection Failed bad ID:%X != %X", ak8963ID, AK8963_DEVICE_ID);
        return false;
    }
    else
    {
        qInfo("MPU9255::testConnection Success");
        return true;
    }
}

bool MPU9255::resetFifo()
{
    if(writeRegister(MPU9255_USER_CTRL, (0x40 | MPU9255_I2C_MST_EN)) ||
       writeRegister(MPU9255_FIFO_EN,(accelEnabled*MPU9255_FIFO_ACCEL)|
                                     (gyroEnabled*MPU9255_FIFO_GYRO)|
                                     (magEnabled*MPU9255_FIFO_MAG)|
                                     (tempEnabled*MPU9255_FIFO_TEMP)))
    {

        fifoFrameSize = accelEnabled*ACCEL_BYTES + gyroEnabled*GYRO_BYTES +
                        magEnabled*MAG_BYTES + tempEnabled*TEMP_BYTES;
        qInfo("MPU9255::resetFifo succes fifoFrameSize=%d", fifoFrameSize);
        return true;
    }
    else
    {
        qCritical("MPU9255::resetFifo, Failed to reset FIFO");
        return false;
    }
}

bool MPU9255::setGyroConfig()
{
    U8 gyroConfig = gryoFsr << 3;

    if(writeRegister(MPU9255_GYRO_CONFIG, gyroConfig) ||
       writeRegister(MPU9255_GYRO_LPF, gryoLPF))
    {
        qInfo("MPU9255::setGyroConfig, succes");
        return true;
    }
    else
    {
        qCritical("MPU9255::setGyroConfig, Failed");
        return false;
    }
}

bool MPU9255::setSampleRate()
{
    U8 smplrt_div = U8(KHz/sampleRate-1);

    if(writeRegister(MPU9255_SMPRT_DIV, smplrt_div))
    {
        qInfo("MPU9255::setSampleRate, succes");
        return true;
    }
    else
    {
        qCritical("MPU9255::setSampleRate, Failed");
        return false;
    }

}

bool MPU9255::setAccelConfig()
{
    U8 accelConfig = accelFsr << 3;

    if(writeRegister(MPU9255_ACCEL_CONFIG, accelConfig) ||
       writeRegister(MPU9255_ACCEL_LPF, accelLPF))
    {
        qInfo("MPU9255::setAccelConfig, succes");
        return true;
    }
    else
    {
        qCritical("MPU9255::setAccelConfig, Failed");
        return false;
    }
}

bool MPU9255::initialize()
{
    if( writeRegister(MPU9255_PWR_MGMT_1, 0x01)              || // select clock source to gyro
        writeRegister(MPU9255_USER_CTRL, 0x20)               || // enable I2C master mode
        writeRegister(MPU9255_I2C_MST_CTRL, 0x0D)            || // set the I2C bus speed to 400 kHz
        writeAK8963Register(AK8963_CNTL1, 0x00)              || // set AK8963 to Power Down
        writeRegister(MPU9255_PWR_MGMT_1, 0x80)              || // reset the MPU9250
        !usleep(SHORT_DELAY)                                 || // wait for MPU-9250 to come back up
        writeAK8963Register(AK8963_CNTL2, 0x01)              || // reset the AK8963
        writeRegister(MPU9255_PWR_MGMT_1, 0x01)              || // select clock source to gyro
        writeRegister(MPU9255_PWR_MGMT_2, 0x00)              || // enable accelerometer and gyro
        setGyroConfig()                                      || // set gyro config
        setAccelConfig()                                     || // set accel config
        setSampleRate()                                      || // set sample rate
        writeRegister(MPU9255_USER_CTRL, MPU9255_I2C_MST_EN) || // enable I2C master mode
        writeRegister(MPU9255_I2C_MST_CTRL, 0x0D)            || // set the I2C bus speed to 400 kHz
        whoImIAK8968()                                       || // verify magnetometer ID
        writeAK8963Register(AK8963_CNTL1, 0x00)              || // set AK8963 to Power Down
        !usleep(LONG_DELAY)                                  || // wait for AK8963 up
        writeAK8963Register(AK8963_CNTL1, 0x0F)              || // set AK8963 to FUSE ROM access
        !usleep(LONG_DELAY))
    {
        // calibration, see documentation
        U8 asa[3];
        readAK8963Register(AK8963_ASAX, asa, 3);
        magnetometer_ASA[0] = ((float)asa[0] - 128.0) / 256.0 + 1.0f;
        magnetometer_ASA[1] = ((float)asa[1] - 128.0) / 256.0 + 1.0f;
        magnetometer_ASA[2] = ((float)asa[2] - 128.0) / 256.0 + 1.0f;

        if(writeAK8963Register(AK8963_CNTL1, 0x00) || // set AK8963 to Power Down
           usleep(LONG_DELAY)                      || // wait for AK8963 up
           writeAK8963Register(AK8963_CNTL1, 0x16) || // set AK8963 to 16 bit resolution, 100 Hz update rate
           usleep(LONG_DELAY)                      || // wait until mode changes
           writeRegister(MPU9255_PWR_MGMT_1, 0x01) || // select clock source to gyro
           resetFifo())
        {
            qInfo("MPU9255::initialize, Succes");
            return true;
        }
    }
    qCritical("MPU9255::initialize, Failed");
    return false;
}

QVector3D MPU9255::readImu()
{
    U8 buffer[MAX_FIFO_SIZE];
    QVector3D rawAccel, rawGyro, rawMag;
    S16 temperature;

    if (readRegister(MPU9255_ACCEL_XOUT_H, buffer, fifoFrameSize))
    {
        // combine into 16 bit values
        rawAccel.setX((((S16)buffer[0]) << 8) | buffer[1]);
        rawAccel.setY((((S16)buffer[2]) << 8) | buffer[3]);
        rawAccel.setZ((((S16)buffer[4]) << 8) | buffer[5]);
        temperature = (((S16)buffer[6]) << 8) | buffer[7];
        rawGyro.setX((((S16)buffer[8]) << 8) | buffer[9]);
        rawGyro.setY((((S16)buffer[10]) << 8) | buffer[11]);
        rawGyro.setZ((((S16)buffer[12]) << 8) | buffer[13]);
        rawMag.setX((((S16)buffer[15]) << 8) | buffer[14]);
        rawMag.setX((((S16)buffer[17]) << 8) | buffer[16]);
        rawMag.setX((((S16)buffer[19]) << 8) | buffer[18]);
    }
}
