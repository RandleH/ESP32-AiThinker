

#include "driver/ledc.h"
#include "./schematic.h"

void rh_misc__init(void){
// Enable Camera Out Clock
// LEDC Timer -- Camera
    {
        ledc_timer_config_t timer_conf;
        timer_conf.duty_resolution = LEDC_TIMER_1_BIT;
        timer_conf.freq_hz         = CAMERA_XCLK_FREQ;              // xclk_freq_hz
        timer_conf.speed_mode      = LEDC_LOW_SPEED_MODE;
        timer_conf.clk_cfg         = LEDC_AUTO_CLK;
        timer_conf.timer_num       = (ledc_timer_t)LEDC_TIMER_0;
        ledc_timer_config(&timer_conf);
    }

// LEDC Timer Channel -- Camera
    {
        ledc_channel_config_t ch_conf;
        ch_conf.gpio_num   = CAMERA_XCLK_GPIO;
        ch_conf.speed_mode = LEDC_LOW_SPEED_MODE;
        ch_conf.channel    = LEDC_CHANNEL_0;
        ch_conf.intr_type  = LEDC_INTR_DISABLE;
        ch_conf.timer_sel  = LEDC_TIMER_0;
        ch_conf.duty       = 1;
        ch_conf.hpoint     = 0;
        ledc_channel_config(&ch_conf);
    }    

}