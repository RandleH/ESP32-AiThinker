#include <Firebase_ESP_Client.h>
// #include <addons/RTDBHelper.h>

#include "rh_common.h"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;


int rh_rtdb__init(const char *host, const char* authkey ){
    config.database_url               = host;
    config.signer.tokens.legacy_token = authkey;

    Firebase.reconnectWiFi(true);
    Firebase.begin(&config, &auth);

    // size_t retry = 1000;
    // while( --retry && !Firebase.ready() );
    
    return Firebase.ready()!=true;
}

int rh_rtdb__upload( const char* __pf ){
    Firebase.Storage.upload( &fbdo, "esp32-wifi-monitor.appspot.com", "/sdcard/img001.jpg", mem_storage_type_sd, "/img001.jpg", "image/jpeg");
}


int rh_rtdb__deinit(void){
    // Firebase.end(fbdo);
	return 0;
}

namespace rh{

int Database::init(void){
    if( app.database.config.isValid ){
        RH_CONSOLE( "host:%s", app.database.config.host );
        RH_CONSOLE( "auth:%s", app.database.config.auth );
        return rh_rtdb__init( app.database.config.host, app.database.config.auth );
    }
    return rh_rtdb__init( "esp32-wifi-monitor-default-rtdb.firebaseio.com", \
                          "JEScAl2BJFIo5goTturXfL76y0eaCJiXBbx1wRgr");
}

int Database::set( const char* path, int value ){
    RH_CONSOLE("%s", __func__);
    if( true!=Firebase.RTDB.setInt( &fbdo, path, value) ){
        RH_CONSOLE( "%s", fbdo.errorReason().c_str() );
        return -1;
    }
    RH_CONSOLE("ok");
    return 0;
}

}


