
#include <string.h>
#include "./rh_app.h"
#include "bsp/rh_sdio.h"
#include "jsmn.h"

typedef std::uint64_t hash_t;
constexpr hash_t prime = 0x100000001B3ull;
constexpr hash_t basis = 0xCBF29CE484222325ull;

static constexpr hash_t MAP(const char *str, hash_t last_value=basis){
    return *str ? MAP(str+1, (*str^last_value)*prime) : last_value;
}

static hash_t GET(const char *str){
    hash_t ret{basis};
    while(*str){
        ret ^= *str;
        ret *= prime;
        ++str;
    }
    return ret;
}

static hash_t GET(const char *str, size_t len){
    hash_t ret{basis};
    while(len--){
        ret ^= *str;
        ret *= prime;
        ++str;
    }
    return ret;
}



#include "esp_camera.h"
static size_t rh_s__write_config_camera   ( const char *js, const jsmntok_t *tok, rh::ConfigCamera&   config){
    size_t offset=0;
    assert( tok->type==JSMN_STRING );
    ++tok;++offset;
    assert( tok->type==JSMN_OBJECT );
    size_t nitem = tok->size;
    ++tok;++offset;

    if( NULL==config.params ){
        config.params = new camera_config_t;
        if( nullptr==config.params ){
            goto WRITE_CONFIG_CAMERA_FAILED;
        }
        memset( config.params, 0, sizeof(camera_config_t));
    }
    for( size_t i=0; i<nitem; ++i, ++tok, ++offset ){
        assert( tok->type==JSMN_STRING );
        // RH_CONSOLE("type=%d, %.*s -- ", tok->type, tok->end-tok->start, &js[tok->start]);  // KEY - "XXX_GPIO"
        hash_t item = GET(&js[tok->start], tok->end-tok->start);
        
        ++tok;++offset;
        assert( tok->type==JSMN_PRIMITIVE );
        // RH_CONSOLE("type=%d, %.*s\n", tok->type, tok->end-tok->start, &js[tok->start]);    // VALUE
        
        // Get the value from JSON and convert string to number
        char buf[32] = {0};
        if( 32<=snprintf( buf, 32, "%.*s", tok->end-tok->start, &js[tok->start]) ){
            RH_CONSOLE("Parameters should be less than 32");
            goto WRITE_CONFIG_CAMERA_FAILED;
        }
        long val = atol( buf );
        switch( item ){
            case MAP("Y2_GPIO"):  
                ((camera_config_t*)(config.params))->pin_d0 = val;break;
            case MAP("Y3_GPIO"):
                ((camera_config_t*)(config.params))->pin_d1 = val;break;
            case MAP("Y4_GPIO"):
                ((camera_config_t*)(config.params))->pin_d2 = val;break;            
            case MAP("Y5_GPIO"):
                ((camera_config_t*)(config.params))->pin_d3 = val;break;
            case MAP("Y6_GPIO"):
                ((camera_config_t*)(config.params))->pin_d4 = val;break;
            case MAP("Y7_GPIO"):
                ((camera_config_t*)(config.params))->pin_d5 = val;break;
            case MAP("Y8_GPIO"):
                ((camera_config_t*)(config.params))->pin_d6 = val;break;
            case MAP("Y9_GPIO"):
                ((camera_config_t*)(config.params))->pin_d7 = val;break;
            case MAP("PCLK_GPIO"):
                ((camera_config_t*)(config.params))->pin_pclk  = val;break;
            case MAP("XCLK_GPIO"):
                ((camera_config_t*)(config.params))->pin_xclk  = val;break;
            case MAP("HSYC_GPIO"):
                break;
            case MAP("VSYC_GPIO"):
                ((camera_config_t*)(config.params))->pin_vsync = val;break;
            case MAP("RSET_GPIO"):
                ((camera_config_t*)(config.params))->pin_reset = val;break;
            case MAP("PWDN_GPIO"):
                ((camera_config_t*)(config.params))->pin_pwdn  = val;break;
            case MAP("HREF_GPIO"):
                ((camera_config_t*)(config.params))->pin_href  = val;break;
            case MAP("SIOD_GPIO"):
                ((camera_config_t*)(config.params))->pin_sscb_sda = val;break;
            case MAP("SIOC_GPIO"):
                ((camera_config_t*)(config.params))->pin_sscb_scl = val;break;
            case MAP("XCLK_FREQ"):
                ((camera_config_t*)(config.params))->xclk_freq_hz = val;break;
            case MAP("PIXFORMAT"):
                ((camera_config_t*)(config.params))->pixel_format = (pixformat_t)val;break;
            case MAP("FRAMESIZE"):
                ((camera_config_t*)(config.params))->frame_size   = (framesize_t)val;break;    
            case MAP("JPEG_QUALITY"):
                ((camera_config_t*)(config.params))->jpeg_quality = val;break;
            case MAP("GRAB_MODE"):
                ((camera_config_t*)(config.params))->grab_mode    = (camera_grab_mode_t)val;break;    
            case MAP("FRAME_CNT"):
                ((camera_config_t*)(config.params))->fb_count     = val;break;
            default:
                RH_CONSOLE("Invalid camera config params detacted");
                goto WRITE_CONFIG_CAMERA_FAILED;
        }
    }
    ((camera_config_t*)(config.params))->ledc_timer   = LEDC_TIMER_0,
    ((camera_config_t*)(config.params))->ledc_channel = LEDC_CHANNEL_0,
    config.isValid = true;

    return offset;

WRITE_CONFIG_CAMERA_FAILED:
    if( nullptr!=config.params )
        delete ((camera_config_t*)config.params);
    config.params  = nullptr;
    config.isValid = false;    
    return -1;
}

