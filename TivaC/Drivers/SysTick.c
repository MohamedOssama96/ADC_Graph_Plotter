/*
 * SysTick.c
 *
 *  Created on: Feb 19, 2020
 *      Author: Mohamed Ossama
 */

#include "SysTick.h"

/* Global pointer to function used to point upper layer functions
 * to be used in Call Back */
static volatile void (*g_SysTick_Call_Back_Ptr)(void) = NULL;

/*********************************************************************************************/
void SysTick_Start(uint16_t Tick_Time)
{
    if(Tick_Time <= ONE_SECOND_MS)
    {
        NVIC_ST_RELOAD_R = Tick_Time * (SYSTEM_CLOCK / ONE_SECOND_MS);
        NVIC_ST_CURRENT_R = 0;
    }
    else
    {
        NVIC_ST_RELOAD_R = SYSTEM_CLOCK;
        NVIC_ST_CURRENT_R = 0;
    }

    NVIC_ST_CTRL_R |= (1 << 2) | (1 << 1) | (1 << 0);
}
/*********************************************************************************************/
void SysTick_SetCallBack(void (*Ptr2Func)(void))
{
    g_SysTick_Call_Back_Ptr = Ptr2Func;
}
/*********************************************************************************************/
/* Interrupt Service Routine for Timer0 compare mode */
volatile void SysTick_ISR(void)
{
    /* Check if the Timer0_setCallBack is already called */
    if(g_SysTick_Call_Back_Ptr != NULL)
    {
        (*g_SysTick_Call_Back_Ptr)(); //call the function in the scheduler using call-back concept
    }
}
/*********************************************************************************************/
