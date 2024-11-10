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


#include "esp32_header.hpp"
#include "esp32_functions.hpp"
#include "esp32_setting.hpp"

#include "mpu6050_library.hpp"

MPU6050_Library mpu;

void setup(void) {
  Serial.begin(9600);
  if (!mpu.begin()) {
    Serial.println("Failed to initialize MPU6050");
    while (1);
  }
  Serial.println("MPU6050 initialized!");
  
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.readSensorData(&a, &g, &temp);

  // Print sensor data as before
  Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);
}