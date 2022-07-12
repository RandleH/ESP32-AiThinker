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


void setup(){
    
    0==rh_sdio__init()? rh_app__load_fromJSON( rh::app ) : rh_app__load_default( rh::app );
    
    0==rh::app.camera.init() ? RH_CONSOLE("[Y] camera") : RH_CONSOLE("[N] camera");
    0==rh::app.gpio.init()   ? RH_CONSOLE("[Y] gpio")   : RH_CONSOLE("[N] gpio");
    0==rh::app.wifi.init()   ? RH_CONSOLE("[Y] wifi")   : RH_CONSOLE("[N] wifi");

    rh::app.camera.save("/", "img001");
    rh::app.camera.save("/", "img002");
    rh::app.camera.save("/", "img003");
    rh::app.camera.save("/", "img004");

}

void loop (){
    vTaskDelay(100);
}