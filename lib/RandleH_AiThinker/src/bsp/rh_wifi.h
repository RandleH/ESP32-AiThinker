#include <stddef.h>
#ifndef RH_WIFI_H
#define RH_WIFI_H




int  rh_wifi__init      (void);
int  rh_wifi__connect   ( const char* ssid, const char* password);


#endif