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
#include "peripherals.h"
#include <string.h>

#define BLINK_GPIO 15
#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
int blink_gpio = BLINK_GPIO;

static const char *TAG = "MAIN";

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
  wifiInit();
  if(wifiGetStatus() == CONNECTED){
    appMQTTStart();
    initBuzzer(10);   
  }
}
