#ifndef UART_H
#define UART_H
#include <stdint.h>
#include "UART_Cfg.h"

typedef void(*UART_FnPtrType)(void);
typedef struct
{
uint8_t UART_Id;
uint32_t BaudRate;
uint8_t CLKDiv;
uint8_t LineControl;
uint8_t Clock;
UART_FnPtrType CbkFnPtr;
}UART_CfgType;
typedef enum {UART_OK = 0 , UART_NOK} UART_CheckType;

extern const UART_CfgType UART_ConfigParam [UART_GROUPS_NUMBER];

/*A function to initialize all the UARTs in the configurations*/
UART_CheckType UART_Init(void);
/*A function to transmit data from a specific UART*/
UART_CheckType UART_Transmission(uint8_t GroupId,uint8_t GroupData);
/*A function to receive data from a specific UART*/
UART_CheckType UART_Reception(uint8_t GroupId,uint8_t* GroupDataPtr);

#endif // UART_H_INCLUDED
