

#include "esp_event.h"

#include "../bsp/schematic.h"


#include "freertos/FreeRTOS.h"
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>

void rh_tcp__as_server( uint16_t port){
    RH_CONSOLE("%s:", __func__);
    struct sockaddr_storage  server;
    ((struct sockaddr_in*)&server)->sin_addr.s_addr = htonl(INADDR_ANY);
    ((struct sockaddr_in*)&server)->sin_family      = AF_INET;
    ((struct sockaddr_in*)&server)->sin_port        = htons(port);
    
    int sock_ser = socket( AF_INET, SOCK_STREAM, IPPROTO_IP);
    
    int opt = 1;
    setsockopt( sock_ser, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    int ret = bind( sock_ser, (struct sockaddr*)&server, sizeof(server));
    if( ret!=0 ){
        RH_CONSOLE("Unable to bind\n");
        goto CLEAN_UP;
    }

    ret = listen( sock_ser, 1);
    if( ret!=0 ){
        RH_CONSOLE("Unable to listen\n");
        goto CLEAN_UP;
    }
    
    char addr_cli[128] = {0};
    while(1){
        struct sockaddr_storage  client = {0};
        size_t                   size   = sizeof(client);

        // 接受对方响应
        int sock_cli = accept( sock_ser, (struct sockaddr *)&client, &size);
        if( sock_cli<0 ){
            RH_CONSOLE("Unable to accept\n");
            break;
        }

        // 获取对方IP地址
        if(client.ss_family==PF_INET){
            inet_ntoa_r( ((struct sockaddr_in*)&client)->sin_addr, addr_cli, sizeof(addr_cli)-1);
        }else if(client.ss_family==PF_INET6){
            inet_ntoa_r( ((struct sockaddr_in6*)&client)->sin6_addr, addr_cli, sizeof(addr_cli)-1);
        }
        addr_cli[sizeof(addr_cli)-1]='\0';
        RH_CONSOLE("Accept the client from %s", addr_cli);

        // 传输数据

        // 关闭会话
        shutdown( sock_cli, 0);
        close( sock_cli);
    }

CLEAN_UP:    
    close( sock_ser);
    // vTaskDelete(NULL);
}






typedef struct{
    void*             ev_handler_arg;
    esp_event_base_t  ev_base; 
    int32_t           ev_id;
    void*             ev_data; 
}callback_handler_t;

void rh_tcpip__handler_0(void* args){
	RH_CONSOLE("%s:", __func__);
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

