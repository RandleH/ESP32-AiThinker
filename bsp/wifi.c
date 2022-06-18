

#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#include "esp_wifi.h"
#include "esp_event.h"
#include  "nvs_flash.h"
#include "./schematic.h"


int  rh_wifi__init(void){
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }

    wifi_init_config_t wifi_init_cfg = {  // WIFI_INIT_CONFIG_DEFAULT()
        .event_handler         = &esp_event_send_internal, \
        .osi_funcs             = &g_wifi_osi_funcs, \
        .wpa_crypto_funcs      = g_wifi_default_wpa_crypto_funcs, \
        .static_rx_buf_num     = CONFIG_ESP32_WIFI_STATIC_RX_BUFFER_NUM,\
        .dynamic_rx_buf_num    = CONFIG_ESP32_WIFI_DYNAMIC_RX_BUFFER_NUM,\
        .tx_buf_type           = CONFIG_ESP32_WIFI_TX_BUFFER_TYPE,\
        .static_tx_buf_num     = WIFI_STATIC_TX_BUFFER_NUM,\
        .dynamic_tx_buf_num    = WIFI_DYNAMIC_TX_BUFFER_NUM,\
        .cache_tx_buf_num      = WIFI_CACHE_TX_BUFFER_NUM,\
        .csi_enable            = WIFI_CSI_ENABLED,\
        .ampdu_rx_enable       = WIFI_AMPDU_RX_ENABLED,\
        .ampdu_tx_enable       = WIFI_AMPDU_TX_ENABLED,\
        .amsdu_tx_enable       = WIFI_AMSDU_TX_ENABLED,\
        .nvs_enable            = WIFI_NVS_ENABLED,\
        .nano_enable           = WIFI_NANO_FORMAT_ENABLED,\
        .rx_ba_win             = WIFI_DEFAULT_RX_BA_WIN,\
        .wifi_task_core_id     = WIFI_TASK_CORE_ID,\
        .beacon_max_len        = WIFI_SOFTAP_BEACON_MAX_LEN, \
        .mgmt_sbuf_num         = WIFI_MGMT_SBUF_NUM, \
        .feature_caps          = g_wifi_feature_caps, \
        .sta_disconnected_pm   = WIFI_STA_DISCONNECTED_PM_ENABLED,  \
        .magic                 = WIFI_INIT_CONFIG_MAGIC\
    };

    if( ESP_OK!=esp_wifi_init(&wifi_init_cfg))  { RH_CONSOLE("wifi init failed");return 1;}

    return ret!=ESP_OK;
}

int  rh_wifi__mode_ap   ( const char* ssid, const char* password, int channel){
    if( ESP_OK!=esp_netif_init())                return 1;
    esp_netif_create_default_wifi_ap();

    if( ESP_OK!=esp_wifi_set_mode(WIFI_MODE_AP)      )   {RH_CONSOLE("wifi ap mode failed");return 1;}
    if( !ssid || strlen(ssid)==0 || strlen(ssid)>=32 )   return 1;
    
    wifi_config_t wifi_config = {
        .ap = {
            .ssid           = {0},
            .password       = {0},
            .ssid_len       = strlen(ssid),
            .channel        = channel,
            .max_connection = 5,
            .authmode       = WIFI_AUTH_WPA_WPA2_PSK
        },
    };
    strcpy( (char*)wifi_config.ap.ssid, ssid);
    if( password!=NULL && strlen(password)!=0 ) {
        strcpy( (char*)wifi_config.ap.password, password);
        RH_CONSOLE("Wifi: name=%s", ssid);
        RH_CONSOLE("Wifi: password=%s", password);
    }else{
        wifi_config.ap.authmode = WIFI_AUTH_OPEN;
        RH_CONSOLE("No password");
    }
    if( ESP_OK!=esp_wifi_set_config(WIFI_IF_AP, &wifi_config)) return 1;

    return 0;
}

int rh_wifi__mode_sta  ( const char *name, const char *password){
    if( ESP_OK!=esp_netif_init())                return 1;
    esp_netif_create_default_wifi_sta();
    // esp_event_handler_instance_t instance_any_id;
    // esp_event_handler_instance_t instance_got_ip;
    // if( ESP_OK!=esp_event_handler_instance_register( WIFI_EVENT, ESP_EVENT_ANY_ID   , &wifi_event_handler, NULL, &instance_any_id)) return 1;
    // if( ESP_OK!=esp_event_handler_instance_register( IP_EVENT  , IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL, &instance_got_ip)) return 1;
    
    if( ESP_OK!=esp_wifi_set_mode(WIFI_MODE_STA)      )   {RH_CONSOLE("wifi sta mode failed");return 1;}
    if( !name || strlen(name)==0 || strlen(name)>=32 )   return 1;

    wifi_config_t wifi_config = {
        .sta = {
            .ssid     = {0},
            .password = {0},
            /* Setting a password implies station will connect to all security modes including WEP/WPA.
             * However these modes are deprecated and not advisable to be used. Incase your Access point
             * doesn't support WPA2, these mode can be enabled by commenting below line */
	        .threshold.authmode = WIFI_AUTH_WPA2_PSK,
        },
    };
    if( password!=NULL && strlen(password)!=0 ) {
        strcpy( (char*)wifi_config.sta.password, password);
        RH_CONSOLE("Wifi: name=%s", name);
        RH_CONSOLE("Wifi: password=%s", password);
    }else{
        RH_CONSOLE("No password");
    }

    if( ESP_OK!=esp_wifi_set_config(WIFI_IF_STA, &wifi_config)) return 1;

    return 0;
}

