
#include <string.h>
#include <stdlib.h>
#include "esp_heap_caps.h"
#include "../schematic.h"

#include "./support/esp_camera.h"
#include "./support/sensor.h"


static esp_err_t init_camera(uint32_t xclk_freq_hz, pixformat_t pixel_format, framesize_t frame_size, uint8_t fb_count)
{
    framesize_t size_bak = frame_size;
    if (PIXFORMAT_JPEG == pixel_format && FRAMESIZE_SVGA > frame_size) {
        frame_size = FRAMESIZE_HD;
    }
    camera_config_t camera_config = {
        .pin_pwdn     = CAMERA_PWDN_GPIO,
        .pin_reset    = CAMERA_RESET_GPIO,
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
        .pin_vsync = CAMERA_VSYNC_GPIO,
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

        .fb_location = CAMERA_FB_IN_PSRAM
    };

    //initialize the camera
    esp_err_t ret = esp_camera_init(&camera_config);
    
    if( ret!=ESP_OK ) while(1);   // PASSED


    if (ESP_OK == ret && PIXFORMAT_JPEG == pixel_format && FRAMESIZE_SVGA > size_bak) {
        sensor_t *s = esp_camera_sensor_get();
        s->set_framesize(s, size_bak);
    }

    return ret;
}

void  rh_camera__init        (void){
    esp_err_t ret = init_camera( 10000000, PIXFORMAT_JPEG, FRAMESIZE_UXGA, 2);
    if( ret!=ESP_OK ) while(1);   // PASSED
}

int   rh_camera__start       (void){
    return 0;
}


void *rh_camera__capture (void *__buf, size_t __buf_size, size_t *__actual_size){
    if( !__buf ) return NULL;
    camera_fb_t* frame = esp_camera_fb_get();

    if( frame->len > __buf_size ) return NULL;
    if( __actual_size!=NULL ){
        *__actual_size = frame->len;
    }

    return memcpy( __buf, frame->buf, frame->len );
}
 


inline static int rh_camera__565_2_888  ( const uint8_t *src, uint8_t *dst, size_t width, size_t height){
    if( !src || !dst ) return -1;
    for( size_t y=0; y<height; ++y){
        for( size_t x=0; x<width; ++x ){
            *dst = (uint8_t)((0x1f&((*((uint16_t*)src))>>11))<<3);  ++dst;
            *dst = (uint8_t)((0x3f&((*((uint16_t*)src))>> 5))<<2);  ++dst;
            *dst = (uint8_t)((0x1f&((*((uint16_t*)src))    ))<<3);  ++dst;
            src+=2;
        }
    }
    return 0;
}

#include "./sdcard.h"
int rh_camera__save_raw    (const char *__pf){
    camera_fb_t* frame = esp_camera_fb_get();
    if( !frame ) return 1;

    rh_sdcard__echo( __pf, "w", (const char*)frame->buf, frame->len);    
    return 0;
}

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "./support/stb_image.h"
#include "./support/stb_image_write.h"
int rh_camera__save_bmp    (const char *__pf){
    camera_fb_t* frame = esp_camera_fb_get();
    if( !frame ) return 1;

    int     comp = 5;
    uint8_t *buf = NULL;
    switch( frame->format ){
        case PIXFORMAT_RGB888:    
            comp=3; 
            break;

        case PIXFORMAT_RGB565:    // 2BPP/RGB565
            comp=3; 
            buf = (uint8_t*)heap_caps_malloc( 3*(frame->width)*(frame->height), MALLOC_CAP_SPIRAM|MALLOC_CAP_8BIT);
            while(!buf);
            rh_camera__565_2_888( frame->buf, buf, frame->width, frame->height);
            break;  

        case PIXFORMAT_GRAYSCALE: // 1BPP/GRAYSCALE
            comp=1; 
            break;   
    
        default: return -1;     // Not support yet
    }
    stbi_write_bmp( __pf, (int)(frame->width), (int)(frame->height), comp, (buf==NULL)?frame->buf:buf);

    if(buf!=NULL) free(buf);

    return 0;
}

int rh_camera__save_jpg    (const char *__pf){
    camera_fb_t* frame = esp_camera_fb_get();
    if( !frame ) return 1;

    int     comp = 5;
    uint8_t *buf = NULL;
    switch( frame->format ){
        case PIXFORMAT_JPEG:
            rh_sdcard__echo( __pf, "w", (const char*)frame->buf, frame->len); 
            break;
        default: 
            return -1;
    }

    return 0;
}

