#include "UART.h"
#include "UART_Cfg.h"
#include "M4MemMap.h"
#include <stdint.h>
typedef volatile uint32_t* const UART_RegAddType;
#define UARTS_NUMBER 8u

#define SYSTEM_CLOCK    16000000U

#define UART0_BASE_ADDRESS 0x4000C000
#define UART_BASE_ADDRESS(CHANNEL_ID) (UART0_BASE_ADDRESS + (0x1000 * CHANNEL_ID))
#define UART_REG_ADDRESS(UART_ID , REG_OFFSET) (UART_BASE_ADDRESS(UART_ID) + REG_OFFSET)

#define UARTDR_REG(UART_ID)                         *((UART_RegAddType)UART_REG_ADDRESS(UART_ID,0x000))
#define UARTFR_REG(UART_ID)                         *((UART_RegAddType)UART_REG_ADDRESS(UART_ID,0x018))
#define UARTIBRD_REG(UART_ID)                       *((UART_RegAddType)UART_REG_ADDRESS(UART_ID,0x024))
#define UARTFBRD_REG(UART_ID)                       *((UART_RegAddType)UART_REG_ADDRESS(UART_ID,0x028))
#define UARTLCRH_REG(UART_ID)                       *((UART_RegAddType)UART_REG_ADDRESS(UART_ID,0x02C))
#define UARTCTL_REG(UART_ID)                        *((UART_RegAddType)UART_REG_ADDRESS(UART_ID,0x030))
#define UARTCC_REG(UART_ID)                         *((UART_RegAddType)UART_REG_ADDRESS(UART_ID,0xFC8))

static uint8_t UART_GroupState[UARTS_NUMBER] = {0};

/*A function to initialize all the UARTs in the configurations*/
UART_CheckType UART_Init(void)
{
    uint8_t LoopIndex;
    uint8_t ErrorFlag = 0;
    uint16_t IBRD = 0;
    uint8_t FBRD = 0;
    UART_CheckType RetVar;
    const UART_CfgType * CfgPtr;

    for(LoopIndex = 0 ; (LoopIndex < UART_GROUPS_NUMBER) && (ErrorFlag == 0) ; LoopIndex ++)
    {
        if(UART_ConfigParam[LoopIndex].UART_Id < UARTS_NUMBER)
        {
            /*Enable UART clock gate*/
            CfgPtr = & UART_ConfigParam[LoopIndex];
            RCGCUART_REG |= 1 << CfgPtr->UART_Id;

            UARTCTL_REG(CfgPtr->UART_Id) &= ~(1 << 0);

            IBRD = SYSTEM_CLOCK / (CfgPtr->CLKDiv * CfgPtr->BaudRate);
            FBRD = ((SYSTEM_CLOCK / (float) (CfgPtr->CLKDiv * CfgPtr->BaudRate)) - IBRD) * 64.0 + 0.5;

            UARTIBRD_REG(CfgPtr->UART_Id) = IBRD;
            UARTFBRD_REG(CfgPtr->UART_Id) = FBRD;
            UARTLCRH_REG(CfgPtr->UART_Id) = CfgPtr->LineControl;
            UARTCC_REG(CfgPtr->UART_Id) = CfgPtr->Clock;

            UARTCTL_REG(CfgPtr->UART_Id) |= 1 << 0;

            UART_GroupState[LoopIndex] = 1;
            RetVar = UART_OK;
        }
        else
        {
            /*Invalid GroupId*/
            RetVar = UART_NOK;
            ErrorFlag = 1;
        }
    }

    return RetVar;
}


/*A function to transmit data from a specific UART*/
UART_CheckType UART_Transmission(uint8_t GroupId , uint8_t GroupData)
{
    const UART_CfgType * CfgTrans;
    UART_CheckType RetVar;

    if (( GroupId <UART_GROUPS_NUMBER))
    {
        CfgTrans =  & UART_ConfigParam[GroupId];

        if (UART_GroupState[GroupId] == 1)
        {
            if ((UARTFR_REG(CfgTrans->UART_Id) & 0x80) != 0x00)
            {
                UARTDR_REG(CfgTrans->UART_Id) = GroupData;
                RetVar = UART_OK ;

            }else {RetVar = UART_NOK ; }

        }else {RetVar = UART_NOK ; }

    }else {RetVar = UART_NOK ; }

    return RetVar;
}


/*A function to receive data from a specific UART*/
UART_CheckType UART_Reception(uint8_t GroupId , uint8_t * GroupDataPtr)
{
    const UART_CfgType * CfgTrans;
    UART_CheckType RetVar;

    if (( GroupId <UART_GROUPS_NUMBER))
    {
        CfgTrans =  & UART_ConfigParam[GroupId];

        if (UART_GroupState[GroupId] == 1)
        {
            if ((UARTFR_REG(CfgTrans->UART_Id) & 0x40) == 0x40)
            {
                * GroupDataPtr = UARTDR_REG(CfgTrans->UART_Id);
                RetVar = UART_OK ;

            }else {RetVar = UART_NOK ; }

        }else {RetVar = UART_NOK ; }

    }else {RetVar = UART_NOK ; }

    return RetVar;
}
