
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
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

#include "esp_err.h"

#include "esp_heap_caps.h"

static const char* TAG = "MAIN";
void app_main(void){
    RH_CONSOLE("APP starts!!!");


    rh_gpio__init();
    
    rh_led__off( BOARD_LED );
    rh_led__off( FLUSH_LED );

    

    rh_sdio__init();  // 没有插SD卡会进死循环

    rh_camera__init();
    rh_camera__stop();

    
    rh_wifi__init( "RandleH-WiFi", "123123123", 1, 4 );

    rh_wifi__mode_ap();

    

    while (1) {
        rh_led__toggle( BOARD_LED);
        vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
    }
}



 
