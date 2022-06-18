

#include "../bsp/wifi.h"

#ifndef RH_APP_EVENT_H
#define RH_APP_EVENT_H


#define RH_APP_EVENT_SYSTEM      (0)
#define RH_APP_EVENT_BLE         (6)
#define RH_APP_EVENT_WIFI        (7)
#define RH_APP_EVENT_TCPIP       (8)

int  rh_event__init  (void);
void rh_event__insert( int RH_APP_EVENT_xxxx, int id, void(*func)(void*) );
void rh_event__delete( int event);


#endif






