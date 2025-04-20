
// -----------------------------------
// Genera un log en el puerto Serial
// -----------------------------------
void log(String s){
    Serial.println(s);
}

// -----------------------------------
// Definir la plataforma
// -----------------------------------
String platform(){
    #ifdef ARDUINO_ESP32_DEV
        return "ESP32";
    #endif
}

// -----------------------------------
// Convert HEX to String
// -----------------------------------
String hexStr(const unsigned long &h, const byte &L = 8){
    String s;
    s = String(h, HEX);
    s.toUpperCase();
    s = ("00000000" + s).substring(s.length() + 8 - L);
    return s;
}

// ------------------------------------
// Create a unique ID for the device
// ------------------------------------
String idUnique(){
    // Return the last 4 bytes of the MAC rotated
    char idunique[15];
    uint64_t chipid = ESP.getEfuseMac();
    uint16_t chip = (uint16_t)(chipid >> 32);
    sniprintf(idunique, 15, "%04x", chip);
    return idunique;
}


// -------------------------------------
// Number of the device
// Garantees that the ID is unique on each device
// -------------------------------------
String deviceID(){
    return String(platform()) + hexStr(ESP.getEfuseMac()) + String(idUnique());
    //return String( platform()) + hexStr(ESP.getEfuseMac());
}