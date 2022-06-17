#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include "driver/sdmmc_host.h"

#include "./schematic.h"


static sdmmc_card_t *card;

int rh_sdio__init  (void){
    sdmmc_host_t        host              = SDMMC_HOST_DEFAULT();         
    sdmmc_slot_config_t slot_config       = SDMMC_SLOT_CONFIG_DEFAULT();
                        slot_config.width = 4;
                        slot_config.flags |= SDMMC_SLOT_FLAG_INTERNAL_PULLUP;
                        // 以下无需配置 ESP32只有一个SD Slot
                        // "SOC_SDMMC_USE_GPIO_MATRIX" is only for ESP32 S3
                        // slot_config.d0    = (gpio_num_t)SDCARD_D0_GPIO;
                        // slot_config.d1    = (gpio_num_t)SDCARD_D1_GPIO;
                        // slot_config.d2    = (gpio_num_t)SDCARD_D2_GPIO;
                        // slot_config.d3    = (gpio_num_t)SDCARD_D3_GPIO;
                        // slot_config.d4    = ...
                        // slot_config.d5    = ...
                        // slot_config.d6    = ...
                        // slot_config.d7    = ...
                        // slot_config.cmd   = (gpio_num_t)SDCARD_CMD_GPIO;
                        // slot_config.clk   = (gpio_num_t)SDCARD_CLK_GPIO;

    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = true,
        .max_files              = 5,
        .allocation_unit_size   = 16 * 1024
    };                    

    esp_err_t ret = esp_vfs_fat_sdmmc_mount( SDCARD_MOUNT_POINT, &host, &slot_config, &mount_config, &card );
    
    {// 初始化SD Card挂节点, 尝试10次, 若失败则返回
        size_t count = 10;
        while(--count && ret!=ESP_OK){
            ret = esp_vfs_fat_sdmmc_mount( SDCARD_MOUNT_POINT, &host, &slot_config, &mount_config, &card );
        }
    }
    return ret!=ESP_OK;
}


int  rh_sdio__unmount(void){
    esp_err_t ret = esp_vfs_fat_sdcard_unmount( SDCARD_MOUNT_POINT, card);
    if( ret!= ESP_OK ){
        return 1;
    }
    card = NULL;
    return 0;
}


void rh_sdio__test(void){
    rh_sdio__init();

    // int v = rh_sdcard__touch( "/sdcard/randleh.txt" );
    
    // while( v==-1 );
    // rh_sdcard__echo( "/sdcard/randleh.txt", "Hello, world!", 13);

    rh_sdio__unmount();
}







