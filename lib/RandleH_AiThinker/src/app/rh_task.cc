
#include "rh_common.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

namespace rh{

int Task::init(void){
    for( auto&i : this->list ){
        xTaskCreatePinnedToCore( i.second.callback,\
                                 i.second.name    ,\
                                 i.second.stack   ,\
                                 i.second.params  ,\
                                 i.second.priority,\
                                 (TaskHandle_t*)&i.first,\
                                 (BaseType_t)i.second.cpu);
    }
    return 0;
}


}


