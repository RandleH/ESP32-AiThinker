
#include "rh_common.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

namespace rh{

int Event::init(void){
    this->wifi = (void*)xEventGroupCreate();
    if( !this->wifi ){
        RH_CONSOLE("Failed to create Wi-Fi event");
        return -1;
    } 

    return 0;
}

int Event::set(rh::EG_WiFi_t event){
    EventBits_t bit;
    switch(event){
        case rh::WIFI__CONNECTED:
            bit = 1<<0;break;
        default: return 1;
    }

    xEventGroupSetBits( this->wifi, bit);
    return 0;
}

}