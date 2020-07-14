/*
 * SysTick.h
 *
 *  Created on: Feb 19, 2020
 *      Author: Mohamed Ossama
 */

#ifndef SYSTICK_H_
#define SYSTICK_H_

#include "stdint.h"
#include "tm4c123gh6pm.h"

#define SYSTEM_CLOCK    16000000U
#define ONE_SECOND_MS   1000U
#define NULL            ((void*) 0)

/*
 * Description: this function responsible for :
 *  1. Initialize SysTick HW
 *  2. Set the Compare Register Value to generate interrupt every Tick_Time ms
 */
void SysTick_Start(uint16_t Tick_Time);

/* Points to the required function in the OS upper layer Scheduler */
void SysTick_SetCallBack(void (*Ptr2Func)(void));

#endif /* SYSTICK_H_ */
