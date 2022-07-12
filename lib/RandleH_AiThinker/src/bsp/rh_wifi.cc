


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

namespace rh{
int WiFi::init(void){
    if( this->config.isValid ){
        for( auto&item : this->config.list ){
            if( 0==rh_wifi__connect( item.first.c_str(), item.second.c_str()) ){
                this->isConnected = true;
                return 0;
            }
        }
    }
    this->isConnected = false;
    return 1;
}

}


