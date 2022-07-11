
#include <string.h>
#include "./rh_app.h"
#include "bsp/rh_sdio.h"
#include "jsmn.h"

static int rh_s__check_fromJSON(  char* __buf, size_t __len){
    for( size_t i=0; i<__len; ++i){
        if( __buf[i]==(char)0xff || __buf[i]=='\t' )    __buf[i] = ' ';
        else if( __buf[i]=='\0' ) return 1;
    }
    return 0;
}

static int rh_s__jsoneq(const char *json, jsmntok_t *tok, const char *s) {
    if (tok->type == JSMN_STRING && (int)strlen(s) == tok->end - tok->start &&
        strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
      return 0;
    }
    return -1;
}

static int rh_s__write_config_camera( const char *js, const jsmntok_t *tok){
    size_t offset=0;
    assert( tok->type==JSMN_STRING );
    ++tok;++offset;
    assert( tok->type==JSMN_OBJECT );
    size_t nitem = tok->size;
    ++tok;++offset;
    for( size_t i=0; i<nitem; ++i, ++tok, ++offset ){
        assert( tok->type==JSMN_STRING );
        RH_CONSOLE("type=%d, %.*s -- ", tok->type, tok->end-tok->start, &js[tok->start]);  // KEY - "XXX_GPIO"
        ++tok;++offset;
        
        assert( tok->type==JSMN_PRIMITIVE );
        RH_CONSOLE("type=%d, %.*s\n", tok->type, tok->end-tok->start, &js[tok->start]);    // VALUE
    }
    return offset;
}

static size_t rh_s__write_config_wifi( const char *js, const jsmntok_t *tok){
    size_t offset=0;
    assert( tok->type==JSMN_STRING );
    ++tok;++offset;
    assert( tok->type==JSMN_ARRAY );
    size_t nitem = tok->size;
    ++tok;++offset;
    for( size_t i=0; i<nitem; ++i ){
        assert( tok->type==JSMN_OBJECT );
        ++tok;++offset;
        
        assert( tok->type==JSMN_STRING );
        RH_CONSOLE("type=%d, %.*s -- ", tok->type, tok->end-tok->start, &js[tok->start]);  // KEY - "ssid"
        ++tok;++offset;
        
        assert( tok->type==JSMN_STRING );
        RH_CONSOLE("type=%d, %.*s\n", tok->type, tok->end-tok->start, &js[tok->start]);    // VALUE
        ++tok; ++offset;
        
        assert( tok->type==JSMN_STRING );
        RH_CONSOLE("type=%d, %.*s -- ", tok->type, tok->end-tok->start, &js[tok->start]);  // KEY - "password"
        ++tok;++offset;
        
        assert( tok->type==JSMN_STRING || tok->type==JSMN_PRIMITIVE );
        RH_CONSOLE("type=%d, %.*s\n", tok->type, tok->end-tok->start, &js[tok->start]);    // VALUE if PRIMITIVE, means no password
        ++tok; ++offset;
    }
    return offset;
}

static size_t rh_s__write_config_firebase( const char *js, const jsmntok_t *tok){
    size_t offset=0;
    assert( tok->type==JSMN_STRING );
    ++tok;++offset;
    assert( tok->type==JSMN_OBJECT );
    size_t nitem = tok->size;
    ++tok;++offset;
    for( size_t i=0; i<nitem; ++i, ++tok, ++offset ){
        assert( tok->type==JSMN_STRING );
        RH_CONSOLE("type=%d, %.*s -- ", tok->type, tok->end-tok->start, &js[tok->start]);  // KEY - "host"
        ++tok;++offset;
        
        assert( tok->type==JSMN_STRING );
        RH_CONSOLE("type=%d, %.*s\n", tok->type, tok->end-tok->start, &js[tok->start]);    // VALUE
    }
    return offset;
}

static int rh_s__pharse( char* __js, size_t __len){
    jsmn_parser parser;
    jsmn_init(&parser);
    int r = jsmn_parse( &parser, __js, __len, NULL, 0);           // 试探性计算token数量
    
    if(r<0){
        return -1;
    }
    
    jsmntok_t *tok = (jsmntok_t*)malloc(r*sizeof(jsmntok_t));       // 分配临时空间
    jsmn_init(&parser);                                             // 初始化(重定向)
    r = jsmn_parse(&parser, __js, __len, tok, r);                   // 获取token

    for ( int i=1; i<r; ) {
        // printf("size=%d, type=%d, %.*s\n", t[i+1].size, t[i+1].type, t[i+1].end-t[i+1].start, &js[t[i+1].start]);
        if(       0==rh_s__jsoneq( __js, &tok[i], "camera") ){
            i += rh_s__write_config_camera  ( __js, &tok[i]);
            // Do something
        }else if( 0==rh_s__jsoneq( __js, &tok[i], "wifi")   ){
            i += rh_s__write_config_wifi    ( __js, &tok[i]);
            // Do something
        }else if( 0==rh_s__jsoneq( __js, &tok[i], "firebase") ){
            i += rh_s__write_config_firebase( __js, &tok[i]);
            // Do something
        }else{
            // Unknown Key Pair
            ++i;
        }
        
    }    
    free(tok);
    return 0;
}

void rh_app__init_fromJSON ( rh::Application& app){
    RH_CONSOLE("%s:", __func__);
    size_t len = 0;
    char*  buf = NULL;

    if( !app.sdcard.isInitialized ){
        RH_CONSOLE("SDCard has't been initialized");    
        RH_CONSOLE("Trying to initialize...");
        if( 0!=rh_sdio__init() ){
            RH_CONSOLE("Failed to initialize SD card");
            goto DEFAULT_INIT;
        }
    }
    
    if(0!=rh_sdio__query( SDCARD_CONFIG_JSON_PATH, &len)){
        RH_CONSOLE("Failed to query %s", SDCARD_CONFIG_JSON_PATH);
        goto DEFAULT_INIT;
    }

    buf = (char*)calloc(len+1, sizeof(char));
    if(NULL==buf){
        RH_CONSOLE("Failed to malloc space for JSON");
        goto DEFAULT_INIT;
    }

    buf[len] = '\0';
    if(0!=rh_sdio__load( SDCARD_CONFIG_JSON_PATH, buf, len)){
        RH_CONSOLE("Failed to load %s");
        goto DEFAULT_INIT;
    }

    if(0!=rh_s__check_fromJSON( buf, len)){
        RH_CONSOLE("Invalid JSON charactor detected");
        goto DEFAULT_INIT;
    }

    RH_CONSOLE("Successfully load JSON file");
    RH_CONSOLE("Pharsing JSON...");
    if(0!=rh_s__pharse( buf, len)){
        RH_CONSOLE("Failed to pharse JSON");
        goto DEFAULT_INIT;
    }
    // RH_CONSOLE("JSON:\n\n\n%s", buf);



    return;

DEFAULT_INIT:
    RH_CONSOLE("Switch to initialize in default mode");
    if( NULL!=buf ) 
        free(buf);
    rh_app__init_default(app);    
}

void rh_app__init_default  ( rh::Application& app){
    RH_CONSOLE("%s:", __func__);
}

