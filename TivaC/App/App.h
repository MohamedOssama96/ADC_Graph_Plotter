/*
 * App.h
 *
 *  Created on: Feb 19, 2020
 *      Author: Mohamed Ossama
 */

#ifndef APP_H_
#define APP_H_

#include "stdint.h"

/* Task Called Once to initialize all the Modules */
void Init_Task(void);

/* Task called every 10 Mili-seconds */
void App_Task(void);

#endif /* APP_H_ */
