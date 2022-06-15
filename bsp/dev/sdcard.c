
#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/dirent.h>
#include <sys/stat.h>

#include "../schematic.h"

#define PATH_MAX_LEN (64)


static char pwd[PATH_MAX_LEN] = SDCARD_MOUNT_POINT;

int rh_sdcard__touch( const char *__f ){
    FILE *f = fopen( __f, "w");
    if( f==NULL ) return -1;
    
    fclose(f);
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

int rh_sdcard__echo ( const char *__f, const char *__mode, const char *__data, size_t __len){
    FILE *file = fopen( __f, __mode);
    if( file==NULL ) return -1;
    fwrite( __data, 1, __len, file);
    fclose(file);
    return 0;
}










