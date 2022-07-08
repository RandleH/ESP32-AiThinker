
#include <SD_MMC.h>
#include <FS.h>
#include "rh_common.h"



int rh_sdio__init  (void){
    if( !SD_MMC.begin( SDCARD_MOUNT_POINT ) ){
        return -1;
    }    
    return 0;
}

int rh_sdio__mkdir( const char* __p ){
    if( !__p )             return 1;
    if( *__p != '/')       return 1;
    {
        size_t len = strlen(__p);
        if( len>0xff || len<2 )
            return 2;
    }
    
    char str[256] = {0}; strcpy( str, __p);

    char* p = str;
    while(true){
        p = strchr( p+1, '/');
        if( !p ){
            if(false==SD_MMC.mkdir(str)) return -1;
            break;
        }else{
            *p = '\0';
            if(false==SD_MMC.mkdir(str)) return -1;
            *p = '/';
        }
    }

    return 0;
}




int  rh_sdio__deinit(void){
    SD_MMC.end();
    return 0;
}


void rh_sdio__test(void){
    
}