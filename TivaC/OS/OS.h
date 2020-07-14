/*
 * OS.h
 *
 *  Created on: Feb 19, 2020
 *      Author: Mohamed Ossama
 */

#ifndef OS_H_
#define OS_H_

#include "stdint.h"

/* System tick counting time in ms */
#define OS_BASE_TIME 10

/* Function responsible for:
 * 1. Initialize the Os
 * 2. Execute the Init Task
 * 3. Start the Scheduler to run the tasks
 */
void OS_Start(void);

/* The Engine of the Os Scheduler used for switch between different tasks */
void OS_Scheduler(void);

/* Function called by the SysTick Driver in the MCAL layer using the call back pointer */
void OS_NewTimerTick(void);

#endif /* OS_H_ */
