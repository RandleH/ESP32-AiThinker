#include <FirebaseESP32.h>
#include <FB_Const.h>
#include <Firebase.h>
#include <FB_Utils.h>
#include <FB_Net.h>
#include <FB_Error.h>

#include <FirebaseFS.h>

#include <WiFi.h>

#define WIFI_SSID     "USC Guest Wireless" // your wifi SSID
#define WIFI_PASSWORD "" //your wifi PASSWORD

#define LedPin 4         // pin d4 as toggle pin

#define FIREBASE_HOST "esp32-wifi-monitor-default-rtdb.firebaseio.com" // change here
#define FIREBASE_AUTH "JEScAl2BJFIo5goTturXfL76y0eaCJiXBbx1wRgr"       // your private key
FirebaseData firebaseData;

#include "rh_common.h"
#include "bsp/rh_gpio.h"
#include "dev/rh_led.h"


#define RH(format, ...)  Serial.printf("RH:"format, ##__VA_ARGS__)

void setup(){
  
    rh_gpio__init();
    rh_led__off(FLUSH_LED);
    rh_led__off(BOARD_LED);

  
  
}

void loop (){
  vTaskDelay(100);
  rh_led__on(FLUSH_LED);
  vTaskDelay(100);
  rh_led__off(FLUSH_LED);
}