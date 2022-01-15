/**
 * @file wifi_app.h
 * @author MagieraJ
 * @brief
 * @version 0.1
 * @date 2021-12-28
 *
 * @copyright Copyright (c) 2021
 *
 */
#ifndef WIFI_APP_H_
#define WIFI_APP_H_


typedef enum{
    STARTED,
    STOPPED,
    DISCONNECTED,
    CONNECTED
}wifi_status_t;

wifi_status_t wifiGetStatus(void);
void wifiInit(void);

#endif