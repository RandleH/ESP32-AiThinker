#ifndef RH_COMMON_H
#define RH_COMMON_H

#ifdef __cplusplus
extern "C"{
#endif


#include "esp_log.h"
#define RH_CONSOLE( format, ...)    ESP_LOGE("RH", format, ##__VA_ARGS__)

#define RH_CPU0    (0)
#define RH_CPU1    (1)

#define BOARD_LED_GPIO       (33U)
#define FLUSH_LED_GPIO       (4U)

#define BOARD_LED            (0)
#define FLUSH_LED            (1)

#define SDCARD_D0_GPIO       (2)
#define SDCARD_D1_GPIO       (4)
#define SDCARD_D2_GPIO       (12)
#define SDCARD_D3_GPIO       (13)
#define SDCARD_CMD_GPIO      (15)
#define SDCARD_CLK_GPIO      (14) 

#define SDCARD_MOUNT_POINT       "/sdcard"
#define SDCARD_CONFIG_JSON_PATH  "/config.json"

// See "ESP32_Manual_Reference - IO_MUX_Pad_List" @Page 57 
#define CAMERA_Y2_GPIO       ( 5)
#define CAMERA_Y3_GPIO       (18)
#define CAMERA_Y4_GPIO       (19)
#define CAMERA_Y5_GPIO       (21)
#define CAMERA_Y6_GPIO       (36)    // SENSOR_VP
#define CAMERA_Y7_GPIO       (39)    // SENSOR_VN
#define CAMERA_Y8_GPIO       (34)    // VDET_1
#define CAMERA_Y9_GPIO       (35)    // VDET_2
#define CAMERA_PCLK_GPIO     (22)
#define CAMERA_XCLK_GPIO     ( 0)
#define CAMERA_HSYC_GPIO     (23)
#define CAMERA_VSYC_GPIO     (25)
#define CAMERA_RSET_GPIO     (-1)
#define CAMERA_PWDN_GPIO     (32)
#define CAMERA_HREF_GPIO     (23)
#define CAMERA_SIOD_GPIO     (26)
#define CAMERA_SIOC_GPIO     (27)

#ifdef __cplusplus
}
#endif

#include <string>
#include <vector>
#include <tuple>
#include <queue>
using namespace std;

namespace rh{

class Allocator{
public:
    Allocator( void*(*__malloc)(size_t), void(*__free)(void*) ):malloc(__malloc),free(__free){}
    void*(*malloc)(size_t);
    void(*free)(void*);
};    

class ConfigGPIO{
public:
    ConfigGPIO():isValid(false){}
    bool     isValid;
    vector< std::tuple<int,uint8_t,bool> > list;
};

class ConfigWifi{
public:
    ConfigWifi():isValid(false){}
    bool     isValid;
    vector< std::pair<string,string> > list;
    std::pair<string,string>           ap;
};

class ConfigCamera{
public:
    ConfigCamera():isValid(false),params(nullptr),alloc(malloc,free){}
    bool     isValid;
    void*    params;
    Allocator alloc;
};

class ConfigFirebase{
public:
    ConfigFirebase():isValid(false),host(nullptr),auth(nullptr),alloc(malloc,free){}
    bool      isValid;
    const char*    host;
    const char*    auth;
    const char*    api_key;
    const char*    email;
    const char*    password;
    const char*    bucket_id;
    Allocator alloc;
};

class ConfigSDCard{
public: 
    ConfigSDCard():isValid(false){}
    ConfigSDCard(int mode):isValid(true), d0_gpio(SDCARD_D0_GPIO),\
                                          d1_gpio(SDCARD_D1_GPIO),\
                                          d2_gpio(SDCARD_D2_GPIO),\
                                          d3_gpio(SDCARD_D2_GPIO),\
                                          cmd_gpio(SDCARD_CMD_GPIO),\
                                          clk_gpio(SDCARD_CLK_GPIO),\
                                          mode(mode){}
    bool     isValid;
    int      mode;
    uint8_t  d0_gpio;
    uint8_t  d1_gpio;
    uint8_t  d2_gpio;
    uint8_t  d3_gpio;
    uint8_t  cmd_gpio;
    uint8_t  clk_gpio;    
};


class Event{
public:
    void*   handler;
    Event():handler(NULL){}
    int init(void);
};


class ConfigTask{
public:
    ConfigTask(){} 
    unsigned int   priority;
    const char*    name;
    size_t         stack;
    void*          params;
    int            cpu;
    void           (*callback)(void*);       
};

class Task{
private:
public:
    vector< pair< void*, ConfigTask> > list;
    Task(){}
    int init();
};


class WiFi{
public:
    int connect(const char* ssid, const char* password);
    int connect(const vector<pair<string,string>>& list); 
    int wlan   (const pair<string,string>& ap);
    int wlan   (const char* ssid, const char* password);
    string     ssid;
public:
    WiFi():config(),sta_connected(false),ap_connected(false){}
    ConfigWifi config;
    Event      event;
    Task       task;
    bool       ap_connected;
    bool       sta_connected;
    int init(void);
    int begin(void);
    int register_event(void);
    int register_task(void);
    //...//
};

class Server{
public:
    Server(){}

    int init(void);
    int begin(void);
    int register_event(void);
    int register_task(void);
};

class Database{
public:
    Database():config(){}
    ConfigFirebase config;
    int init(void);
    int set( const char* path, int    value );
    int set( const char* path, string value );//
    int set( const char* path, bool   value );//
};

class Camera{
public:
    Camera():config(){}
    ConfigCamera config;
    int init(void);
    int save(const char* path, const char* name);
    int save(void** buf, size_t *len);
};

class GPIO{
public:
    GPIO():config(){}
    ConfigGPIO  config;
    Event       event;
    Task        task;
    int init(void);
    int begin(void);
    int register_event(void);
    int register_task(void);
};

class SDCard{
public:
    SDCard():config(),isInitialized(false){}
    ConfigSDCard config;
    bool         isInitialized;
};



class Application{
public:
    Application(string tc):time_compile(tc){}
    SDCard    sdcard;
    GPIO      gpio;
    Camera    camera;
    Database  database;
    WiFi      wifi;

    Server    server;

    string    time_compile;
    string    time_real;

    void      print(int(*cb)(char const* format, ...));

};

extern Application app;

}

#include "bsp/rh_gpio.h"
#include "bsp/rh_sdio.h"
#include "bsp/rh_wifi.h"

#include "dev/rh_led.h"
#include "dev/rh_camera.h"
#include "app/rh_rtdb.h"
#include "app/rh_app.h"


#endif
