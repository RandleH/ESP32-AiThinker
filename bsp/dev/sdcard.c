
#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/dirent.h>
#include <sys/stat.h>

#include "../schematic.h"

#define PATH_MAX_LEN (64)


static char pwd[PATH_MAX_LEN] = SDCARD_MOUNT_POINT;

int rh_sdcard__touch( const char *__f ){
    RH_CONSOLE("%s", __func__);
    FILE *f = fopen( __f, "w");
    if( f==NULL ){
        RH_CONSOLE("Failed to create a file");
        return -1;
    } 
    fclose(f);

    RH_CONSOLE("Success");
    return 0;
}

int rh_sdcard__cd( const char *__p){
    DIR *dirptr = opendir(__p);

    if( dirptr!=NULL ){
        ptrdiff_t offset = strncpy( pwd, __p, PATH_MAX_LEN ) - pwd;

        closedir( dirptr);
        if( offset>PATH_MAX_LEN)
            return 1;
    }else{
        closedir( dirptr);
        return -1;
    }
        
    return 0;
}

int rh_sdcard__echo ( const char *__pf, const char *__mode, const char *__data, size_t __len){
    FILE *file = fopen( __pf, __mode);
    if( file==NULL ) return -1;
    fwrite( __data, 1, __len, file);
    fclose(file);
    return 0;
}

int rh_sdcard__read ( const char *__pf, const char *__mode, char *__buf, size_t __len){
    if( !__buf || __len==0 ) return -1;
    FILE *file = fopen( __pf, __mode);
    if( file==NULL ) return 1;
    fread(__buf, 1, __len, file);
    fclose(file);
    return 0;
}








