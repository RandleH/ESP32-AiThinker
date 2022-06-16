

#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#include "esp_wifi.h"
#include "esp_event.h"
#include  "nvs_flash.h"

struct{
    const char *ssid;
    const char *password;
    size_t      maximum;
    int         channel;
    bool        isValid;
}cache = {.isValid=false};

static void wifi_event_handler(void* arg, esp_event_base_t event_base,
                                    int32_t event_id, void* event_data){
    
}



int  rh_wifi__init( const char *name, const char *password, int channel, size_t maximum){
    if(!name)             return -1;
    if(strlen(name)>=32)  return 1;  // Name too long

    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }

    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_ap();
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

    ret = esp_wifi_init(&wifi_init_cfg);

    
    cache.isValid = true;
    cache.channel = channel;
    cache.ssid    = strcpy( calloc(1, strlen(name)+1)    , name );
    if( !password )
        cache.password = strcpy( calloc(1, strlen(password)+1), password );
    else
        cache.password = NULL;
    cache.maximum = maximum;


    esp_event_handler_instance_register(WIFI_EVENT,
                                        ESP_EVENT_ANY_ID,
                                        &wifi_event_handler,
                                        NULL,
                                        NULL);

    return ret!=ESP_OK;
}


int  rh_wifi__mode_ap   (void){
    if( !cache.isValid ) return -1;
    esp_err_t ret = esp_wifi_set_mode( WIFI_MODE_AP );
    if( ret!=ESP_OK ){
        // while(1);    FAILED
        return 1;
    } 

    wifi_config_t wifi_config = {
        .ap = {
            .ssid_len       = strlen(cache.ssid),
            .channel        = cache.channel,
            .max_connection = cache.maximum,
            .authmode       = WIFI_AUTH_WPA_WPA2_PSK
        },
    };
    strcpy( (char*)wifi_config.ap.ssid, cache.ssid);
    if( cache.password!=NULL )
        strcpy( (char*)wifi_config.ap.password, cache.password );

    if( cache.password==NULL || strlen(cache.password) == 0) {
        wifi_config.ap.authmode = WIFI_AUTH_OPEN;
    }
    if( ESP_OK!=esp_wifi_set_config(WIFI_IF_AP, &wifi_config)) return 1;
    if( ESP_OK!=esp_wifi_start()) return 1;

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










