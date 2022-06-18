

#include "esp_event.h"
#include "./event.h"

#include "../bsp/schematic.h"

typedef struct{
    void*             ev_handler_arg;
    esp_event_base_t  ev_base; 
    int32_t           ev_id;
    void*             ev_data 
}callback_handler_t;

/*================================================================================
系统事件总管: Wi-Fi
================================================================================*/
static void(*rh_event_wifi[32])(void*)={NULL};  // Callback函数数组 - Wi-Fi
static void rh_event__wifi_handler( void* ev_handler_arg, esp_event_base_t ev_base, int32_t ev_id, void* ev_data ){
    RH_CONSOLE("Wifi event received\n");
    if( ev_id >= 32 ) return;
    if( rh_event_wifi[ev_id]!=NULL ){
        callback_handler_t args = {
            .ev_handler_arg = ev_handler_arg,
            .ev_base        = ev_base,
            .ev_id          = ev_id,
            .ev_data        = ev_data
        };
        rh_event_wifi[ev_id](&args);
    }
}

/*================================================================================
系统事件总管: TCPIP协议事件
================================================================================*/
static void(*rh_event_tcpip[32])(void*)={NULL};  // Callback函数数组 - TCPIP
static void rh_event__tcpip_handler( void* ev_handler_arg, esp_event_base_t ev_base, int32_t ev_id, void* ev_data ){
    RH_CONSOLE("IP event received\n");
    if( ev_id >= 32 ) return;
    if( rh_event_tcpip[ev_id]!=NULL ){
        callback_handler_t args = {
            .ev_handler_arg = ev_handler_arg,
            .ev_base        = ev_base,
            .ev_id          = ev_id,
            .ev_data        = ev_data
        };
        rh_event_tcpip[ev_id](&args);
    }
}



int  rh_event__init  (void){
    // 初始化ESP32系统默认事件组
    esp_event_loop_create_default();

    esp_event_base_t              ev_base;
    int32_t                       ev_id  ;
    esp_event_handler_t           ev_func;
    void*                         ev_args;
    esp_event_handler_instance_t  ev_ins ;

    // 初始化事件: Wi-Fi 
    ev_base = WIFI_EVENT;
    ev_id   = ESP_EVENT_ANY_ID;
    ev_func = rh_event__wifi_handler;
    ev_args = NULL;
    ev_ins  = NULL;
    if( ESP_OK!=esp_event_handler_instance_register( ev_base, ev_id, ev_func, ev_args, &ev_ins))    return 1;
    
    // 初始化事件: TCPIP
    ev_base = IP_EVENT;
    ev_id   = ESP_EVENT_ANY_ID;
    ev_func = rh_event__tcpip_handler;
    ev_args = NULL;
    ev_ins  = NULL;
    if( ESP_OK!=esp_event_handler_instance_register( ev_base, ev_id, ev_func, ev_args, &ev_ins))    return 1;
    
    return 0; 
}

void rh_event__insert( int RH_APP_EVENT_xxxx, int id, void(*func)(void*) ){
    if(id>=32) return;
    
    switch( RH_APP_EVENT_xxxx ){
        case RH_APP_EVENT_WIFI:{
            rh_event_wifi[id] = func; 
            break;        
        }
        case RH_APP_EVENT_TCPIP:{
            rh_event_tcpip[id] = func;
            break;
        }
        default:
            break;
        
    }
}









