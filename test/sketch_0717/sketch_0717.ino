

#include <WiFi.h>
#include <ESPAsyncWebServer.h>



void rh_wifi__event( WiFiEvent_t event){
    switch(event){
        case SYSTEM_EVENT_STA_CONNECTED:
            Serial.printf("STA connected\n");break;
        case SYSTEM_EVENT_AP_START:
            Serial.printf("AP connected\n");break;
        case SYSTEM_EVENT_AP_STACONNECTED:
            Serial.printf("APSTA connected\n");break;
        case SYSTEM_EVENT_AP_STADISCONNECTED:
            Serial.printf("APSTA disconnected\n");break;
        default:
            Serial.printf("E:%d\n", event);break;              
    }
}

void rh_server__handler( AsyncWebServerRequest *request ){
    if( ON_STA_FILTER(request) ){
        request->send( 200, "text/plain", "Hello from STA");
        return;
    }else if( ON_AP_FILTER(request) ){
        request->send( 200, "text/plain", "Hello from AP");
        return;
    }
    request->send( 200, "text/plain", "Hello from undefined");
}

AsyncWebServer server(80);


void setup() {
    Serial.begin(115200);
  // put your setup code here, to run once:
    WiFi.mode( WIFI_MODE_APSTA );
    WiFi.softAP( "ESP32-AiThinker", "0629johnny");    
    WiFi.begin( "USC Guest Wireless", nullptr );
    WiFi.onEvent(rh_wifi__event);

    Serial.print("Connecting...");
    while( WL_CONNECTED!=WiFi.status() ){
        delay(500);
        Serial.print(".");
    }

    server.on("/hello", HTTP_GET, rh_server__handler);
    server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:

}
