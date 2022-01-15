/**
 * @file mqtt_app.h
 * @author MagieraJ
 * @brief 
 * @version 0.1
 * @date 2021-12-28
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "esp_err.h"
#ifndef APP_MQTT_H_
#define APP_MQTT_H_

void appMQTTStart(void);
esp_err_t appMQTTPublish(const char* topic, const char* data);

#endif