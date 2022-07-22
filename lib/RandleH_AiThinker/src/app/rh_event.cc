
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



}