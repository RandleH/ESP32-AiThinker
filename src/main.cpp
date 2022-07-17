

#include <Firebase_ESP_Client.h>
#include <addons/RTDBHelper.h>

#include <Arduino.h>
#include "rh_common.h"
#include "bsp/rh_gpio.h"
#include "bsp/rh_sdio.h"
#include "bsp/rh_wifi.h"

#include "dev/rh_led.h"
#include "dev/rh_camera.h"

#include "app/rh_rtdb.h"
#include "app/rh_app.h"

#include <WiFi.h>
// #define STORAGE_BUCKET_ID "esp32-wifi-monitor.appspot.com"

namespace rh{
    Application   app(__TIME__);
}



extern FirebaseData fbdo;
extern FirebaseAuth auth;
extern FirebaseConfig config;

extern "C"{
    void rh_wifi__task0(void* params);
}

#define CPU1   1

void setup(){
    0==rh_sdio__init()? rh_app__load_fromJSON( rh::app ) : rh_app__load_default( rh::app );
    
    0==rh::app.camera.init()   ? RH_CONSOLE("[Y] camera")   : RH_CONSOLE("[N] camera");
    0==rh::app.gpio.init()     ? RH_CONSOLE("[Y] gpio")     : RH_CONSOLE("[N] gpio");
    0==rh::app.wifi.init()     ? RH_CONSOLE("[Y] wifi")     : RH_CONSOLE("[N] wifi");
    0==rh::app.database.init() ? RH_CONSOLE("[Y] database") : RH_CONSOLE("[N] database");
        
    rh::app.camera.save("/", "img001");

    rh::app.gpio.event.init();
    rh::app.wifi.event.init();
    RH_CONSOLE("Wait...");
    vTaskDelay(5000);
    rh::app.gpio.task.init();
    rh::app.wifi.task.init();
    
}


#include <sstream>
void loop (){
    static int cnt = 0;
    
    // clock_t cs = clock();
    Firebase.RTDB.setInt(&fbdo, "/val", cnt++);
    // RH_CONSOLE("fb duration: %ld", clock()-cs);

    vTaskDelay(2000);
    // rh::app.database.set("/val", cnt++);
    // stringstream name;
    // name<<"img"<<cnt;
    // rh::app.camera.save("/", name.str().c_str());
    
}