idf_component_register(SRCS "main.c"
                            "../bsp/gpio.c"
                            "../bsp/sdio.c"
                            "../bsp/i2s.c"
                            "../bsp/misc.c"
                            "../bsp/wifi.c"
                            
                            "../bsp/dev/led.c"
                            "../bsp/dev/sdcard.c"
                            "../bsp/dev/camera.c"
                            
                            "../bsp/dev/support/esp_camera.c"
                            "../bsp/dev/support/sensor.c"
                            "../bsp/dev/support/cam_hal.c"
                            "../bsp/dev/support/ll_cam.c"
                            "../bsp/dev/support/xclk.c"
                            "../bsp/dev/support/sccb.c"
                            "../bsp/dev/support/ov2640.c"

                            "../app/event.c"
                            "../app/tcpip.c"
                            



                    INCLUDE_DIRS ".")





                    