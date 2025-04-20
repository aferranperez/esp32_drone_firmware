#include <WiFi.h>
#include <DNSServer.h>
#include <ESPmDNS.h>

DNSServer dnsServer;
const byte DNSSERVER_PORT = 53;

// WiFi.mode(WIFI_STA)      - station mode: the ESP32 connects to an access point
// WiFi.mode(WIFI_AP)       - access point mode: stations can connect to the ESP32
// WiFi.mode(WIFI_AP_STA)   - access point and a station connected to another access point

IPAddress ap_IPv4(192, 168, 4, 1);
IPAddress ap_subnet(255, 255, 255, 0);

int wifi_mode = WIFI_AP;
bool wifi_change = false;

unsigned long previousMillisWIFI = 0;
unsigned long previousMillisAP = 0;
unsigned long intervalWIFI = 30000;

const char* esp_hostname = device_id;

// -----------------------------------
// Start AP mode
// -----------------------------------
void startAP(){
    log ("[ Info ] Starting AP mode...");
    log ("[ Info ] AP mode SSID: " + String(app_ssid));
    log ("[ Info ] AP mode Password: " + String(ap_password));

    WiFi.disconnect(true);
    WiFi.mode(WIFI_AP);

    WiFi.softAPConfig(ap_IPv4, ap_IPv4, ap_subnet);
    WiFi.hostname(esp_hostname);

    WiFi.softAP(app_ssid, ap_password, ap_chanel, ap_visibility, ap_connect);
    dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure);
    dnsServer.start(DNSSERVER_PORT, "*", ap_IPv4);
    wifi_mode = WIFI_AP;
}

void startClient(){
    
}

// -----------------------------------
// Inicialize WiFi setup
// -----------------------------------
void wifiSetup(){
    log("[ Info ] Inicialize WiFi setup");
    log("[ Info ] WiFi mode: " + String(wifi_mode));

    WiFi.disconnect(true);

    if(ap_mode){
        startAP();
        log("[ Info ] Wifi in AP mode");
    }
    
    // Initialize hostname broadcast 
    if(wifi_mode == WIFI_STA || wifi_mode == WIFI_AP){
        if(MDNS.begin(esp_hostname)){
            MDNS.addService("http", "tcp", 80);
        }
    }

}

// -----------------------------------
// WifiAP loop
// -----------------------------------
void wifiAPLoop(){
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillisAP >= intervalWIFI) {
        previousMillisAP = currentMillis;

        // Captive portal
        dnsServer.processNextRequest();
    }
}

