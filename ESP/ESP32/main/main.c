#include "app_mqtt.h"
#include "esp_log.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"
#include "freertos/task.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include "nvs_flash.h"
#include "wifi_app.h"
#include <string.h>

#define BLINK_GPIO 15
#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE

BaseType_t xLedTaskHandle;
TaskHandle_t xHandle = NULL;
static const char *TAG = "MAIN";
void ledTask(void *taskParams){
  uint32_t cntr = 0;
  uint16_t cntr2 = 0;
  while(1){
    vTaskDelay(cntr2/portTICK_PERIOD_MS);
    if(cntr2 < 100){
      cntr2++;
    }
    else{
      cntr2 = 0;
    }
    gpio_set_level(BLINK_GPIO, cntr);
    cntr = !cntr;
  }

}

void app_main(void) {

  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
      ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  gpio_reset_pin(BLINK_GPIO);
  gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
  ESP_LOGI(TAG, "ESP_WIFI_MODE_AP");
  wifi_init();
  if(wifi_get_status() == CONNECTED){
    app_mqtt_start();
    xTaskCreate(ledTask, "LED task", 512, NULL, 13, xHandle );
    if( xHandle != NULL )
    {
      vTaskDelete( xHandle );
    }
  }
}
