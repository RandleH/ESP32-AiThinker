#include <Firebase_ESP_Client.h>
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
#include "bsp/rh_sdio.h"
#include "bsp/rh_wifi.h"

#include "dev/rh_led.h"
#include "dev/rh_camera.h"

#include "app/rh_rtdb.h"

#include <SD_MMC.h>
#include <FS.h>


#define STORAGE_BUCKET_ID "esp32-wifi-monitor.appspot.com"

void setup(){
  
    rh_gpio__init();
    rh_led__off(FLUSH_LED);
    rh_led__off(BOARD_LED);

    rh_sdio__init();

    0==rh_wifi__init()?RH_CONSOLE("WiFi connect success"):RH_CONSOLE("WiFi connect failed");
    0==rh_camera__init()? RH_CONSOLE("camera init success") : RH_CONSOLE("camera init failed");
    0==rh_camera__save2file("/sdcard/img001.jpg")?RH_CONSOLE("save success"):RH_CONSOLE("save failed");

    0==rh_rtdb__init(FIREBASE_HOST,FIREBASE_AUTH)?RH_CONSOLE("database success"):RH_CONSOLE("database failed");
    
}

void loop (){
    // vTaskDelay(100);
    // rh_led__on(FLUSH_LED);
    // rh_led__off(FLUSH_LED);
    vTaskDelay(100);
}