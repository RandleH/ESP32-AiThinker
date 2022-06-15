
// #include "/esp_private/periph_ctrl.h"    
#include "driver/i2s.h"


typedef enum {
    /* camera sends byte sequence: s1, s2, s3, s4, ...
     * fifo receives: 00 s1 00 s2, 00 s2 00 s3, 00 s3 00 s4, ...
     */
    SM_0A0B_0B0C = 0,
    /* camera sends byte sequence: s1, s2, s3, s4, ...
     * fifo receives: 00 s1 00 s2, 00 s3 00 s4, ...
     */
    SM_0A0B_0C0D = 1,
    /* camera sends byte sequence: s1, s2, s3, s4, ...
     * fifo receives: 00 s1 00 00, 00 s2 00 00, 00 s3 00 00, ...
     */
    SM_0A00_0B00 = 3,
} i2s_sampling_mode_t;


void rh_i2s__init(void){

// CAMERA I2S INIT    
    periph_module_enable(PERIPH_I2S0_MODULE);

    I2S0.conf.rx_reset         = 1;
    I2S0.conf.rx_reset         = 0;
    I2S0.conf.rx_fifo_reset    = 1;
    I2S0.conf.rx_fifo_reset    = 0;
    I2S0.lc_conf.in_rst        = 1;
    I2S0.lc_conf.in_rst        = 0;
    I2S0.lc_conf.ahbm_fifo_rst = 1;
    I2S0.lc_conf.ahbm_fifo_rst = 0;
    I2S0.lc_conf.ahbm_rst      = 1;
    I2S0.lc_conf.ahbm_rst      = 0;

    I2S0.conf.rx_slave_mod     = 1;
    I2S0.conf.rx_right_first   = 0;
    I2S0.conf.rx_msb_right     = 0;
    I2S0.conf.rx_msb_shift     = 0;
    I2S0.conf.rx_mono          = 0;
    I2S0.conf.rx_short_sync    = 0;

    I2S0.conf2.lcd_en          = 1;
    I2S0.conf2.camera_en       = 1;

    // Configure clock divider
    I2S0.clkm_conf.clkm_div_a   = 0;
    I2S0.clkm_conf.clkm_div_b   = 0;
    I2S0.clkm_conf.clkm_div_num = 2;
    
    I2S0.fifo_conf.dscr_en              = 1;
    I2S0.fifo_conf.rx_fifo_mod          = SM_0A0B_0C0D;    // UNSOLVED
    I2S0.fifo_conf.rx_fifo_mod_force_en = 1;

    I2S0.conf_chan.rx_chan_mod          = 1;
    I2S0.sample_rate_conf.rx_bits_mod   = 0;
    I2S0.timing.val                     = 0;
    I2S0.timing.rx_dsync_sw             = 1;
}



