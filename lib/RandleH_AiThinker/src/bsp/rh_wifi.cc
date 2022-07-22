


#include <WiFi.h>
#include "rh_common.h"
#include "rh_wifi.h"

#include "app/rh_event.h"

extern WiFiClass WiFi;
static WiFiClass &ArduinoWiFi = WiFi;

namespace rh{

int WiFi::init(void){
    ArduinoWiFi.mode(WIFI_MODE_APSTA);
    begin();
    return 0;
}

int WiFi::register_event(void){
    event.init();
    ArduinoWiFi.onEvent([](WiFiEvent_t event){
        switch(event){
            case ARDUINO_EVENT_WIFI_STA_CONNECTED:
                if(rh::app.wifi.sta_connected==false){
                    xEventGroupClearBits(rh::app.wifi.event.handler, 1<<rh::WIFI__STA_DISCONNECTED);
                    xEventGroupSetBits  (rh::app.wifi.event.handler, 1<<rh::WIFI__STA_CONNECTED);
                    rh::app.wifi.sta_connected = true;
                }break;
            case ARDUINO_EVENT_WIFI_AP_STACONNECTED:
                if(rh::app.wifi.sta_connected==true){
                    xEventGroupSetBits   (rh::app.wifi.event.handler, 1<<rh::WIFI__STA_DISCONNECTED);
                    xEventGroupClearBits (rh::app.wifi.event.handler, 1<<rh::WIFI__STA_CONNECTED);
                    rh::app.wifi.sta_connected = false;
                }break;
            case ARDUINO_EVENT_WIFI_AP_STADISCONNECTED:
                if(0){

                }break;
            case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
                if(0){

                }break;
            default:
                RH_CONSOLE("ARDUINO_EVENT_WIFI_XXXX=%d", event);
        }

    });
    return 0;
}

int WiFi::begin(void){
    config.isValid? wlan   ( config.ap   ) : wlan   ("ESP32-AiThinker"   , "");
    config.isValid? connect( config.list ) : connect("USC Guest Wireless", "");
    return 0;
}

int WiFi::connect(const char* ssid, const char* password){
    ArduinoWiFi.begin( ssid, password);
    size_t retry=0;
    while( WL_CONNECTED!=ArduinoWiFi.status() && retry<1000 ){
        ++retry;
        vTaskDelay(10);
    }
    if( WL_CONNECTED!=ArduinoWiFi.status() )
    	return -1;
    return 0;
}

int WiFi::connect(const vector<pair<string,string>>& list){
    for( auto &item : list ){
        if( 0==connect( item.first.c_str(), item.second.c_str()) ){
            this->ssid = item.first;
            this->sta_connected = true;
            return 0;
        }
    }
    return -1;
}

int WiFi::wlan(const pair<string,string>& ap){
    return true!=ArduinoWiFi.softAP( ap.first.c_str(), ap.second.c_str());
}

int WiFi::wlan(const char* ssid, const char* password){
    return true!=ArduinoWiFi.softAP( ssid, password);
}



}


