/**
 * Sitio Web:
 * Correo:
 * Plataforma:
 * Framework:
 * Nombre: ESP32 Drone
 * Version: 1.0.0
 * Autor: Alejandro Joel Ferran Perez
 */

#include <Arduino.h>
#include <ArduinoJson.h>
#include <SPIFFS.h>
#include <EEPROM.h>


#include "esp32_header.hpp"
#include "esp32_functions.hpp"
#include "esp32_setting.hpp"
#include "esp32_wifi.hpp"
#include "esp32_webserver.hpp"

#include "mpu6050_library.hpp"

MPU6050_Library mpu;

void setup(void) {
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);

  Serial.begin(115200);
  setCpuFrequencyMhz(240);

  // Save count of reinitializations
  EEPROM.begin(256);
  EEPROM.get(Restart_Address, device_restart);

  device_restart++;

  EEPROM.put(Restart_Address, device_restart);
  EEPROM.commit();
  EEPROM.end();

  log("[ Info ] Inicialize setup");
  log("[ Info ] Restart: " + String(device_restart));


  // Initialize SPIFFS
  if(!SPIFFS.begin(true)){
    log("[ Error ] SPIFFS mount failed, formatting...");
  }

  // Read settings from SPIFFS
  if(!settingRead()){
    settingSave();
  }

  // Initialize WiFi
  wifiSetup();

  // Initialize web server
  webServerSetup();

  // Initialize MPU6050
  if (!mpu.begin()) {
    log("Failed to initialize MPU6050");
    while (1);
  }
  log("MPU6050 initialized!");
}

void loop() {
  wifiAPLoop();

  sensors_event_t a, g, temp;
  mpu.readSensorData(&a, &g, &temp);
  mpu.showSensorData(&a, &g, &temp);
  Serial.println("===================================");
  delay(500);
}