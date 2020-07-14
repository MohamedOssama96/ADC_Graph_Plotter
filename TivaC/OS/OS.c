/*
 * OS.c
 *
 *  Created on: Feb 19, 2020
 *      Author: Mohamed Ossama
 */

#include "OS.h"
#include "SysTick.h"

/* Global variable to indicate the the timer has a new tick */
static uint8_t g_New_Time_Tick_Flag = 0;

/*********************************************************************************************/
void OS_Start(void)
{
    /*
     * Set the Call Back function call to Scheduler_New_Timer_Tick
     * this function will be called every SysTick Compare Interrupt(20ms)
     */
    SysTick_SetCallBack(OS_NewTimerTick);

    /* Start Timer0 in Compare Mode to generate interrupt every 20ms */
    SysTick_Start(OS_BASE_TIME);

    /* Execute the Init Task */
    Init_Task();

    /* Start the Os Scheduler */
    OS_Scheduler();
}
/*********************************************************************************************/
void OS_NewTimerTick(void)
{
    /* Set the flag to 1 to indicate that there is a new timer tick */
    g_New_Time_Tick_Flag = 1;
}
/*********************************************************************************************/
void OS_Scheduler(void)
{
    while(1)
    {
        /* Code is only executed in case there is a new timer tick */
        if(g_New_Time_Tick_Flag == 1)
        {
            App_Task();
            g_New_Time_Tick_Flag = 0;
        }
    }

}
/*********************************************************************************************/
