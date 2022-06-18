
#include "esp_event.h"
#include "../bsp/schematic.h"

typedef struct{
    void*             ev_handler_arg;
    esp_event_base_t  ev_base; 
    int32_t           ev_id;
    void*             ev_data; 
}callback_handler_t;

void rh_tcpip__handler_0(void* args){
	RH_CONSOLE("TCPIP Handler 0\n");
	callback_handler_t *args_ = (callback_handler_t*)args;
    ip_event_got_ip_t* event = (ip_event_got_ip_t*)args_->ev_data;
    RH_CONSOLE("got ip:%d.%d.%d.%d\n", IP2STR(&event->ip_info.ip));
}
void rh_tcpip__handler_1(void* args){

}
void rh_tcpip__handler_2(void* args){

}
void rh_tcpip__handler_3(void* args){

}
void rh_tcpip__handler_4(void* args){

}
void rh_tcpip__handler_5(void* args){

}
void rh_tcpip__handler_6(void* args){

}
void rh_tcpip__handler_7(void* args){

}

