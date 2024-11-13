// Definición de la macro de protección contra múltiples inclusión
#ifndef MPU6050_LIBRARY_H
#define MPU6050_LIBRARY_H

//Estas líneas incluyen las bibliotecas necesarias para utilizar el MPU-6050 y el protocolo I2C.
#include "Adafruit_MPU6050.h"
#include "Adafruit_Sensor.h"
#include "Wire.h"



class MPU6050_Library 
{
    public:
        MPU6050_Library();
        
        sensors_event_t a, g, temp;  
        long time_execution, loop_timer;
        float angulo_pitch_acc, angulo_roll_acc, angulo_pitch, angulo_roll;

        bool begin();
        void readSensorData(sensors_event_t *a, sensors_event_t *g, sensors_event_t *t);
        void printSensorData(sensors_event_t *a, sensors_event_t *g, sensors_event_t *t);
        void calibrateSensor(sensors_event_t *a, sensors_event_t *g, sensors_event_t *t);
        void processingDataOfSensor(sensors_event_t *a, sensors_event_t *g, sensors_event_t *t);

    private:
        Adafruit_MPU6050 mpu;
        float gyro_X_cal, gyro_Y_cal, gyro_Z_cal;
        float acc_X_cal, acc_Y_cal, acc_Z_cal;

        bool accCalibOK  = false;
        float gyro_Z, gyro_X, gyro_Y, temperature;


        float acc_total_vector, ax, ay, az;
        bool set_gyro_angles;
};

#endif