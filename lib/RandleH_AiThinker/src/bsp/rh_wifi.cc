


#include <WiFi.h>
#include "rh_common.h"
#include "rh_wifi.h"

int rh_wifi__init(void){
    return rh_wifi__connect( "SUES", "TripleUS2637");
}

int rh_wifi__connect(const char* ssid, const char* password){
    RH_CONSOLE("%s", __func__);
    RH_CONSOLE("Connecting Wi-Fi [%s]:[%s]", ssid, password);
    WiFi.begin( ssid, password);
    size_t retry=0;
    while( WL_CONNECTED!=WiFi.status() && retry<1000 ){
        ++retry;
        vTaskDelay(10);
    }
    if( WL_CONNECTED!=WiFi.status() )
    	return -1;

    return 0;
}

#include "app/rh_event.h"
#ifdef __cplusplus
extern "C"{
#endif

// name: "Wi-Fi Status Check"
// type: periodical=1000
void rh_wifi__task0(void* params){
    while(1){
        if( rh::app.wifi.isConnected==false && WL_CONNECTED==WiFi.status() ){
            xEventGroupClearBits(rh::app.wifi.event.handler, 1<<rh::WIFI__NOT_CONNECTED);
            xEventGroupSetBits  (rh::app.wifi.event.handler, 1<<rh::WIFI__CONNECTED);
            rh::app.wifi.isConnected = true;
        }
        if( rh::app.wifi.isConnected==true  && WL_CONNECTED!=WiFi.status() ){
            xEventGroupClearBits(rh::app.wifi.event.handler, 1<<rh::WIFI__CONNECTED);
            xEventGroupSetBits  (rh::app.wifi.event.handler, 1<<rh::WIFI__NOT_CONNECTED);
            rh::app.wifi.isConnected = false;
        }

        vTaskDelay(1000);
    }
}

#ifdef __cplusplus
}
#endif

namespace rh{
int WiFi::init(void){
    if( this->config.isValid ){
        for( auto&item : this->config.list ){
            if( 0==rh_wifi__connect( item.first.c_str(), item.second.c_str()) ){
                goto WIFI_DONE_CONNECTION;
            }
        }
    }

WIFI_DONE_CONNECTION:
    rh::ConfigTask config;

    config.priority = 1;
    config.cpu      = 0;
    config.stack    = 32768;
    config.callback = rh_wifi__task0;
    config.name     = "wifi[0]: Check Status";
    config.params   = NULL;
    this->task.list.push_back( make_pair( nullptr, config) );

    
WIFI_DONE_TASK_ASSIGNMENT:
    
    
    
WIFI_DONE_EVENT_ASSIGNMENT:

    return 0;
}



}


