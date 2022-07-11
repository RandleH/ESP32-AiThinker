#include <Firebase_ESP_Client.h>
#include <FB_Const.h>
#include <Firebase.h>
#include <FB_Utils.h>
#include <FB_Net.h>
#include <FB_Error.h>

#include <FirebaseFS.h>



static FirebaseData   fbdo;
static FirebaseConfig fbcfg;
static FirebaseAuth   fbau;

int rh_rtdb__init(const char *host, const char* authkey ){
    fbcfg.database_url = host;
    fbcfg.api_key      = authkey;

    fbau.user.email    = "randle.helmslay@gmail.com";
    fbau.user.password = "123123";


    Firebase.begin( &fbcfg, &fbau);
    Firebase.reconnectWiFi(true);
    
    size_t retry = 0;
    while( !Firebase.ready() && retry<1000 ){
        ++retry;
        vTaskDelay(10);
    }
    
    return (!Firebase.ready());
}

int rh_rtdb__upload( const char* __pf ){
    Firebase.Storage.upload( &fbdo, "esp32-wifi-monitor.appspot.com", "/sdcard/img001.jpg", mem_storage_type_sd, "/img001.jpg", "image/jpeg");
}


int rh_rtdb__deinit(void){
    // Firebase.end(fbdo);
	return 0;
}


