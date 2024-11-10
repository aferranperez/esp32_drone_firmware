/**
 * Definimos los pines I/O del ESP32 
 **/ 
#define LED 12
#define MQTTLED 13

/**
 * Calcular la capacidad del JSON
 */
const size_t capacitySetting = 1536;

/**
 * Zona de Configuracion del Dispositivo
 */
boolean     device_config_file;
char        device_id[30];
int         device_restart;
char        device_old_user[15];
char        device_old_password[15];
uint8_t     ip[4];


/**
 * Zona de configuracion de Wifi modo Cliente
 */
boolean     wifi_ip_static;
char        wifi_ssid[30];
char        wifi_password[30];
char        wifi_ipv4[15];
char        wifi_gateway[15];
char        wifi_subnet[15];
char        wifi_dns_primary[15];
char        wifi_dns_secondary[15];

/**
 * Zona de configuracion de Wifi modo AP
 */
boolean     ap_mode;
char        app_ssid[31];
char        ap_password[63];
int         ap_chanel;
int         ap_visibility;
int         ap_connect;