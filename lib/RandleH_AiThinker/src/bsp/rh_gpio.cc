
#include "driver/gpio.h"
#include "hal/gpio_hal.h"
#include "soc/io_mux_reg.h"
#include "soc/gpio_sig_map.h"
#include "Arduino.h"
#include "rh_common.h"

int  rh_gpio__init   (void){
    pinMode(BOARD_LED_GPIO, OUTPUT);
    digitalWrite( BOARD_LED_GPIO, 1);
    return 0;
}

void rh_gpio__set    ( char gpio, int pin, int value){
    gpio_set_level( (gpio_num_t)pin, value);
}

int  rh_gpio__get    ( char gpio, int pin){
	return gpio_get_level((gpio_num_t)pin);
}

void rh_gpio__toggle ( char gpio, int pin){
    if( pin < 32 )
        (&GPIO)->out      ^= (1<<pin);
    else
        (&GPIO)->out1.val ^= (1<<pin);   
}




#include "dev/rh_led.h"
#include "app/rh_event.h"

#ifdef __cplusplus
extern "C"{
#endif

void rh_gpio__task0(void*){
    while(1){
        EventBits_t ret = xEventGroupWaitBits( rh::app.wifi.event.handler,
                                               (1<<rh::WIFI__STA_CONNECTED)|(1<<rh::WIFI__STA_DISCONNECTED),\
                                               pdTRUE,           // xClearOnExit
                                               pdFALSE,          // xWaitForAllBits
                                               portMAX_DELAY);
                                            
        if( ret & (1<<rh::WIFI__STA_CONNECTED) ){
            rh_led__on(BOARD_LED);
        }
        if( ret & (1<<rh::WIFI__STA_DISCONNECTED) ){
            rh_led__off(BOARD_LED);
        }
    }
}

#ifdef __cplusplus
}
#endif


namespace rh{

int GPIO::init(void){
    if( this->config.isValid ){
        for( auto&item:this->config.list){
            pinMode( std::get<0>(item), std::get<1>(item));
            if( std::get<1>(item)==INPUT )
                digitalWrite( std::get<0>(item), std::get<2>(item));
        }
        goto GPIO_CONFIG_DONE;
    }
    rh_gpio__init();

GPIO_CONFIG_DONE:
    
    return 0;
}

int GPIO::register_event(void){
    return 0;
}

int GPIO::register_task(void){
    ConfigTask ct;
    ct.callback = rh_gpio__task0;
    ct.name     = "gpio[0]: LED";
    ct.params   = NULL;
    ct.priority = 1;
    ct.stack    = 4096;
    ct.cpu      = 0;
    this->task.list.push_back( make_pair( nullptr, ct) );
    
    return this->task.init();
}

}

