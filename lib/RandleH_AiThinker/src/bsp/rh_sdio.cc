#include <stdint.h>
#include <SD_MMC.h>
#include <FS.h>
#include "rh_common.h"



int rh_sdio__init  (void){
    if( !SD_MMC.begin( SDCARD_MOUNT_POINT ) ){
        return -1;
    }
    rh::app.sdcard.isInitialized = true;    
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

int rh_sdio__touch( const char* __pf){
    if( !__pf )             return 1;
    if( *__pf != '/')       return 1;
    {
        size_t len = strlen(__pf);
        if( len>0xff || len<2 )
            return 2;
    }
    
    char str[256] = {0}; strcpy( str, __pf);
    
    char* p = str;
    while(true){
        p = strchr( p+1, '/');
        if( !p ){
            File fs = SD_MMC.open( str, FILE_WRITE);
            if(!fs) return -1;
            fs.close();
            break;
        }else{
            *p = '\0';
            if(false==SD_MMC.mkdir(str)) return -1;
            *p = '/';
        }
    }
    return 0;
}

int rh_sdio__echo( const char* __pf, const void* __data, size_t __n, const char* opt){
    if( !__data|| !__pf || __n==0 ) return 1;
    
    uint8_t sum=0;
    {
        const char* p = opt;
        while(*p!='\0'){
            sum += *p;
            ++p;
        }
    }

    switch(sum){
        case 'w':
        case 'w'+'b':
            rh_sdio__touch(__pf);
            break;

        case 'r':
        case 'r'+'b':
            return 1;
        
        case 'a':
        case 'a'+'b':
            break;
        default:
            return 1;
    }

    
    File f = SD_MMC.open( __pf, FILE_APPEND);
    if(!f) return -1;
    
    if( !f.write( (uint8_t*)(__data), __n) )
        return -1;
    
    return 0;
}

int rh_sdio__query (const char* __pf, size_t * __len){
    if( !__pf || !__len ) return 1;
    File fp = SD_MMC.open( __pf );
    if( !fp ) return -1;

    *__len = fp.size();
    fp.close();
    return 0;
}

int rh_sdio__load  ( const char* __pf, char* __buf, size_t __len){
    if( !__pf || !__len || !__buf ) return 1;
    File fp = SD_MMC.open( __pf );
    if( !fp ) return -1;
    
    char *__ptr = __buf;
    while(__len){
        size_t toRead = __len;
        if( toRead>512 )
            toRead = 512;
        fp.read( (uint8_t*)__ptr, toRead);
        __ptr += toRead;
        __len -= toRead;    
    }
    fp.close();
    return 0;
}

int  rh_sdio__deinit(void){
    SD_MMC.end();
    rh::app.sdcard.isInitialized = false; 
    return 0;
}


void rh_sdio__test(void){
    0==rh_sdio__touch("/sdcard/msg1.txt")?RH_CONSOLE("touch success"):RH_CONSOLE("touch failed");
    0==rh_sdio__touch("/sdcard/msg2.txt")?RH_CONSOLE("touch success"):RH_CONSOLE("touch failed");
    0==rh_sdio__touch("/sdcard/msg3.txt")?RH_CONSOLE("touch success"):RH_CONSOLE("touch failed");
    0==rh_sdio__touch("/sdcard/msg4.txt")?RH_CONSOLE("touch success"):RH_CONSOLE("touch failed");
    0==rh_sdio__touch("/sdcard/msg5.txt")?RH_CONSOLE("touch success"):RH_CONSOLE("touch failed");

    //存在的文件采用
    0==rh_sdio__echo("/sdcard/msg1.txt", "msg1", 4, "w")?RH_CONSOLE("echo success"):RH_CONSOLE("echo failed");
    0==rh_sdio__echo("/sdcard/msg1.txt", "appe", 4, "a")?RH_CONSOLE("echo success"):RH_CONSOLE("echo failed");
    0==rh_sdio__echo("/sdcard/msg2.txt", "msg1", 4, "w")?RH_CONSOLE("echo success"):RH_CONSOLE("echo failed");
    0==rh_sdio__echo("/sdcard/msg2.txt", "appe", 4, "w")?RH_CONSOLE("echo success"):RH_CONSOLE("echo failed");
    
    // 不存在的文件采用"w"模式
    0==rh_sdio__echo("/sdcard/msg6.txt", "msg1", 4, "w")?RH_CONSOLE("echo success"):RH_CONSOLE("echo failed");
    0==rh_sdio__echo("/sdcard/msg6.txt", "appe", 4, "a")?RH_CONSOLE("echo success"):RH_CONSOLE("echo failed");
}