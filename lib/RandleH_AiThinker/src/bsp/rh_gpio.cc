
#include "driver/gpio.h"
#include "hal/gpio_hal.h"
#include "soc/io_mux_reg.h"
#include "soc/gpio_sig_map.h"
#include "Arduino.h"
#include "rh_common.h"

void rh_gpio__init   (void){
  pinMode(BOARD_LED_GPIO, OUTPUT);
  pinMode(FLUSH_LED_GPIO, OUTPUT);
  
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




