#include "rh_common.h"
#include "/bsp/rh_sdio.h"


// CASE 1: Read the JSON file from SD Card
void TEST1(void){
    0==rh_sdio__init()? rh_app__init_fromJSON( rh::app ) : rh_app__init_default( rh::app );
    size_t len = 0;
    0==rh_sdio__query( "/config.json", &len)?RH_CONSOLE("query success, len=%ld", len):RH_CONSOLE("query failed");
    
    char *buf = (char*)calloc(len+1, sizeof(char));
    NULL!=buf?RH_CONSOLE("malloc success"):RH_CONSOLE("malloc failed");
    
    buf[len] = '\0';
    0==rh_sdio__load("/config.json", buf, len)?RH_CONSOLE("load success"):RH_CONSOLE("load failed");
    
    RH_CONSOLE("JSON:\n\n\n%s", buf);
}


