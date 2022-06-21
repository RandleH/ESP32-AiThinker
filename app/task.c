




#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "../bsp/schematic.h"
#include "../bsp/dev/led.h"
#include "./tcpip.h"


void rh_task__led_blink   (void* param){
    while(1){
        rh_led__toggle( BOARD_LED);
        vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
    }
}

void rh_task__tcp_server  (void* param){
    rh_tcp__as_server((uint16_t)param);
}