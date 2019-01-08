
#ifndef GLOBAL_H
#define GLOBAL_H
/**
    Global Definitions
*/
#define DEBUG Serial
#define UDP_SERVER_LISTEN_PORT 1234
#define SWITCH_PIN 4

#define CONFIG_FILE "/config.conf"

/*  Global application data */
struct AppConfig {


  static bool wifi_disconnected;

  static unsigned int led_update_ts;

};

bool AppConfig::wifi_disconnected = true;
unsigned int AppConfig::led_update_ts = 0;



/**
   Global variables
*/

extern WiFiEventHandler onConnectHandler;
extern WiFiEventHandler onDisconnectHandler;

class Config;

/**
   Function Declarations
*/

void spiffsInit();
void wifiInit(const Config& conf);
void udpServerInit();
void otaSetup();

#endif
