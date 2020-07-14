#ifndef ADC_H_
#define ADC_H_

#include "ADC_Cfg.h"
#include <stdint.h>

typedef void(*ADC_FnPtrType)(void);

typedef struct
{
    uint8_t ADC_Num;         /* ADC module number 0 or 1 */
    uint8_t SS_Num;          /* Sample sequencer number 0 --> 3 */
    uint8_t SSpriority;      /* Sample sequencer priority, 0x0 is highest, 0x3 is lowest */

    uint8_t triggerEvent;    /*  0x0          processor(default), by setting SSn bit in ADCPSSI
                              *  0x1 , 0x2    Analog Comparators 0 & 1
                              *  0x4          GPIO
                              *  0x5          Timer
                              *  0x6 , 0x7    PWM Generators 0 & 1
                              *  0x8 , 0x9    PWM Generators 2 & 3
                              *  0xF          Continuous sampling
                              */

    uint8_t triggerSS;       /* Trigger source select 0x0 for PWM0 & 0x1 for PWM1 */

    uint8_t analogInput;     /* Select input pin 0 --> 11  p.801 */
    uint8_t sampleNum;       /* Sample number in the sample sequencer Mux */

    uint8_t end;             /* Select wether the sample is the end sample */
    uint8_t intEnable;       /* Enable interrupt for sample */
    uint8_t diff;            /* Enable differential sampling */


    /* Digital Comparator */

    uint8_t comparatorNum;   /* Comparatoe number 0 --> 7 */
    uint8_t comp0;           /* Lower compare value */
    uint8_t comp1;           /* Upper compare value */
    uint8_t sendToComp;      /* 0x00: send to fifo, 0x01: send to comp */

    uint8_t CTE;             /* Comparator trigger enable for PWM */
    uint8_t CTC;             /* Comparator trigger control: 0x0->Low band , 0x1->Mid band , 0x3->High band */
    uint8_t CTM;             /* Comparator trigger mode: 0x0->Always , 0x1->Once , 0x2->Hyst. always , 0x3-> Hyst. once */

    uint8_t CIE;             /* Comparator interrupt enable */
    uint8_t CIC;             /* Comparator interrupt control: 0x0->Low band , 0x1->Mid band , 0x3->High band */
    uint8_t CIM;             /* Comparator interrupt mode: 0x0->Always , 0x1->Once , 0x2->Hyst. always , 0x3-> Hyst. once */

    ADC_FnPtrType CbkFnPtr;

}ADC_CfgType;

typedef enum {ADC_OK = 0, ADC_NOK} ADC_CheckType;

#define BLOCKING        (uint8_t) 0x01
#define NON_BLOCKING    (uint8_t) 0x00

extern const ADC_CfgType ADC_ConfigParam [ADC_GROUPS_NUMBER];

/* Functions prototype */

/* A function to initialize ADC channels */
ADC_CheckType ADC_Init(void);

/* A function to enable clock for ADC module */
ADC_CheckType ADC_Clk(uint8_t moduleId);

/* A function to read from FIFO */
ADC_CheckType ADC_Read(uint8_t groupId , uint16_t *Data , uint8_t Blocking);


#endif /* ADC_H_ */
