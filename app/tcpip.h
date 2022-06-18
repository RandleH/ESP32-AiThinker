#ifndef RH_TCPIP_H
#define RH_TCPIP_H




#define RH_TCPIP_EVENTID__STA_GOT_IP              0 
#define RH_TCPIP_EVENTID__STA_LOST_IP             1 
#define RH_TCPIP_EVENTID__AP_STAIPASSIGNED        2 
#define RH_TCPIP_EVENTID__GOT_IP6                 3 
#define RH_TCPIP_EVENTID__ETH_GOT_IP              4 
#define RH_TCPIP_EVENTID__ETH_LOST_IP             5 
#define RH_TCPIP_EVENTID__PPP_GOT_IP              6 
#define RH_TCPIP_EVENTID__PPP_LOST_IP             7 


void rh_tcpip__handler_0(void* args);
void rh_tcpip__handler_1(void* args);
void rh_tcpip__handler_2(void* args);
void rh_tcpip__handler_3(void* args);
void rh_tcpip__handler_4(void* args);
void rh_tcpip__handler_5(void* args);
void rh_tcpip__handler_6(void* args);
void rh_tcpip__handler_7(void* args);

#endif

