
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"
#include "sdkconfig.h"

#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>


#include "../bsp/schematic.h"
#include "../bsp/gpio.h"
#include "../bsp/sdio.h"
#include "../bsp/i2s.h"
#include "../bsp/misc.h"
#include "../bsp/wifi.h"

#include "../bsp/dev/led.h"
#include "../bsp/dev/sdcard.h"
#include "../bsp/dev/camera.h" 

#include "../app/event.h"
#include "../app/tcpip.h"

#include "esp_err.h"
#include "esp_event.h"


void app_main(void){
    RH_CONSOLE("APP starts!!!"); 
/*===============================================================
初始化 物理层
===============================================================*/    
    rh_gpio__init();
    rh_wifi__init();
    rh_sdio__init();
    rh_camera__init();
    
/*===============================================================
初始化 应用层
===============================================================*/ 
    rh_event__init();
    rh_event__insert( RH_APP_EVENT_WIFI, RH_WIFI_EVENTID__READY               , rh_wifi__handler_0  );
    rh_event__insert( RH_APP_EVENT_WIFI, RH_WIFI_EVENTID__SCAN_DONE           , rh_wifi__handler_1  );
    rh_event__insert( RH_APP_EVENT_WIFI, RH_WIFI_EVENTID__STA_START           , rh_wifi__handler_2  );
    rh_event__insert( RH_APP_EVENT_WIFI, RH_WIFI_EVENTID__STA_STOP            , rh_wifi__handler_3  );

    rh_event__insert( RH_APP_EVENT_WIFI, RH_WIFI_EVENTID__STA_CONNECTED       , rh_wifi__handler_4  );
    rh_event__insert( RH_APP_EVENT_WIFI, RH_WIFI_EVENTID__STA_DISCONNECTED    , rh_wifi__handler_5  );
    rh_event__insert( RH_APP_EVENT_WIFI, RH_WIFI_EVENTID__STA_AUTHMODE_CHANGE , rh_wifi__handler_6  );
    rh_event__insert( RH_APP_EVENT_WIFI, RH_WIFI_EVENTID__STA_WPS_ER_SUCCESS  , rh_wifi__handler_7  );
    rh_event__insert( RH_APP_EVENT_WIFI, RH_WIFI_EVENTID__STA_WPS_ER_FAILED   , rh_wifi__handler_8  );

    rh_event__insert( RH_APP_EVENT_WIFI, RH_WIFI_EVENTID__AP_START            , rh_wifi__handler_12 );
    rh_event__insert( RH_APP_EVENT_WIFI, RH_WIFI_EVENTID__AP_STOP             , rh_wifi__handler_13 );
    rh_event__insert( RH_APP_EVENT_WIFI, RH_WIFI_EVENTID__AP_STACONNECTED     , rh_wifi__handler_14 );
    rh_event__insert( RH_APP_EVENT_WIFI, RH_WIFI_EVENTID__AP_STADISCONNECTED  , rh_wifi__handler_15 );

    rh_event__insert( RH_APP_EVENT_TCPIP, RH_TCPIP_EVENTID__STA_GOT_IP , rh_tcpip__handler_0  );
    
/*===============================================================
启动 物理层
===============================================================*/ 
    rh_wifi__mode_sta( "SUES", "TripleUS2637", 1);
    rh_wifi__start();
    
    while (1) {
        rh_led__toggle( BOARD_LED);
        vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
    }
}



 
