#ifndef RH_COMMON_H
#define RH_COMMON_H

#ifdef __cplusplus
extern "C"{
#endif


#include "esp_log.h"
#define RH_CONSOLE( format, ...)    ESP_LOGE("RH", format, ##__VA_ARGS__)

#define BOARD_LED_GPIO       (33U)
#define FLUSH_LED_GPIO       (4U)

#define BOARD_LED            (0)
#define FLUSH_LED            (1)

#define SDCARD_D0_GPIO       ()
#define SDCARD_D1_GPIO       ()
#define SDCARD_D2_GPIO       ()
#define SDCARD_D3_GPIO       ()
#define SDCARD_CMD_GPIO      ()
#define SDCARD_CLK_GPIO      () 

#define SDCARD_MOUNT_POINT   "/sdcard"


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

#endif
