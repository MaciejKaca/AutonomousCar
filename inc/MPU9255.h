#pragma once

/*
Written by Qiyong Mu (kylongmu@msn.com)
Adapted for Raspberry Pi by Mikhail Avkhimenia (mikhail.avkhimenia@emlid.com)
Adapted for MPU9255 and I2C by Maciej Kaca (maciek.kaca@gmail.com)
*/

#include "common.h"
#include "RTIMUHal.h"
#include <QVector3D>

class MPU9255
{
public:
    MPU9255();
    ~MPU9255();

    bool initialize();
    bool whoImIAK8968();

    bool writeRegister(U8 subAddress, U8 data);
    bool readRegister(U8 subAddress, U8* data, U8 count=1);
    bool writeAK8963Register(U8 subAddress, U8 data);
    bool readAK8963Register(U8 subAddress, U8 *dest, U8 count=1);

    unsigned int set_gyro_scale(U32 scale);
    unsigned int set_acc_scale(U32 scale);

    void calib_acc();
    void calib_mag();

    void read_temp();
    void read_acc();
    void read_gyro();
    void read_mag();
    QVector3D readImu();

    unsigned int whoami();
    U8 AK8963_whoami();

    void getMotion9(float *ax, float *ay, float *az, float *gx, float *gy, float *gz, float *mx, float *my, float *mz);
    void getMotion6(float *ax, float *ay, float *az, float *gx, float *gy, float *gz);

private:
    struct CalibrationData
    {
        U8 min;
        U8 max;
    };

    RTIMUHal* i2c;

    float acc_divider;
    float gyro_divider;

    int calib_data[3];
    float magnetometer_ASA[3];

    float temperature;
    float accelerometer_data[3];
    float gyroscope_data[3];
    float magnetometer_data[3];

    bool accelEnabled;
    bool gyroEnabled;
    bool tempEnabled;
    bool magEnabled;

    U8 fifoFrameSize;
    U8 gryoFsr;
    U8 gryoLPF;
    U8 accelFsr;
    U8 accelLPF;
    U8 sampleRate;
    const U8 MAX_FIFO_SIZE = 21;

    float _error;

    const U8 GYRO_BYTES = 6;
    const U8 ACCEL_BYTES = 6;
    const U8 TEMP_BYTES = 2;
    const U8 MAG_BYTES = 7;

    // transformation matrix
    /* transform the accel and gyro axes to match the magnetometer axes */
    const S16 tX[3] = {0,  1,  0};
    const S16 tY[3] = {1,  0,  0};
    const S16 tZ[3] = {0,  0, -1};

    bool resetFifo();
    bool setGyroConfig();
    bool setAccelConfig();
    bool setSampleRate();

    void accelCalibration();

    CalibrationData accelCalibrationData;
    CalibrationData gyroCalibrationData;
};

#define MPU9255_ADDRESS0            0x68
#define MPU9255_ADDRESS1            0x69
#define MPU9255_ID                  0x73
#define AK8963_ADDRESS              0x0c

//  Register map

#define MPU9255_SMPRT_DIV           0x19
#define MPU9255_GYRO_LPF            0x1a
#define MPU9255_GYRO_CONFIG         0x1b
#define MPU9255_ACCEL_CONFIG        0x1c
#define MPU9255_ACCEL_LPF           0x1d
#define MPU9255_I2C_MST_EN          0x20
#define MPU9255_FIFO_EN             0x23
#define MPU9255_I2C_MST_CTRL        0x24
#define MPU9255_I2C_SLV0_ADDR       0x25
#define MPU9255_I2C_SLV0_REG        0x26
#define MPU9255_I2C_SLV0_CTRL       0x27
#define MPU9255_I2C_SLV1_ADDR       0x28
#define MPU9255_I2C_SLV1_REG        0x29
#define MPU9255_I2C_SLV1_CTRL       0x2a
#define MPU9255_I2C_SLV2_ADDR       0x2b
#define MPU9255_I2C_SLV2_REG        0x2c
#define MPU9255_I2C_SLV2_CTRL       0x2d
#define MPU9255_I2C_SLV4_CTRL       0x34
#define MPU9255_I2C_READ_FLAG       0x80
#define MPU9255_INT_PIN_CFG         0x37
#define MPU9255_INT_ENABLE          0x38
#define MPU9255_INT_STATUS          0x3a
#define MPU9255_ACCEL_XOUT_H        0x3b
#define MPU9255_GYRO_XOUT_H         0x43
#define MPU9255_EXT_SENS_DATA_00    0x49
#define MPU9255_I2C_SLV0_DO         0x63
#define MPU9255_I2C_SLV1_DO         0x64
#define MPU9255_I2C_MST_DELAY_CTRL  0x67
#define MPU9255_USER_CTRL           0x6a
#define MPU9255_PWR_MGMT_1          0x6b
#define MPU9255_PWR_MGMT_2          0x6c
#define MPU9255_FIFO_COUNT_H        0x72
#define MPU9255_FIFO_R_W            0x74
#define MPU9255_WHO_AM_I            0x75
//  sample rate defines (applies to gyros and accels, not mags)

