// #include <Firebase_ESP_Client.h>
// // #include <addons/RTDBHelper.h>
// #include <addons/TokenHelper.h>
// #include "rh_common.h"

// FirebaseData fbdo;
// FirebaseAuth auth;
// FirebaseConfig config;


// int rh_rtdb__init(const char *host, const char* authkey ){
//     config.database_url               = host;
//     config.signer.tokens.legacy_token = authkey;
//     config.api_key                    = "AIzaSyANzso7XRzHlsUhhQcOy51oMihIm-43-KY"; 
//     config.token_status_callback = tokenStatusCallback;
//     auth.user.email    = "randle.helmslay@gmail.com";
//     auth.user.password = "0629johnny";

//     Firebase.reconnectWiFi(true);
//     Firebase.begin(&config, &auth);

//     // size_t retry = 1000;
//     // while( --retry && !Firebase.ready() );
    
//     return Firebase.ready()!=true;
// }

// int rh_rtdb__upload( const char* __pf ){
//     Firebase.Storage.upload( &fbdo, \
//                              "esp32-wifi-monitor.appspot.com",\
//                              "/sdcard/img001.jpg",\
//                              mem_storage_type_sd,\
//                              "/img001.jpg",\
//                               "image/jpeg");
//     return 0;
// }


// int rh_rtdb__deinit(void){
//     // Firebase.end(fbdo);
// 	return 0;
// }

#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#include "rh_common.h"

FirebaseData    fbdo;
FirebaseAuth    auth;
FirebaseConfig  configF;

namespace rh{

int Database::init(void){

    this->config.bucket_id = "esp32-wifi-monitor.appspot.com";
    if( app.database.config.isValid ){
        auth.user.email    = this->config.email    = "randle.helmslay@gmail.com";
        auth.user.password = this->config.password = "0629johnny";
        configF.token_status_callback = tokenStatusCallback; 
        configF.api_key    = this->config.api_key  = "AIzaSyANzso7XRzHlsUhhQcOy51oMihIm-43-KY";
        
        Firebase.begin(&configF, &auth);
        Firebase.reconnectWiFi(true);
        return 0;
        // return rh_rtdb__init( app.database.config.host, app.database.config.auth );
    }
    configF.database_url               = this->config.host = "esp32-wifi-monitor-default-rtdb.firebaseio.com";
    configF.signer.tokens.legacy_token = this->config.auth = "JEScAl2BJFIo5goTturXfL76y0eaCJiXBbx1wRgr";
    return 0;
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


