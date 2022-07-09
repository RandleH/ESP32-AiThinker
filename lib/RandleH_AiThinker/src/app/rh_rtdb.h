#ifndef RH_RTDB_H
#define RH_RTDB_H


int rh_rtdb__init(const char *host, const char* authkey );
int rh_rtdb__upload( const char* __pf );
int rh_rtdb__deinit(void);


#endif
