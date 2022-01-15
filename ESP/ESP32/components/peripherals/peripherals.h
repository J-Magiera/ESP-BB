/**
 * @file peripherals.h
 * @author Magieraj
 * @brief 
 * @version 0.1
 * @date 2022-01-07
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef PERIPHERALS_H_
#define PERIPHERALS_H_

void initBuzzer(int priority);
void initADC(void);
void resumeBuzzer(void);
void stopBuzzer(void);
const char* ADCRead(void);


#endif