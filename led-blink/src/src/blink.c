#include "esp_common.h"
#include "freertos/task.h"
#include "gpio.h"

/******************************************************************************
 * FunctionName : user_rf_cal_sector_set
 * Description  : SDK just reversed 4 sectors, used for rf init data and paramters.
 *                We add this function to force users to set rf cal sector, since
 *                we don't know which sector is free in user's application.
 *                sector map for last several sectors : ABCCC
 *                A : rf cal
 *                B : rf init data
 *                C : sdk parameters
 * Parameters   : none
 * Returns      : rf cal sector
*******************************************************************************/
uint32 user_rf_cal_sector_set(void)
{
    flash_size_map size_map = system_get_flash_size_map();
    uint32 rf_cal_sec = 0;
    switch (size_map)
    {
    case FLASH_SIZE_4M_MAP_256_256:
        rf_cal_sec = 128 - 5;
        break;

    case FLASH_SIZE_8M_MAP_512_512:
        rf_cal_sec = 256 - 5;
        break;

    case FLASH_SIZE_16M_MAP_512_512:
    case FLASH_SIZE_16M_MAP_1024_1024:
        rf_cal_sec = 512 - 5;
        break;

    case FLASH_SIZE_32M_MAP_512_512:
    case FLASH_SIZE_32M_MAP_1024_1024:
        rf_cal_sec = 1024 - 5;
        break;

    default:
        rf_cal_sec = 0;
        break;
    }

    return rf_cal_sec;
}

void task_blink(void* ignore)
{

    //! deklarasi pin 16 atau D0 untuk
    GPIO_ConfigTypeDef io_out_conf;
    io_out_conf.GPIO_IntrType   = GPIO_PIN_INTR_DISABLE; //! tidak menggunakan interrupt
    io_out_conf.GPIO_Mode       = GPIO_Mode_Output; //! sebagai output
    io_out_conf.GPIO_Pin        = 16; //! pin 16 atau D0
    io_out_conf.GPIO_Pullup     = GPIO_PullUp_EN; //! aktifkan pullup internal
    gpio_config(&io_out_conf);


    while(true) //! ulang selama lama lama nya
    {
        GPIO_OUTPUT_SET(16,0); //! D0 = 0, LED nyala
        vTaskDelay(1000/portTICK_RATE_MS); //! tunggu 1000ms = 1s detik
        GPIO_OUTPUT_SET(16,1); //! D0 = 1, LED mati
        vTaskDelay(1000/portTICK_RATE_MS); //! tunggu 1000ms = 1s detik
    }

    vTaskDelete(NULL);
}

/******************************************************************************
 * FunctionName : user_init
 * Description  : entry of user application, init user function here
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void user_init(void)
{
    xTaskCreate(&task_blink, "startup", 2048, NULL, 1, NULL);
}


