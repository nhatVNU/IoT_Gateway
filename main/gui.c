/**
********************************************************************************
* @file         gui.c
* @brief        Graphical User Interface module
* @since        Created on 2023-09-26
* @author       Tran Minh Nhat - 2014008
********************************************************************************
*/

#include "gui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "esp_vfs.h"
#include "esp_spiffs.h"
#include "esp_heap_caps.h"
#include "ili9340.h"
#include "fontx.h"
#include "bmpfile.h"
#include "decode_jpeg.h"
#include "decode_png.h"
#include "pngle.h"
#include "driver/gpio.h"


FontxFile fx16G[2];
FontxFile fx24G[2];
FontxFile fx32G[2];
FontxFile fx32L[2];
FontxFile fx16M[2];
FontxFile fx24M[2];
FontxFile fx32M[2];

static TFT_t dev;
static const char *TAG = "GUI module";

TickType_t FillTest(TFT_t * dev, int width, int height) 
{
	TickType_t startTick, endTick, diffTick;
	startTick = xTaskGetTickCount();

	lcdFillScreen(dev, RED);
	vTaskDelay(50);
	lcdFillScreen(dev, GREEN);
	vTaskDelay(50);
	lcdFillScreen(dev, BLUE);
	vTaskDelay(50);

	endTick = xTaskGetTickCount();
	diffTick = endTick - startTick;
	ESP_LOGI(__FUNCTION__, "elapsed time[ms]:%"PRIu32,diffTick*portTICK_PERIOD_MS);
	return diffTick;
}

void guiInit(void)
{
    InitFontx(fx16G,"/spiffs/ILGH16XB.FNT",""); // 8x16Dot  Gothic
	InitFontx(fx24G,"/spiffs/ILGH24XB.FNT",""); // 12x24Dot Gothic
	InitFontx(fx32G,"/spiffs/ILGH32XB.FNT",""); // 16x32Dot Gothic
	InitFontx(fx32L,"/spiffs/LATIN32B.FNT",""); // 16x32Dot Latinc
    InitFontx(fx16M,"/spiffs/ILMH16XB.FNT",""); // 8x16Dot  Mincyo
	InitFontx(fx24M,"/spiffs/ILMH24XB.FNT",""); // 12x24Dot Mincyo
	InitFontx(fx32M,"/spiffs/ILMH32XB.FNT",""); // 16x32Dot Mincyo

    spi_master_init(&dev, MOSI_GPIO, SCLK_GPIO, CS_GPIO, DC_GPIO, RESET_GPIO, BACKLIGHT_GPIO, 
                    XPT_MISO_GPIO, XPT_CS_GPIO, XPT_IRQ_GPIO, XPT_SCLK_GPIO, XPT_MOSI_GPIO);
    uint16_t model = 0x9341;    //Using ILI9341 driver
    lcdInit(&dev, model, SCREEN_WIDTH, SCREEN_HEIGHT, GRAM_X_OFFSET, GRAM_Y_OFFSET);

#if CONFIG_USE_RGB_COLOR
    ESP_LOGI(TAG, "Change BGR filter to RGB filter");
    lcdBGRFilter(&dev);
#endif 
}

void GUI(void *pvParameters)
{
    guiInit();
    FillTest(&dev, SCREEN_WIDTH, SCREEN_HEIGHT);
    while(1){
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

