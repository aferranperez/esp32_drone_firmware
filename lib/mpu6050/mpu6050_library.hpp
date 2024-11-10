#ifndef MPU6050_LIBRARY_H
#define MPU6050_LIBRARY_H

#include "Adafruit_MPU6050.h"
#include "Adafruit_Sensor.h"
#include "Wire.h"

class MPU6050_Library {
    public:
        MPU6050_Library();
        bool begin();
        void readSensorData(sensors_event_t *a, sensors_event_t *g, sensors_event_t *t);
    private:
        Adafruit_MPU6050 mpu;
};

#endif