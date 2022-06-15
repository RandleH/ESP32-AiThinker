

#include "../schematic.h"
#include "../gpio.h"

void rh_led__on( int led){
    switch(led){
        case BOARD_LED: rh_gpio__set( '#', BOARD_LED_GPIO, 0); break;
        case FLUSH_LED: rh_gpio__set( '#', FLUSH_LED_GPIO, 1); break;
        default: break;
    }
}

void rh_led__off( int led){
    switch(led){
        case BOARD_LED: rh_gpio__set( '#', BOARD_LED_GPIO, 1); break;
        case FLUSH_LED: rh_gpio__set( '#', FLUSH_LED_GPIO, 0); break;
        default: break;
    }
}

void rh_led__toggle( int led){
    switch(led){
        case BOARD_LED: rh_gpio__toggle( '#', BOARD_LED_GPIO); break;
        case FLUSH_LED: rh_gpio__toggle( '#', FLUSH_LED_GPIO); break;
        default: break;
    }
}

void rh_led__loop_test( void ){
    rh_led__off( BOARD_LED );
    rh_led__off( FLUSH_LED );

     while (1){
        rh_led__toggle( BOARD_LED);
        rh_led__toggle( FLUSH_LED); 
        // delay(1000);
    }
}
