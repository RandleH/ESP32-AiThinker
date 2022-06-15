
#include <string.h>

#include "driver/gpio.h"
#include "hal/gpio_hal.h"
#include "soc/io_mux_reg.h"
#include "soc/gpio_sig_map.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "./schematic.h"


#define CAMERA_RESET_GPIO    (-1) //software reset will be performed
#define CAMERA_XCLK_GPIO     (0U)
#define CAMERA_SIOD_GPIO     (26U)
#define CAMERA_SIOC_GPIO     (27U)
#define CAMERA_PWDN_GPIO     (32U)

#define CAMERA_Y9_GPIO       (35U)
#define CAMERA_Y8_GPIO       (34U)
#define CAMERA_Y7_GPIO       (39U)
#define CAMERA_Y6_GPIO       (36U)
#define CAMERA_Y5_GPIO       (21U)
#define CAMERA_Y4_GPIO       (19U)
#define CAMERA_Y3_GPIO       (18U)
#define CAMERA_Y2_GPIO       (5U)
#define CAMERA_VSYNC_GPIO    (25U)
#define CAMERA_HREF_GPIO     (23U)
#define CAMERA_PCLK_GPIO     (22U)


void rh_gpio__init   (void){

// LED GPIO INIT
    {
        gpio_reset_pin    ( BOARD_LED_GPIO);
        gpio_reset_pin    ( FLUSH_LED_GPIO);
        gpio_set_direction( BOARD_LED_GPIO, GPIO_MODE_OUTPUT);
        gpio_set_direction( FLUSH_LED_GPIO, GPIO_MODE_OUTPUT);

    }
    

// CAMERA GPIO INIT
    {
        gpio_config_t io_conf = {
            .intr_type    = GPIO_PIN_INTR_NEGEDGE,  // Negtive edge interrupt trigger
            .pin_bit_mask = 1LL<<CAMERA_VSYNC_GPIO,   // Pin number of VSYNC
            .mode         = GPIO_MODE_INPUT,        // Input mode
            .pull_up_en   = true 
        };
        // gpio_config(&io_conf);
        // gpio_install_isr_service(ESP_INTR_FLAG_LOWMED | ESP_INTR_FLAG_IRAM);
        // gpio_isr_handler_add( CAMERA_VSYNC_GPIO, ll_cam_vsync_isr, cam);      // UNSOLVED
        // gpio_isr_handler_add( CAMERA_VSYNC_GPIO, NULL, NULL);                    // UNSOLVED
        // gpio_intr_disable( CAMERA_VSYNC_GPIO);

        PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[CAMERA_PCLK_GPIO], PIN_FUNC_GPIO);
        gpio_set_direction (CAMERA_PCLK_GPIO, GPIO_MODE_INPUT);
        gpio_set_pull_mode (CAMERA_PCLK_GPIO, GPIO_FLOATING  );
        gpio_matrix_in     (CAMERA_PCLK_GPIO, I2S0I_WS_IN_IDX, false);

        PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[CAMERA_VSYNC_GPIO], PIN_FUNC_GPIO);
        gpio_set_direction (CAMERA_VSYNC_GPIO, GPIO_MODE_INPUT);
        gpio_set_pull_mode (CAMERA_VSYNC_GPIO, GPIO_FLOATING  );
        gpio_matrix_in     (CAMERA_VSYNC_GPIO, I2S0I_V_SYNC_IDX, false);

        PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[CAMERA_HREF_GPIO], PIN_FUNC_GPIO);
        gpio_set_direction (CAMERA_HREF_GPIO, GPIO_MODE_INPUT);
        gpio_set_pull_mode (CAMERA_HREF_GPIO, GPIO_FLOATING  );
        gpio_matrix_in     (CAMERA_HREF_GPIO, I2S0I_H_SYNC_IDX, false);

        int data_pins[8] = {
            CAMERA_Y2_GPIO,\
            CAMERA_Y3_GPIO,\
            CAMERA_Y4_GPIO,\
            CAMERA_Y5_GPIO,\
            CAMERA_Y6_GPIO,\
            CAMERA_Y7_GPIO,\
            CAMERA_Y8_GPIO,\
            CAMERA_Y9_GPIO\
        };
        for (int i = 0; i < 8; i++) {
            PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[data_pins[i]], PIN_FUNC_GPIO);
            gpio_set_direction(data_pins[i], GPIO_MODE_INPUT);
            gpio_set_pull_mode(data_pins[i], GPIO_FLOATING);
            gpio_matrix_in(data_pins[i], I2S0I_DATA_IN0_IDX + i, false);
        }

        gpio_matrix_in( 0x38, I2S0I_H_ENABLE_IDX, false); //0x30 sends 0, 0x38 sends 1

        memset( &io_conf, 0, sizeof(gpio_config_t));
        io_conf.pin_bit_mask = 1LL << CAMERA_PWDN_GPIO;
        io_conf.mode         = GPIO_MODE_OUTPUT;
        gpio_config(&io_conf);

        // carefull, logic is inverted compared to reset pin
        gpio_set_level( CAMERA_PWDN_GPIO, 1);
        vTaskDelay(10 / portTICK_PERIOD_MS);
        gpio_set_level( CAMERA_PWDN_GPIO, 0);
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }



}

void rh_gpio__set    ( char gpio, int pin, int value){
    gpio_set_level( pin, value);
}

int  rh_gpio__get    ( char gpio, int pin){
	return gpio_get_level(pin);
}

void rh_gpio__toggle ( char gpio, int pin){
    if( pin < 32 )
        (&GPIO)->out      ^= (1<<pin);
    else
        (&GPIO)->out1.val ^= (1<<pin);   
}