static size_t rh_s__write_config_wifi     ( const char *js, const jsmntok_t *tok, rh::ConfigWifi&     config){
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
        // RH_CONSOLE("type=%d, %.*s -- ", tok->type, tok->end-tok->start, &js[tok->start]);  // KEY - "ssid"
        ++tok;++offset;
        
        assert( tok->type==JSMN_STRING );
        // RH_CONSOLE("type=%d, %.*s\n", tok->type, tok->end-tok->start, &js[tok->start]);    // VALUE
        
        string ssid( &js[tok->start], tok->end-tok->start);
        ++tok; ++offset;
        
        assert( tok->type==JSMN_STRING );
        // RH_CONSOLE("type=%d, %.*s -- ", tok->type, tok->end-tok->start, &js[tok->start]);  // KEY - "password"
        ++tok;++offset;
        
        assert( tok->type==JSMN_STRING || tok->type==JSMN_PRIMITIVE );
        // RH_CONSOLE("type=%d, %.*s\n", tok->type, tok->end-tok->start, &js[tok->start]);    // VALUE if PRIMITIVE, means no password
        string password;
        if( tok->type==JSMN_PRIMITIVE )
            password = string();
        if( tok->type==JSMN_STRING)
            password = string( &js[tok->start], tok->end-tok->start);
        ++tok; ++offset;

        config.list.push_back( std::make_pair( ssid, password) );
    }
    config.isValid = true;
    return offset;
}

static size_t rh_s__write_config_firebase ( const char *js, const jsmntok_t *tok, rh::ConfigFirebase& config){
    size_t offset=0;
    assert( tok->type==JSMN_STRING );
    ++tok;++offset;
    assert( tok->type==JSMN_OBJECT );
    size_t nitem = tok->size;
    ++tok;++offset;
    
    size_t nc = 0;

    // JSON = "[host]"
    assert( tok->type==JSMN_STRING );
    ++tok;++offset;

    // JSON = "xxxxx.com"
    assert( tok->type==JSMN_STRING );
    nc = tok->end-tok->start;
    char* host = (char*)config.alloc.malloc( (nc+1)*sizeof(char) );
    strncpy( host, &js[tok->start], nc );
    host[nc] = '\0';
    config.host = host;
    ++tok;++offset;

    // JSON = "[auth]"
    assert( tok->type==JSMN_STRING );
    ++tok;++offset;

    // JSON = "********"
    assert( tok->type==JSMN_STRING );
    nc = tok->end-tok->start;
    char* auth = (char*)config.alloc.malloc( (nc+1)*sizeof(char) );
    strncpy( auth, &js[tok->start], nc );
    auth[nc] = '\0';
    config.auth = auth;
    ++tok;++offset;

    config.isValid = true;
    return offset;
}

static int rh_s__check (       char* js, size_t len){
    for( size_t i=0; i<len; ++i){
        if( js[i]==(char)0xff || js[i]=='\t' )    js[i] = ' ';
        else if( js[i]=='\0' ) return 1;
    }
    return 0;
}

static int rh_s__jsoneq( const char *js, jsmntok_t *tok, const char *s) {
    if (tok->type == JSMN_STRING && (int)strlen(s) == tok->end - tok->start &&
        strncmp(js + tok->start, s, tok->end - tok->start) == 0) {
      return 0;
    }
    return -1;
}

static int rh_s__pharse( const char* js, size_t len, rh::Application& __app){
    jsmn_parser parser;
    jsmn_init(&parser);
    int r = jsmn_parse( &parser, js, len, NULL, 0);           // 试探性计算token数量
    
    if(r<0){
        return -1;
    }
    
    jsmntok_t *tok = (jsmntok_t*)malloc(r*sizeof(jsmntok_t));       // 分配临时空间
    jsmn_init(&parser);                                             // 初始化(重定向)
    r = jsmn_parse(&parser, js, len, tok, r);                   // 获取token

    for ( int i=1; i<r; ) {
        if(       0==rh_s__jsoneq( js, &tok[i], "camera") ){
            i += rh_s__write_config_camera  ( js, &tok[i], __app.camera.config);
            // Do something
        }else if( 0==rh_s__jsoneq( js, &tok[i], "wifi")   ){
            i += rh_s__write_config_wifi    ( js, &tok[i], __app.wifi.config);
            // Do something
        }else if( 0==rh_s__jsoneq( js, &tok[i], "firebase") ){
            i += rh_s__write_config_firebase( js, &tok[i], __app.database.config);
            // Do something
        }else{
            // Unknown Key Pair
            ++i;
        }
        
    }    
    free(tok);
    return 0;
}



void rh_app__load_fromJSON ( rh::Application& app){
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

    if(0!=rh_s__check( buf, len)){
        RH_CONSOLE("Invalid JSON charactor detected");
        goto DEFAULT_INIT;
    }

    RH_CONSOLE("Successfully load JSON file");
    RH_CONSOLE("Pharsing JSON...");
    if(0!=rh_s__pharse( buf, len, app)){
        RH_CONSOLE("Failed to pharse JSON");
        goto DEFAULT_INIT;
    }


    return;

DEFAULT_INIT:
    RH_CONSOLE("Switch to initialize in default mode");
    if( NULL!=buf ) 
        free(buf);
    rh_app__load_default(app);    
}

void rh_app__load_default  ( rh::Application& app){
    RH_CONSOLE("%s:", __func__);
}

