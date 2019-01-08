

#include <ArduinoRGBLed.h>
#include <FS.h>
#include<ESP8266WiFi.h>
#include <ArduinoOTA.h>
#include <ArduinoJson.h>
#include "Led.h"
#include<ESP8266mDNS.h>
#include <ESPAsyncUDP.h>
#include "global.h"
#include  "IotService.h"
#include "DeviceSettings.h"

extern "C" {
#include "user_interface.h"
}



#define RED 14
#define GREEN 16
#define BLUE  12

/* Wifi event handlers are called as long as handler objects are in scope: hence global */
WiFiEventHandler onConnectHandler;
WiFiEventHandler onDisconnectHandler;
AsyncUDP udp;
ArduinoRGBLed led(RED, GREEN, BLUE);
LED system_led(2, ACTIVE_LOW);

int led_blink_time = 1000;

int lastUpdate = 0;



void setup() {
  /* Enable Serial Logging First */
  DEBUG.begin(115200);
  DEBUG.println("\n\n\n\n*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*\n\n");
  DEBUG.println("Esp8266 Application booting..");

  /* Read Configuration Second */
  spiffsInit();

  /** Device settings should be initialised at very begining **/
  DeviceSettings::instance()->init();

  if (DeviceSettings::instance()->getMode() == "AP")
  {
    /** Dont blink remain on **/
    led_blink_time = 0;
  }
  else if (DeviceSettings::instance()->getMode() = "STA")
  {
    led_blink_time = 1000;
  }
  else
  {
    led_blink_time = 200;
  }



  /* Setup Wifi based on configuration */
  wifiInit(DeviceSettings::instance());

  /* Start UDP server for commands */
  udpServerInit();

  /* Setup ota based on read configuration */
  otaSetup();


  // Add your initialization code here
  led.setMode(RGB_DROWSY);
  led.setColor(RGB_GREEN);
  lastUpdate = millis();


  /* Advertise yourself as iot device on the network */
  MDNS.addService("iot", "udp", UDP_SERVER_LISTEN_PORT);
  MDNS.addServiceTxt("iot", "udp", "name", DeviceSettings::instance()->getHostname());
  DEBUG.println("Esp8266 Boot complete..");
}

void loop() {
  // put your main code here, to run repeatedly:

  if (led_blink_time && millis() - AppConfig::led_update_ts > led_blink_time) {
    AppConfig::led_update_ts = millis();
    system_led.toggle();
  } else if (led_blink_time == 0) {
    system_led.ON();
  }

  if ((millis() - lastUpdate) > 10000) {
    if (led.getColor() == RGB_RED)
      led.setColor(RGB_BLUE);
    else if (led.getColor() == RGB_BLUE)
      led.setColor(RGB_GREEN);
    else if (led.getColor() == RGB_GREEN)
      led.setColor(RGB_WHITE);
    else if (led.getColor() == RGB_WHITE)
      led.setColor(RGB_YELLOW);
    else if (led.getColor() == RGB_YELLOW)
      led.setColor(RGB_VIOLET);
    else if (led.getColor() == RGB_VIOLET)
      led.setColor(RGB_INDIGO);
    else if (led.getColor() == RGB_INDIGO)
      led.setColor(RGB_RED);

    lastUpdate = millis();
  }

  delay(20);


  IotService::update(udp);
  ArduinoOTA.handle();

}
