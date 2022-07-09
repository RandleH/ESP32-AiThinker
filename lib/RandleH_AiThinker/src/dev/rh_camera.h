#include <stdint.h>
#include <stddef.h>

#ifndef RH_CAMERA_H
#define RH_CAMERA_H

/*==========================================================================
Parameter notation table
============================================================================
* __buf            Pointer to the buffer
* __buf_size       Bytewise memory size of the buffer
* __actural_size   The actural bytewise memory size of the camera
* __pf             String contains both path and file name
============================================================================*/


int   rh_camera__init        (void);
int   rh_camera__start       (void);//
int   rh_camera__stop        (void);//
int   rh_camera__save2memory (void       *__buf, size_t __buf_len, size_t *__actual_size);
int   rh_camera__save2file   (const char* __pf );


int   rh_camera__save_raw    (const char *__pf);
int   rh_camera__save_bmp    (const char *__pf);//
int   rh_camera__save_jpg    (const char *__pf);//
int   rh_camera__exist       (void);//

int   rh_camera__brightness ( uint8_t x);//
int   rh_camera__contrast   ( uint8_t x);//
int   rh_camera__saturation ( uint8_t x);//




#endif