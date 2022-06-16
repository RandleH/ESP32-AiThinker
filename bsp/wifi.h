#include <stddef.h>
#ifndef RH_WIFI_H
#define RH_WIFI_H




int  rh_wifi__init( const char *name, const char *password, int channel, size_t maximum);

int  rh_wifi__mode_ap   ( void );
int  rh_wifi__mode_sta  ( void );//
int  rh_wifi__mode_apsta( void );//


int rh_wifi__start      (void);
int rh_wifi__stop       (void);
int rh_wifi__connect    (void);


#endif


