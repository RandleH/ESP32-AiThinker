
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

#include "../bsp/dev/led.h"
#include "../bsp/dev/sdcard.h"
#include "../bsp/dev/camera.h" 



#include "esp_heap_caps.h"


void app_main(void){

    rh_gpio__init();
    
    rh_led__off( BOARD_LED );
    rh_led__off( FLUSH_LED );

    rh_sdio__init();  // 没有插SD卡会进死循环

    rh_camera__init();
    rh_camera__start();
    // rh_camera__save_bmp("/sdcard/test.bmp");
    rh_camera__save_raw("/sdcard/test.bin");
    rh_camera__save_jpg("/sdcard/test.jpg");

    while (1) {
        rh_led__toggle( BOARD_LED);
        vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
    }
}



 
