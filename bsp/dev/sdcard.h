#include <stddef.h>

#ifndef RH_SDCARD_H
#define RH_SDCARD_H


int rh_sdcard__touch( const char *__f);
int rh_sdcard__echo ( const char *__f, const char *__mode, const char *__data, size_t __len);
int rh_sdcard__open ( const char *__f, char *__buf, size_t __n);//
int rh_sdcard__mv   ( const char *__f1, const char *__f2);//
int rh_sdcard__cd   ( const char *__p);//
int rh_sdcard__pwd  (       char *__p, size_t __n);//
int rh_sdcard__mkdir( const char *__p);//

#endif





