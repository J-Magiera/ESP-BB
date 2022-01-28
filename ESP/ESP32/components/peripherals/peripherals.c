#include "peripherals.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "app_adc.h"


TaskHandle_t xBuzzerHandle = NULL;   
char txt[20];

static const char *TAG = "PERIPHERALS";
static void buzzerTask(void *taskParams) {
    for(;;){
        gpio_set_level(15, 0);
        vTaskDelay(500/portTICK_PERIOD_MS);
        gpio_set_level(15, 1);
        vTaskDelay(500/portTICK_PERIOD_MS);
    }
}

void initBuzzer(int priority) {
    ESP_LOGI(TAG,"Starting buzzer task");
    xTaskCreate(buzzerTask, "Buzzer task", 1000, NULL, priority, &xBuzzerHandle );
    vTaskSuspend( xBuzzerHandle );
}

void stopBuzzer(void) {
    if( xBuzzerHandle != NULL)
    {
        vTaskSuspend( xBuzzerHandle );
    }
}

void resumeBuzzer(void){
    if( xBuzzerHandle != NULL)
    {
        vTaskResume( xBuzzerHandle );
    }
}

void initADC(void){
    ESP_LOGI(TAG,"Starting ADC task");
    appADCInit();
}

const char* ADCRead(void){
    printf("ADC reads\n");
    sprintf(txt, "%u", appADCRead());
    return txt;
}
