
#include "rh_common.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

namespace rh{

int Event::init(void){
    this->handler = (void*)xEventGroupCreate();
    if( !this->handler ){
        return -1;
    }
    return 0;
}

// int Event::set(rh::EG_WiFi_t event){
//     EventBits_t bit;
//     switch(event){
//         case rh::WIFI__CONNECTED:
//             bit = 1<<0;break;
//         default: return 1;
//     }

//     xEventGroupSetBits( this->handler, bit);
//     return 0;
// }

}