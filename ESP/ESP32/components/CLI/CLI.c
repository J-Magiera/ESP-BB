/**
 * @file CLI.c
 * @author MagieraJ
 * @brief 
 * @version 0.1
 * @date 2022-01-07
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "CLI.h"
#include <string.h>
#include "peripherals.h"
#include "app_mqtt.h"

#define END_OF_LINE_BYTE_SET " \n"
#define N_COMMANDS 3

static const char* return_topic = "/topic/CLI/return";
static cmd_error_t cmd_info_callback(char*);
static cmd_error_t cmd_buzzer_callback(char*);
static cmd_error_t cmd_adc_callback(char*);

static const cmd_t CLI[N_COMMANDS] = {
    {"info", &cmd_info_callback},
    {"buzzer", &cmd_buzzer_callback},
    {"adc", &cmd_adc_callback},
};

void cmdAnalize(char* cmd) {
    cmd_error_t CMD_ERROR = CMD_OK;
    char* token;
    printf("data in analize: %s\n", cmd);
    token = strtok(cmd , END_OF_LINE_BYTE_SET);
    printf("data after stuff: %s\n", cmd);
    while (*token != '\0') {
        for (int j = 0; j < N_COMMANDS; j++) {
            if (strncmp(token, CLI[j].command,
                        strlen(CLI[j].command)) == 0) {
                CMD_ERROR = CLI[j].callback(strtok(NULL, " "));
                break;
            }
            CMD_ERROR = CMD_UNSUPPORTED;
        }

        if (CMD_ERROR == CMD_OK) {
            break;
        } else {
            int j = (int)strcspn(token, END_OF_LINE_BYTE_SET);
            token = token + j + 1;
        }
    }

    switch (CMD_ERROR) {
        case CMD_WRONG_PARAM:
            appMQTTPublish(return_topic,"Wrong command or parameter.");
            break;
        case CMD_TOO_LONG:
            appMQTTPublish(return_topic,"Parameter too long.");
            break;
        case CMD_UNSUPPORTED:
            appMQTTPublish(return_topic,"Unsupported command.");
            break;
        default:
            break;
    }
}


static cmd_error_t cmd_info_callback(char* command){
    (void)command;
    static const char* info_msg = "This is info callback!\n";
    appMQTTPublish(return_topic, info_msg);
    return CMD_OK;
}

static cmd_error_t cmd_buzzer_callback(char* command){
    char* _cmd;
    _cmd = strtok(command, END_OF_LINE_BYTE_SET);
    printf("I'm here %s\n", _cmd);
    if(strncmp(_cmd, "run", strlen("set")) == 0){
        resumeBuzzer();
        return CMD_OK;
    }else if (strncmp(_cmd, "stop", strlen("stop")) == 0){
        stopBuzzer();
        return CMD_OK;
    }
    return CMD_UNSUPPORTED;
}

static cmd_error_t cmd_adc_callback(char* command){
    char* _cmd;
    printf("I'm here %s\n", command);
    _cmd = strtok(command, END_OF_LINE_BYTE_SET);
    if(strncmp(_cmd, "init", strlen("init")) == 0){
        initADC();
        return CMD_OK;
    }else if (strncmp(_cmd, "read", strlen("read")) == 0){        
        appMQTTPublish(return_topic, ADCRead());
        return CMD_OK;
    }
    return CMD_UNSUPPORTED;
}