#define MPU9255_SAMPLERATE_MIN      5                       // 5 samples per second is the lowest
#define MPU9255_SAMPLERATE_MAX      32000                   // 32000 samples per second is the absolute maximum
//  compass rate defines

#define MPU9255_COMPASSRATE_MIN     1                       // 1 samples per second is the lowest
#define MPU9255_COMPASSRATE_MAX     100                     // 100 samples per second is maximum

//  Gyro LPF options

#define MPU9255_GYRO_LPF_8800       0x11                    // 8800Hz, 0.64mS delay
#define MPU9255_GYRO_LPF_3600       0x10                    // 3600Hz, 0.11mS delay
#define MPU9255_GYRO_LPF_250        0x00                    // 250Hz, 0.97mS delay
#define MPU9255_GYRO_LPF_184        0x01                    // 184Hz, 2.9mS delay
#define MPU9255_GYRO_LPF_92         0x02                    // 92Hz, 3.9mS delay
#define MPU9255_GYRO_LPF_41         0x03                    // 41Hz, 5.9mS delay
#define MPU9255_GYRO_LPF_20         0x04                    // 20Hz, 9.9mS delay
#define MPU9255_GYRO_LPF_10         0x05                    // 10Hz, 17.85mS delay
#define MPU9255_GYRO_LPF_5          0x06                    // 5Hz, 33.48mS delay
//  Gyro FSR options

#define MPU9255_GYROFSR_250         0                       // +/- 250 degrees per second
#define MPU9255_GYROFSR_500         8                       // +/- 500 degrees per second
#define MPU9255_GYROFSR_1000        0x10                    // +/- 1000 degrees per second
#define MPU9255_GYROFSR_2000        0x18                    // +/- 2000 degrees per second
//  Accel FSR options

#define MPU9255_ACCELFSR_2          0                       // +/- 2g
#define MPU9255_ACCELFSR_4          8                       // +/- 4g
#define MPU9255_ACCELFSR_8          0x10                    // +/- 8g
#define MPU9255_ACCELFSR_16         0x18                    // +/- 16g
//  Accel LPF options

#define MPU9255_ACCEL_LPF_1130      0x08                    // 1130Hz, 0.75mS delay
#define MPU9255_ACCEL_LPF_460       0x00                    // 460Hz, 1.94mS delay
#define MPU9255_ACCEL_LPF_184       0x01                    // 184Hz, 5.80mS delay
#define MPU9255_ACCEL_LPF_92        0x02                    // 92Hz, 7.80mS delay
#define MPU9255_ACCEL_LPF_41        0x03                    // 41Hz, 11.80mS delay
#define MPU9255_ACCEL_LPF_20        0x04                    // 20Hz, 19.80mS delay
#define MPU9255_ACCEL_LPF_10        0x05                    // 10Hz, 35.70mS delay
#define MPU9255_ACCEL_LPF_5         0x06                    // 5Hz, 66.96mS delay

// Fifo Enable
#define MPU9255_FIFO_TEMP         0x80
#define MPU9255_FIFO_GYRO         0x70
#define MPU9255_FIFO_ACCEL        0x08
#define MPU9255_FIFO_MAG          0x01

//  AK8963 compass registers

#define AK8963_WHO_AM_I             0x00
#define AK8963_DEVICE_ID            0x48                    // the device ID
#define AK8963_ST1                  0x02                    // status 1
#define AK8963_CNTL1                0x0a                    // control reg
#define AK8963_CNTL2                0x0b
#define AK8963_ASAX                 0x10                    // start of the fuse ROM data

/* ---- Sensitivity --------------------------------------------------------- */

#define MPU9250A_2g       ((float)0.000061035156f) // 0.000061035156 g/LSB
#define MPU9250A_4g       ((float)0.000122070312f) // 0.000122070312 g/LSB
#define MPU9250A_8g       ((float)0.000244140625f) // 0.000244140625 g/LSB
#define MPU9250A_16g      ((float)0.000488281250f) // 0.000488281250 g/LSB

#define MPU9250G_250dps   ((float)0.007633587786f) // 0.007633587786 dps/LSB
#define MPU9250G_500dps   ((float)0.015267175572f) // 0.015267175572 dps/LSB
#define MPU9250G_1000dps  ((float)0.030487804878f) // 0.030487804878 dps/LSB
#define MPU9250G_2000dps  ((float)0.060975609756f) // 0.060975609756 dps/LSB

#define MPU9250M_4800uT   ((float)0.6f)            // 0.6 uT/LSB

#define MPU9250T_85degC   ((float)0.002995177763f) // 0.002995177763 degC/LSB

#define Magnetometer_Sensitivity_Scale_Factor ((float)0.15f)
