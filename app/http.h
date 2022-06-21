


#ifndef RH_HTTP_H
#define RH_HTTP_H

#define  RH_HTTP_METHOD_DELETE                      0
#define  RH_HTTP_METHOD_GET                         1
#define  RH_HTTP_METHOD_HEAD                        2
#define  RH_HTTP_METHOD_POST                        3
#define  RH_HTTP_METHOD_PUT                         4
#define  RH_HTTP_METHOD_CONNECT                     5
#define  RH_HTTP_METHOD_OPTIONS                     6
#define  RH_HTTP_METHOD_TRACE                       7
#define  RH_HTTP_METHOD_COPY                        8
#define  RH_HTTP_METHOD_LOCK                        9
#define  RH_HTTP_METHOD_MKCOL                      10
#define  RH_HTTP_METHOD_MOVE                       11
#define  RH_HTTP_METHOD_PROPFIND                   12
#define  RH_HTTP_METHOD_PROPPATCH                  13
#define  RH_HTTP_METHOD_SEARCH                     14
#define  RH_HTTP_METHOD_UNLOCK                     15
#define  RH_HTTP_METHOD_BIND                       16
#define  RH_HTTP_METHOD_REBIND                     17
#define  RH_HTTP_METHOD_UNBIND                     18
#define  RH_HTTP_METHOD_ACL                        19
#define  RH_HTTP_METHOD_REPORT                     20
#define  RH_HTTP_METHOD_MKACTIVITY                 21
#define  RH_HTTP_METHOD_CHECKOUT                   22
#define  RH_HTTP_METHOD_MERGE                      23
#define  RH_HTTP_METHOD_MSEARCH                    24
#define  RH_HTTP_METHOD_NOTIFY                     25
#define  RH_HTTP_METHOD_SUBSCRIBE                  26
#define  RH_HTTP_METHOD_UNSUBSCRIBE                27
#define  RH_HTTP_METHOD_PATCH                      28
#define  RH_HTTP_METHOD_PURGE                      29
#define  RH_HTTP_METHOD_MKCALENDAR                 30
#define  RH_HTTP_METHOD_LINK                       31
#define  RH_HTTP_METHOD_UNLINK                     32
#define  RH_HTTP_METHOD_SOURCE                     33

int rh_http__init        ( void );
int rh_http__server_on   ( const char* url, int RH_HTTP_METHOD_xxxx, int(*handler)(void*), void* param);

int rh_http__handler_0  (void* param);
int rh_http__handler_1  (void* param);
int rh_http__handler_2  (void* param);
int rh_http__handler_3  (void* param);
int rh_http__handler_4  (void* param);
int rh_http__handler_5  (void* param); 

#endif


