/**
 * Sitio Web:
 * Correo:
 * Plataforma:
 * Framework:
 * Nombre:
 * Autor:
 */


#include <Arduino.h>
#include <ArduinoJson.h>
#include <SPIFFS.h>
#include <Wire.h>


#include "esp32_header.hpp"
#include "esp32_functions.hpp"
#include "esp32_setting.hpp"

#include "mpu6050_library.hpp"

#define usCiclo 5000  // Ciclo de ejecución de software en microsegundos

MPU6050_Library mpu;

void setup(void) {
  Serial.begin(9600);

  if (!mpu.begin()) {
    Serial.println("Failed to initialize MPU6050");
    while (1);
  }
  mpu.calibrateSensor(&mpu.a, &mpu.g, &mpu.temp);
  Serial.println("MPU6050 initialized!");


}

void loop() {
  while (micros() - mpu.loop_timer < usCiclo);
  mpu.time_execution = (micros() - mpu.loop_timer) / 1000;
  mpu.loop_timer = micros();


  mpu.readSensorData(&mpu.a, &mpu.g, &mpu.temp);
  mpu.processingDataOfSensor(&mpu.a, &mpu.g, &mpu.temp);

  // Monitor Serie
  Serial.print(mpu.angulo_pitch);
  Serial.print("\t");
  Serial.println(mpu.angulo_roll);

}