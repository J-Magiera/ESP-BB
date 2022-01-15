/**
 * @file CLI.h
 * @author MagieraJ
 * @brief 
 * @version 0.1
 * @date 2022-01-07
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef CLI_H_
#define CLI_H_


typedef enum {
    CMD_OK,
    CMD_WRONG_PARAM,
    CMD_TOO_LONG,
    CMD_UNSUPPORTED
} cmd_error_t;

typedef struct {
    char* command;
    cmd_error_t (*callback)(char*);
} cmd_t;

void cmdAnalize(char* cmd);
#endif