
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
// De HEX a String
// -----------------------------------
String hexStr(const unsigned long &h, const byte &L = 8){
    String s;
    s = String(h, HEX);
    s.toUpperCase();
    s = ("00000000" + s).substring(s.length() + 8 - L);
    return s;
}

// ------------------------------------
// Crear un ID unico desde la direccion MAC
// ------------------------------------
String idUnique(){
    // Retorna los ultimos 4 Bytes del MAC rotados
    char idunique[15];
    uint64_t chipid = ESP.getEfuseMac();
    uint16_t chip = (uint16_t)(chipid >> 32);
    sniprintf(idunique, 15, "%04x", chip);
    return idunique;
}


// -------------------------------------
// Numero de serie del dispositivo
// Se garantiza que el ID sea unico en cada dispositivo
// -------------------------------------
String deviceID(){
    // return String( platform()) + hexStr(ESP.getEfuseMac() + idUnique());
    return String( platform()) + hexStr(ESP.getEfuseMac());
}