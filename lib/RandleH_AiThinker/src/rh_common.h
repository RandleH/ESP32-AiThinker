#ifndef RH_COMMON_H
#define RH_COMMON_H

#ifdef __cplusplus
extern "C"{
#endif



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



#ifdef __cplusplus
}
#endif

#endif
