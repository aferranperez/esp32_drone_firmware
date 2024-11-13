#include "mpu6050_library.hpp"

MPU6050_Library::MPU6050_Library() {}

bool MPU6050_Library::begin() {
     // Try to initialize!
    if (!mpu.begin()) {
        Serial.println("Failed to find MPU6050 chip");
        while (1) {
            delay(10);
        }
    }

    Serial.println("MPU6050 Found!");

    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    Serial.println("Accelerometer range set to: ");
    switch (mpu.getAccelerometerRange()) {
        case MPU6050_RANGE_2_G:
            Serial.println("+-2G");
            break;
        case MPU6050_RANGE_4_G:
            Serial.println("+-4G");
            break;
        case MPU6050_RANGE_8_G:
            Serial.println("+-8G");
            break;
        case MPU6050_RANGE_16_G:
            Serial.println("+-16G");
            break;
    }

    mpu.setGyroRange(MPU6050_RANGE_2000_DEG);
    Serial.print("Gyro range set to: ");
    switch (mpu.getGyroRange()) {
        case MPU6050_RANGE_250_DEG:
            Serial.println("+- 250 deg/s");
            break;
        case MPU6050_RANGE_500_DEG:
            Serial.println("+- 500 deg/s");
            break;
        case MPU6050_RANGE_1000_DEG:
            Serial.println("+- 1000 deg/s");
            break;
        case MPU6050_RANGE_2000_DEG:
            Serial.println("+- 2000 deg/s");
            break;
    }

    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
    Serial.print("Filter bandwidth set to: ");
    switch (mpu.getFilterBandwidth()) {
        case MPU6050_BAND_260_HZ:
            Serial.println("260 Hz");
            break;
        case MPU6050_BAND_184_HZ:
            Serial.println("184 Hz");
            break;
        case MPU6050_BAND_94_HZ:
            Serial.println("94 Hz");
            break;
        case MPU6050_BAND_44_HZ:
            Serial.println("44 Hz");
            break;
        case MPU6050_BAND_21_HZ:
            Serial.println("21 Hz");
            break;
        case MPU6050_BAND_10_HZ:
            Serial.println("10 Hz");
            break;
        case MPU6050_BAND_5_HZ:
            Serial.println("5 Hz");
            break;
    }
    return true;
}

void MPU6050_Library::readSensorData(sensors_event_t *a, sensors_event_t *g, sensors_event_t *t) 
{
    mpu.getEvent(a, g, t);
}

void MPU6050_Library::printSensorData(sensors_event_t *a, sensors_event_t *g, sensors_event_t *t) 
{
    Serial.print("Acceleration X: ");
    Serial.print(a->acceleration.x);
    Serial.print(", Y: ");
    Serial.print(a->acceleration.y);
    Serial.print(", Z: ");
    Serial.print(a->acceleration.z);
    Serial.println(" m/s^2");

    Serial.print("Rotation X: ");
    Serial.print(g->gyro.x);
    Serial.print(", Y: ");
    Serial.print(g->gyro.y);
    Serial.print(", Z: ");
    Serial.print(g->gyro.z);
    Serial.println(" rad/s");

    Serial.print("Temperature: ");
    Serial.print(t->temperature);
    Serial.println(" degC");
}

void MPU6050_Library::calibrateSensor(sensors_event_t *a, sensors_event_t *g, sensors_event_t *t){
    // Calibrar giroscopio y acelerómetro. El sensor tiene que estar inmovil y en una supercifie plana.
    // Leer los datos del MPU6050 3000 veces y calcular el valor medio
    for (int cal_int = 0; cal_int < 3000 ; cal_int ++) {
        // Leer sensor MPU6050
        MPU6050_Library::readSensorData(a, g, t); 
        gyro_X_cal += g->gyro.x;
        gyro_Y_cal += g->gyro.y;
        gyro_Z_cal += g->gyro.z;
        acc_X_cal  += a->acceleration.x;
        acc_Y_cal  += a->acceleration.y;
        acc_Z_cal  += a->acceleration.z;
        delayMicroseconds(50);
    }
    gyro_X_cal = gyro_X_cal / 3000;
    gyro_Y_cal = gyro_Y_cal / 3000;
    gyro_Z_cal = gyro_Z_cal / 3000;

    acc_X_cal  = acc_X_cal  / 3000;
    acc_Y_cal  = acc_Y_cal  / 3000;
    acc_Z_cal  = acc_Z_cal  / 3000;

    accCalibOK = true;
    
    loop_timer = micros();
}

void MPU6050_Library::processingDataOfSensor(sensors_event_t *a, sensors_event_t *g, sensors_event_t *t){
    time_execution = (micros() - loop_timer) / 1000;

    // Restar valores de calibración del acelerómetro
    a->acceleration.x -= acc_X_cal;
    a->acceleration.y -= acc_Y_cal;
    a->acceleration.z -= acc_Z_cal;
    a->acceleration.z  = a->acceleration.z + 4096;

    // Restar valores de calibración del giroscopio y calcular
    // velocidad angular en º/s. Leer 65.5 en raw equivale a 1º/s
    gyro_X = (g->gyro.x - gyro_X_cal) / 65.5;
    gyro_Y = (g->gyro.y - gyro_Y_cal) / 65.5;
    gyro_Z = (g->gyro.z - gyro_Z_cal) / 65.5;

    // Calcular ángulo de inclinación con datos del giroscopio
    // 0.000000266 = tiempo_ejecucion / 1000 / 65.5 * PI / 180
    angulo_pitch += gyro_X * time_execution / 1000;
    angulo_roll  += gyro_Y * time_execution / 1000;
    angulo_pitch += angulo_roll  * sin((g->gyro.z - gyro_Z_cal) * time_execution  * 0.000000266);
    angulo_roll  -= angulo_pitch * sin((g->gyro.z - gyro_Z_cal) * time_execution  * 0.000000266);

    // Calcular vector de aceleración
    // 57.2958 = Conversion de radianes a grados 180/PI
    acc_total_vector  = sqrt(pow(a->acceleration.y , 2) + pow(a->acceleration.x, 2) + pow(a->acceleration.z, 2));
    angulo_pitch_acc  = asin((float)a->acceleration.y  / acc_total_vector) * 57.2958;
    angulo_roll_acc   = asin((float)a->acceleration.x  / acc_total_vector) * -57.2958;

    // Filtro complementario
    if (set_gyro_angles) {
        angulo_pitch = angulo_pitch * 0.99 + angulo_pitch_acc * 0.01;
        angulo_roll  = angulo_roll  * 0.99 + angulo_roll_acc  * 0.01;
    }
    else {
        angulo_pitch = angulo_pitch_acc;
        angulo_roll  = angulo_roll_acc;
        set_gyro_angles = true;
    }
}