/**
 * @file mqtt_app.c
 * @author MagieraJ
 * @brief
 * @version 0.1
 * @date 2021-12-28
 *
 * @copyright Copyright (c) 2021
 *
 */
#include "app_mqtt.h"
#include "esp_event.h"
#include "esp_log.h"
#include "freertos/event_groups.h"
#include "mqtt_client.h"
#include "peripherals.h"
#include <string.h>
#include "CLI.h"

#define HOST_IP MQTT_HOST_IP
#define USERNAME MQTT_USERNAME
#define CLIENT_ID MQTT_CLIENT_ID
#define PASSWORD MQTT_PASSWORD
#define MAX_CMD_LENGTH 40
#define SENSOR_ID "2"

static const char *TAG = "MQTT";
static esp_mqtt_client_handle_t client = NULL;

static void mqtt_topic_handler(esp_mqtt_event_handle_t event){
  if(event != NULL) {
    printf("topic_handler initiated, data =%.*s\n", event->data_len, event->data);
    if(strncmp(event->topic, "topic/2/CLI", event->topic_len) == 0 && event->data_len <= MAX_CMD_LENGTH){
      char msg[MAX_CMD_LENGTH];
      strncpy(msg, event->data, event->data_len);
      cmdAnalize(msg);
    }
  }  
}

static void log_error_if_nonzero(const char *message, int error_code) {
  if (error_code != 0) {
    ESP_LOGE(TAG, "Last error %s: 0x%x", message, error_code);
  }
}

static void mqtt_event_handler(void *handler_args, esp_event_base_t base,
                               int32_t event_id, void *event_data) {
  ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%d", base,
           event_id);
  esp_mqtt_event_handle_t event = event_data;
  client = event->client;
  int msg_id;

  switch ((esp_mqtt_event_id_t)event_id) {
    case MQTT_EVENT_CONNECTED:
      ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
      msg_id = esp_mqtt_client_subscribe(client, "/topic/2/CLI", 0);
      ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);
      break;

    case MQTT_EVENT_DISCONNECTED:
      ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
      break;

    case MQTT_EVENT_SUBSCRIBED:
      break;
    case MQTT_EVENT_UNSUBSCRIBED:
      break;
    case MQTT_EVENT_PUBLISHED:
      ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
      break;
    case MQTT_EVENT_DATA:
      ESP_LOGI(TAG, "MQTT_EVENT_DATA");
      mqtt_topic_handler(event);
      printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
      printf("DATA=%.*s\r\n", event->data_len, event->data);
      break;
    case MQTT_EVENT_ERROR:
      ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
      if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT) {
        log_error_if_nonzero("reported from esp-tls",
                            event->error_handle->esp_tls_last_esp_err);
        log_error_if_nonzero("reported from tls stack",
                            event->error_handle->esp_tls_stack_err);
        log_error_if_nonzero("captured as transport's socket errno",
                            event->error_handle->esp_transport_sock_errno);
        ESP_LOGI(TAG, "Last errno string (%s)",
                strerror(event->error_handle->esp_transport_sock_errno));
      }
      break;
    default:
      ESP_LOGI(TAG, "Other event id:%d", event->event_id);
      break;
    }
}

void appMQTTStart(void) {
  const esp_mqtt_client_config_t mqtt_cfg = {.host = "192.168.0.157",
                                             .port = 1883,
                                             .client_id = "sensor2",
                                             .username = "sensor",
                                             .password = "password",
                                             .disable_clean_session = false};

  client = esp_mqtt_client_init(&mqtt_cfg);
  if (client == NULL)
	{
		ESP_LOGI(TAG, "fail");
	}
  esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler,
                                 NULL);
  esp_mqtt_client_start(client);
}

esp_err_t appMQTTPublish(const char* topic, const char* data){ 
  if(client == NULL)
  {
    //ESP_LOGI(TAG, "Event fail: %d", event->event_id);
    return ESP_FAIL;
  }
  esp_mqtt_client_publish(client, topic, data,
                                            strlen(data), 0, 0);
  return ESP_OK;

}