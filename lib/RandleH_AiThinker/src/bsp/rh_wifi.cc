


#include <WiFi.h>

#include "rh_wifi.h"

int rh_wifi__init(void){
    return rh_wifi__connect( "SUES", "TripleUS2637");
}

int rh_wifi__connect(const char* ssid, const char* password){
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




