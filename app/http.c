




#include <esp_http_server.h>
#include "../bsp/schematic.h"

static httpd_handle_t server = NULL;

int rh_http__init(void){
    RH_CONSOLE("%s", __func__);
    httpd_config_t cfg = {
        .task_priority      = tskIDLE_PRIORITY+5,       \
        .stack_size         = 4096,                     \
        .core_id            = tskNO_AFFINITY,           \
        .server_port        = 80,                       \
        .ctrl_port          = 32768,                    \
        .max_open_sockets   = 7,                        \
        .max_uri_handlers   = 8,                        \
        .max_resp_headers   = 8,                        \
        .backlog_conn       = 5,                        \
        .lru_purge_enable   = true,                     \
        .recv_wait_timeout  = 5,                        \
        .send_wait_timeout  = 5,                        \
        .global_user_ctx = NULL,                        \
        .global_user_ctx_free_fn = NULL,                \
        .global_transport_ctx = NULL,                   \
        .global_transport_ctx_free_fn = NULL,           \
        .open_fn = NULL,                                \
        .close_fn = NULL,                               \
        .uri_match_fn = NULL                            \
    };
    if( ESP_OK!=httpd_start(&server, &cfg)){
        RH_CONSOLE("Error starting server!");
        return 1;
    }    
    return 0;
}



int rh_http__server_on( const char* url, int RH_HTTP_METHOD_xxxx, void(*handler)(void*), void* param){
    RH_CONSOLE("%s:", __func__);
    if( url==NULL ) return -1;
    httpd_uri_t cfg = {
        .uri      = url,
        .method   = RH_HTTP_METHOD_xxxx,
        .handler  = (esp_err_t (*)(httpd_req_t *))handler,
        .user_ctx = param
    };
    if( ESP_OK!=httpd_register_uri_handler( server, &cfg)){
        RH_CONSOLE("Register http server failed. url=%s", cfg.uri);
        return 1;
    }
    return 0;
}



#include "../bsp/dev/sdcard.h"

int  rh_http__handler_0(void* param){
    RH_CONSOLE("%s:", __func__);
    char resp_str[2048] = {0};
    
    int ret = rh_sdcard__read("/sdcard/MAIN", "rb", resp_str, 2048);
    if( 0!=ret ){
        RH_CONSOLE("Failed to load main.html. Return value=%d", ret);
    }

    httpd_resp_send((httpd_req_t*)param, resp_str, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

int  rh_http__handler_1(void* param){
    RH_CONSOLE("%s:", __func__);
    httpd_resp_send((httpd_req_t*)param, "han1", HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

int  rh_http__handler_2(void* param){
    RH_CONSOLE("%s:", __func__);
    httpd_resp_send((httpd_req_t*)param, "han2", HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

int  rh_http__handler_3(void* param){
    RH_CONSOLE("%s:", __func__);
    httpd_resp_send((httpd_req_t*)param, "han3", HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

int  rh_http__handler_4(void* param){
    RH_CONSOLE("%s:", __func__);
    httpd_resp_send((httpd_req_t*)param, "han4", HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

int  rh_http__handler_5(void* param){
    RH_CONSOLE("%s:", __func__);
    return ESP_OK;
}
