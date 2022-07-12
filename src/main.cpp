#include <Firebase_ESP_Client.h>
#include <FB_Const.h>
#include <Firebase.h>
#include <FB_Utils.h>
#include <FB_Net.h>
#include <FB_Error.h>

#include <FirebaseFS.h>

#include <WiFi.h>


#include "rh_common.h"
#include "bsp/rh_gpio.h"
#include "bsp/rh_sdio.h"
#include "bsp/rh_wifi.h"

#include "dev/rh_led.h"
#include "dev/rh_camera.h"

#include "app/rh_rtdb.h"
#include "app/rh_app.h"

#include <SD_MMC.h>
#include <FS.h>


#define STORAGE_BUCKET_ID "esp32-wifi-monitor.appspot.com"

namespace rh{
    Application   app(__TIME__);
}

#include "esp_camera.h"
void setup(){
    0==rh::app.camera.init();?RH_CONSOLE("success"):RH_CONSOLE("failed");



    // 0==rh_sdio__init()? rh_app__init_fromJSON( rh::app ) : rh_app__init_default( rh::app );
    // size_t len = 0;
    // 0==rh_sdio__query( "/config.json", &len)?RH_CONSOLE("query success, len=%ld", len):RH_CONSOLE("query failed");
    
    // char *buf = (char*)calloc(len+1, sizeof(char));
    // NULL!=buf?RH_CONSOLE("malloc success"):RH_CONSOLE("malloc failed");
    
    // buf[len] = '\0';
    // 0==rh_sdio__load("/config.json", buf, len)?RH_CONSOLE("load success"):RH_CONSOLE("load failed");
    
    

    // rh_gpio__init();
    // rh_led__off(FLUSH_LED);
    // rh_led__off(BOARD_LED);

 
    // 0==rh_wifi__init()?RH_CONSOLE("WiFi connect success"):RH_CONSOLE("WiFi connect failed");
    // 0==rh_camera__init()? RH_CONSOLE("camera init success") : RH_CONSOLE("camera init failed");
    // 0==rh_camera__save2file("/sdcard/img001.jpg")?RH_CONSOLE("save success"):RH_CONSOLE("save failed");

}

void loop (){
    vTaskDelay(100);
}