#include <stddef.h>
#ifndef RH_WIFI_H
#define RH_WIFI_H




int  rh_wifi__init(void);

int  rh_wifi__mode_ap   ( const char* ssid, const char* password, int channel );
int  rh_wifi__mode_sta  ( const char* ssid, const char* password, int channel );
int  rh_wifi__mode_apsta( void );//

int rh_wifi__start      (void);
int rh_wifi__stop       (void);
int rh_wifi__connect    (void);
int rh_wifi__send       (void* buf, size_t len);


#define RH_WIFI_EVENTID__READY                         0               
#define RH_WIFI_EVENTID__SCAN_DONE                     1           
#define RH_WIFI_EVENTID__STA_START                     2           
#define RH_WIFI_EVENTID__STA_STOP                      3           
#define RH_WIFI_EVENTID__STA_CONNECTED                 4           
#define RH_WIFI_EVENTID__STA_DISCONNECTED              5           
#define RH_WIFI_EVENTID__STA_AUTHMODE_CHANGE           6           
#define RH_WIFI_EVENTID__STA_WPS_ER_SUCCESS            7          
#define RH_WIFI_EVENTID__STA_WPS_ER_FAILED             8           
#define RH_WIFI_EVENTID__STA_WPS_ER_TIMEOUT            9           
#define RH_WIFI_EVENTID__STA_WPS_ER_PIN               10           
#define RH_WIFI_EVENTID__STA_WPS_ER_PBC_OVERLAP       11           
#define RH_WIFI_EVENTID__AP_START                     12           
#define RH_WIFI_EVENTID__AP_STOP                      13          
#define RH_WIFI_EVENTID__AP_STACONNECTED              14          
#define RH_WIFI_EVENTID__AP_STADISCONNECTED           15          
#define RH_WIFI_EVENTID__AP_PROBEREQRECVED            16          
#define RH_WIFI_EVENTID__FTM_REPORT                   17          

void rh_wifi__handler_0  (void* args);
void rh_wifi__handler_1  (void* args);
void rh_wifi__handler_2  (void* args);
void rh_wifi__handler_3  (void* args);
void rh_wifi__handler_4  (void* args);
void rh_wifi__handler_5  (void* args);
void rh_wifi__handler_6  (void* args);
void rh_wifi__handler_7  (void* args);
void rh_wifi__handler_8  (void* args);
void rh_wifi__handler_9  (void* args);
void rh_wifi__handler_10 (void* args);
void rh_wifi__handler_11 (void* args);
void rh_wifi__handler_12 (void* args);
void rh_wifi__handler_13 (void* args);
void rh_wifi__handler_14 (void* args);
void rh_wifi__handler_15 (void* args);
void rh_wifi__handler_16 (void* args);
void rh_wifi__handler_17 (void* args);

#endif


