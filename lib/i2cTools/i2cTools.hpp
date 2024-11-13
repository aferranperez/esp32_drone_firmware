#ifndef I2CTOOLS_H
#define I2CTOOLS_H

#include "Arduino.h"
#include "Wire.h"

class I2CTools
{
    public:
        I2CTools();
        void i2cScanner();
    private:
        uint8_t error, address;
        int nDevices;
};

#endif // I2C_INTERFACE_HEADER_H