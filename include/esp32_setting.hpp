


boolean settingRead();
void    settingReset();
boolean settingSave();

// ---------------------------------------------------
// Leer el setting.json
// ---------------------------------------------------
boolean settingRead(){
    JsonDocument jsonSettings;
    
    File file = SPIFFS.open("/settings.json", "r");
    DeserializationError error = deserializeJson(jsonSettings, file);

    if(error){
        settingReset();
        log("[ Error ] Fallo la lectura de las configuraciones, tomando valores por defecto");
        log("[ Error ]" + String(error.c_str()) );
        return false;
    }else{
        /**
         * Dispositivo setting.json
         */
        device_config_file = jsonSettings["device_config_file"];
        strlcpy(device_id, jsonSettings["device_id"], sizeof(device_id));
        strlcpy(device_old_user, jsonSettings["device_old_user"], sizeof(device_old_user));
        strlcpy(device_old_password, jsonSettings["device_old_password"], sizeof(device_old_password));

        /**
         * WIFI Cliente setting.json
         */
        wifi_ip_static = jsonSettings["wifi_ip_static"];
        strlcpy(wifi_ssid, jsonSettings["wifi_ssid"], sizeof(wifi_ssid));
        strlcpy(wifi_password, jsonSettings["wifi_password"], sizeof(wifi_password));
        strlcpy(wifi_ipv4, jsonSettings["wifi_ipv4"], sizeof(wifi_ipv4));
        strlcpy(wifi_subnet, jsonSettings["wifi_subnet"], sizeof(wifi_subnet));
        strlcpy(wifi_gateway, jsonSettings["wifi_gateway"], sizeof(wifi_gateway));
        strlcpy(wifi_dns_primary, jsonSettings["wifi_dns_primary"], sizeof(wifi_dns_primary));
        strlcpy(wifi_dns_secondary, jsonSettings["wifi_dns_secondary"], sizeof(wifi_dns_secondary));

        /**
         * WIFI AP setting.json
         */
        ap_mode         = jsonSettings["ap_mode"];
        ap_visibility   = jsonSettings["ap_visibility"];
        ap_chanel       = jsonSettings["ap_chanel"];
        ap_connect      = jsonSettings["ap_connect"];
        strlcpy(app_ssid, jsonSettings["ap_ssid"], sizeof(app_ssid));
        strlcpy(ap_password, jsonSettings["ap_password"], sizeof(ap_password));

        file.close();
        log("[INFO] Lectura de las configuraciones correcta");
        return true;
    }
}

// ---------------------------------------------------
// Valores de Fabrica al setting.json
// ---------------------------------------------------
void settingReset() {

    /**
     * Dispositivo setting.json
     */
    device_config_file = true;
    strlcpy(device_id, "drone_esp32", sizeof(device_id));
    strlcpy(device_old_user, "admin", sizeof(device_old_user));
    strlcpy(device_old_password, "admin", sizeof(device_old_password));

    /**
     * WIFI Cliente setting.json
     */
    wifi_ip_static = false;
    strlcpy(wifi_ssid, "drone_esp32", sizeof(wifi_ssid));
    strlcpy(wifi_password, "123456", sizeof(wifi_password));
    strlcpy(wifi_ipv4, "192.168.0.60", sizeof(wifi_ipv4));
    strlcpy(wifi_subnet, "255.255.255.0", sizeof(wifi_subnet));
    strlcpy(wifi_gateway, "192.168.0.1", sizeof(wifi_gateway));
    strlcpy(wifi_dns_primary, "8.8.8.8", sizeof(wifi_dns_primary));
    strlcpy(wifi_dns_secondary, "8.8.4.4", sizeof(wifi_dns_secondary));

     /**
     * WIFI AP setting.json
     */
    ap_mode         = false;
    ap_visibility   = false;
    ap_chanel       = 9;
    ap_connect      = 4;
    strlcpy(app_ssid, "drone_esp32", sizeof(app_ssid));
    strlcpy(ap_password, "123456", sizeof(ap_password));

}