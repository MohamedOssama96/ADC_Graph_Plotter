/*
 * App.c
 *
 *  Created on: Feb 19, 2020
 *      Author: Mohamed Ossama
 */

#include "GPIO.h"
#include "ADC.h"
#include "UART.h"

/* Task Called Once to initialize all the Modules */
void Init_Task(void)
{
    /* Initialize GPIO Pins */
    GPIO_Init();

    GPIO_SetAlternFuntion(0 , 1);

    ADC_Init();
    UART_Init();

    uint16_t PotValue_12Bit = 0;
    uint8_t PotValue_8Bit = 0;


    ADC_Read(0 , &PotValue_12Bit , BLOCKING);

    PotValue_8Bit = uLawCompanding(PotValue_12Bit);

    UART_Transmission(0 , PotValue_8Bit);
}


static uint8_t uLawCompanding(uint16_t Data)
{
    if((Data & 0x07FF) <= 15)
    {
        return ((Data & 0x0800) >> 4) | (Data & 0x0F);
    }
    else if((Data & 0x07FF) <= 31)
    {
        return ((Data & 0x0800) >> 4) | (1 << 4) | (Data & 0x0F);
    }
    else if((Data & 0x07FF) <= 63)
    {
        return ((Data & 0x0800) >> 4) | (2 << 4) | (Data & 0x0F);
    }
    else if((Data & 0x07FF) <= 127)
    {
        return ((Data & 0x0800) >> 4) | (3 << 4) | (Data & 0x0F);
    }
    else if((Data & 0x07FF) <= 255)
    {
        return ((Data & 0x0800) >> 4) | (4 << 4) | (Data & 0x0F);
    }
    else if((Data & 0x07FF) <= 511)
    {
        return ((Data & 0x0800) >> 4) | (5 << 4) | (Data & 0x0F);
    }
    else if((Data & 0x07FF) <= 1023)
    {
        return ((Data & 0x0800) >> 4) | (6 << 4) | (Data & 0x0F);
    }
    else if((Data & 0x07FF) <= 2047)
    {
        return ((Data & 0x0800) >> 4) | (7 << 4) | (Data & 0x0F);
    }

    return 0;
}


/* Task called every 10 Mili-seconds */
void App_Task(void)
{
    uint16_t PotValue_12Bit = 0;
    uint8_t PotValue_8Bit = 0;


    ADC_Read(0 , &PotValue_12Bit , BLOCKING);

    PotValue_8Bit = uLawCompanding(PotValue_12Bit);

    UART_Transmission(0 , PotValue_8Bit);
}
