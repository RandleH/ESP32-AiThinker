#include <string.h>
#include <stdlib.h>
#include "esp_heap_caps.h"
#include "rh_common.h"

#include "../support/esp_camera.h"
#include "../support/sensor.h"
#include "../support/cam_hal.h"
#include "bsp/rh_sdio.h"
#include "dev/rh_camera.h"


int rh_camera__init        (void){
    framesize_t frame_size   = FRAMESIZE_UXGA;
    pixformat_t pixel_format = PIXFORMAT_JPEG;
    uint8_t     fb_count     = 2;
    int         xclk_freq_hz = 10000000;

    framesize_t size_bak = frame_size;
    if (PIXFORMAT_JPEG == pixel_format && FRAMESIZE_SVGA > frame_size) {
        frame_size = FRAMESIZE_HD;
    }
    camera_config_t camera_config = {
        .pin_pwdn     = CAMERA_PWDN_GPIO,
        .pin_reset    = CAMERA_RSET_GPIO,
        .pin_xclk     = CAMERA_XCLK_GPIO,
        .pin_sscb_sda = CAMERA_SIOD_GPIO,
        .pin_sscb_scl = CAMERA_SIOC_GPIO,

        .pin_d7    = CAMERA_Y9_GPIO,
        .pin_d6    = CAMERA_Y8_GPIO,
        .pin_d5    = CAMERA_Y7_GPIO,
        .pin_d4    = CAMERA_Y6_GPIO,
        .pin_d3    = CAMERA_Y5_GPIO,
        .pin_d2    = CAMERA_Y4_GPIO,
        .pin_d1    = CAMERA_Y3_GPIO,
        .pin_d0    = CAMERA_Y2_GPIO,
        .pin_vsync = CAMERA_VSYC_GPIO,
        .pin_href  = CAMERA_HREF_GPIO,
        .pin_pclk  = CAMERA_PCLK_GPIO,

        //EXPERIMENTAL: Set to 16MHz on ESP32-S2 or ESP32-S3 to enable EDMA mode
        .xclk_freq_hz = xclk_freq_hz,
        .ledc_timer = LEDC_TIMER_0,
        .ledc_channel = LEDC_CHANNEL_0,

        .pixel_format = pixel_format, //YUV422,GRAYSCALE,RGB565,JPEG
        .frame_size = frame_size,    //QQVGA-UXGA Do not use sizes above QVGA when not JPEG

        .jpeg_quality = 12, //0-63 lower number means higher quality
        .fb_count = fb_count,       //if more than one, i2s runs in continuous mode. Use only with JPEG
        .grab_mode = CAMERA_GRAB_WHEN_EMPTY,
    };

    return rh_camera__init(&camera_config);
}

int rh_camera__init(void* params){
    camera_config_t *config = (camera_config_t*)params;
    //initialize the camera
    esp_err_t ret = esp_camera_init(config);
    
    if( ret!=ESP_OK ) while(1);   // PASSED

    if (ESP_OK == ret && PIXFORMAT_JPEG == config->pixel_format && FRAMESIZE_SVGA > config->frame_size) {
        sensor_t *s = esp_camera_sensor_get();
        s->set_framesize(s, config->frame_size);
    }
    return ret!=ESP_OK;
}

int rh_camera__save2memory(void *__buf, size_t __buf_len, size_t *__actual_size){
    camera_fb_t *fb = esp_camera_fb_get();
    if( !fb || !__buf )
        return -1;

    if( __actual_size ){
        *__actual_size = fb->len;
    }
    
    memcpy( __buf, fb->buf, __buf_len<fb->len?__buf_len:fb->len);
    return 0;
}

int rh_camera__save2file  (const char* __pf ){
    camera_fb_t *fb = esp_camera_fb_get();
    if( !fb || !__pf )
        return -1;

    int ret = rh_sdio__echo( __pf, fb->buf, fb->len, "w");
    esp_camera_fb_return(fb);
    return ret!=0;
}



namespace rh{

int Camera::init(void){
    RH_CONSOLE("%s", __func__);
    if( this->config.isValid ){
        // RH_CONSOLE("%d", ((camera_config_t*)this->config.params)->pin_d0);
        // RH_CONSOLE("%d", ((camera_config_t*)this->config.params)->pin_d1);
        // RH_CONSOLE("%d", ((camera_config_t*)this->config.params)->pin_d2);
        // RH_CONSOLE("%d", ((camera_config_t*)this->config.params)->pin_d3);
        // RH_CONSOLE("%d", ((camera_config_t*)this->config.params)->pin_d4);
        // RH_CONSOLE("%d", ((camera_config_t*)this->config.params)->pin_d5);
        // RH_CONSOLE("%d", ((camera_config_t*)this->config.params)->pin_d6);
        // RH_CONSOLE("%d", ((camera_config_t*)this->config.params)->pin_d7);
        // RH_CONSOLE("%d", ((camera_config_t*)this->config.params)->pin_pwdn    );
        // RH_CONSOLE("%d", ((camera_config_t*)this->config.params)->pin_reset   );
        // RH_CONSOLE("%d", ((camera_config_t*)this->config.params)->pin_xclk    );
        // RH_CONSOLE("%d", ((camera_config_t*)this->config.params)->pin_sscb_sda);
        // RH_CONSOLE("%d", ((camera_config_t*)this->config.params)->pin_sscb_scl);

        // RH_CONSOLE("%d", ((camera_config_t*)this->config.params)->fb_count);
        // RH_CONSOLE("%d", ((camera_config_t*)this->config.params)->jpeg_quality);
        // RH_CONSOLE("%d", ((camera_config_t*)this->config.params)->grab_mode);
        // RH_CONSOLE("%d", ((camera_config_t*)this->config.params)->ledc_channel);
        // RH_CONSOLE("%d", ((camera_config_t*)this->config.params)->ledc_timer);
        // RH_CONSOLE("%d", ((camera_config_t*)this->config.params)->pixel_format);
        // RH_CONSOLE("%d", ((camera_config_t*)this->config.params)->xclk_freq_hz);
        // RH_CONSOLE("%d", ((camera_config_t*)this->config.params)->fb_location);
        // RH_CONSOLE("%d", ((camera_config_t*)this->config.params)->frame_size);
        // RH_CONSOLE("%d", ((camera_config_t*)this->config.params)->jpeg_quality);

        return rh_camera__init( this->config.params );
    }
    return rh_camera__init();
}

}