int rh_wifi__start      (void){
    return esp_wifi_start()!=ESP_OK;
}

int rh_wifi__stop       (void){
    return esp_wifi_stop()!=ESP_OK;
}

int rh_wifi__connect    (void){
    wifi_mode_t mode = WIFI_MODE_NULL;
    esp_wifi_get_mode(&mode);
    if( mode!=WIFI_MODE_STA && mode!=WIFI_MODE_APSTA )
        return 1;
    if( mode==WIFI_MODE_NULL )
        return -1;
    
    return esp_wifi_connect()!=ESP_OK;
}

int rh_wifi__send       (void* buf, size_t len){

    wifi_interface_t ifx = WIFI_IF_AP | WIFI_IF_STA;
    wifi_mode_t mode;
    esp_wifi_get_mode(&mode);
    
    switch( mode ){
        case WIFI_MODE_AP:       ifx = WIFI_IF_AP;  break;
        case WIFI_MODE_STA:      ifx = WIFI_IF_STA; break;    
        case WIFI_MODE_APSTA:    ifx = WIFI_IF_STA|WIFI_IF_AP; break;
        case WIFI_MODE_NULL:
        default:                 return 1;
    }
    
    esp_wifi_set_protocol( ifx, WIFI_PROTOCOL_11B|WIFI_PROTOCOL_11G|WIFI_PROTOCOL_11N);
    return ESP_OK!=esp_wifi_80211_tx( ifx, buf, len, false);
}

typedef struct{
    void*             ev_handler_arg;
    esp_event_base_t  ev_base; 
    int32_t           ev_id;
    void*             ev_data 
}callback_handler_t;

void rh_wifi__handler_0  (void* args){    // Wi-Fi准备就绪
    RH_CONSOLE("Handler 0");
    wifi_mode_t mode = WIFI_MODE_NULL;
    esp_wifi_get_mode(&mode);

    if( mode==WIFI_MODE_AP )
        return;
    RH_CONSOLE("Scan all the wifi access points...");
    wifi_config_t cfg={0};
    esp_wifi_get_config( WIFI_IF_STA, &cfg);
    esp_wifi_scan_start( &cfg, false);
}
void rh_wifi__handler_1  (void* args){    // 扫描AP完成
    RH_CONSOLE("Handler 1");
    esp_wifi_scan_stop();

    uint16_t num=0;
    if( ESP_OK==esp_wifi_scan_get_ap_num(&num) ){
        RH_CONSOLE("%d access points has been detected", num);
    }
    wifi_ap_record_t *list = (wifi_ap_record_t*)malloc(num*sizeof(wifi_ap_record_t));
    esp_wifi_scan_get_ap_records( &num, list);

    for( size_t i=0; i<num; ++i ){
        RH_CONSOLE("%s", list[i].ssid);
    }
    free(list);
}
void rh_wifi__handler_2  (void* args){    // 作为STA开始工作
    RH_CONSOLE("Handler 2");
    RH_CONSOLE("Try to connect to the AP...");
    if( ESP_OK!=esp_wifi_connect() )
        RH_CONSOLE("Connect failed");
}
void rh_wifi__handler_3  (void* args){    // 作为STA结束工作
    RH_CONSOLE("Handler 3");
    RH_CONSOLE("Connect to the AP failed");
}
void rh_wifi__handler_4  (void* args){    // 作为STA连接上AP
    RH_CONSOLE("Handler 4");
}
void rh_wifi__handler_5  (void* args){    // 作为STA断开AP
    RH_CONSOLE("Handler 5");
}
void rh_wifi__handler_6  (void* args){    // 
    RH_CONSOLE("Handler x");
}
void rh_wifi__handler_7  (void* args){    // 作为STA从AP获取了IP
    RH_CONSOLE("Handler 7");
}
void rh_wifi__handler_8  (void* args){    // 作为STA丢到了IP(错误)
    RH_CONSOLE("Handler 8");
}
void rh_wifi__handler_9  (void* args){
    RH_CONSOLE("Handler 9");
}
void rh_wifi__handler_10 (void* args){
    RH_CONSOLE("Handler 10");
}
void rh_wifi__handler_11 (void* args){
    RH_CONSOLE("Handler 11");
}
void rh_wifi__handler_12 (void* args){    // 作为AP开始工作
    RH_CONSOLE("Handler 12");
}
void rh_wifi__handler_13 (void* args){    // 作为AP结束工作  
    RH_CONSOLE("Handler 13");
}
void rh_wifi__handler_14 (void* args){    // 作为AP 发现有STA连接上自己
    RH_CONSOLE("Handler 14");
    callback_handler_t *args_ = (callback_handler_t*)args;
    wifi_event_ap_staconnected_t* event = (wifi_event_ap_staconnected_t*)args_->ev_data;
    RH_CONSOLE("station %02x:%02x:%02x:%02x:%02x:%02x join, AID=%d", MAC2STR(event->mac), event->aid);
}
void rh_wifi__handler_15 (void* args){    // 作为AP 发现有STA断开连接
    RH_CONSOLE("Handler 15");
    callback_handler_t *args_ = (callback_handler_t*)args;
    wifi_event_ap_stadisconnected_t* event = (wifi_event_ap_stadisconnected_t*)args_->ev_data;
    RH_CONSOLE("station %02x:%02x:%02x:%02x:%02x:%02x leave, AID=%d", MAC2STR(event->mac), event->aid);
}
void rh_wifi__handler_16 (void* args){    
    RH_CONSOLE("Handler x");
}
void rh_wifi__handler_17 (void* args){
    RH_CONSOLE("Handler x");
}


