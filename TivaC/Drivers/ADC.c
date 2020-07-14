#include "ADC.h"
#include "ADC_Cfg.h"
#include "M4MemMap.h"
#include <stdint.h>

typedef volatile uint32_t* const ADC_RegAddType;
#define CHANNELS_NUMBER 2u

/* Register memory map */
#define ADC0_BASE_ADDRESS       0x40038000
#define ADC1_BASE_ADDRESS       0x40039000

static const uint32_t ChannelsBaseAddressLut[CHANNELS_NUMBER] =
{
    ADC0_BASE_ADDRESS,
    ADC1_BASE_ADDRESS
};

#define ADC_REG_ADDRESS(CHANNEL_ID,REG_OFFSET)  (ChannelsBaseAddressLut[CHANNEL_ID] + REG_OFFSET)

/* Registers Addresses */
#define ADCACTSS_REG(CHANNEL_ID)                     *((ADC_RegAddType)ADC_REG_ADDRESS(CHANNEL_ID,0x000))
#define ADCRIS_REG(CHANNEL_ID)                       *((ADC_RegAddType)ADC_REG_ADDRESS(CHANNEL_ID,0x004))
#define ADCIM_REG(CHANNEL_ID)                        *((ADC_RegAddType)ADC_REG_ADDRESS(CHANNEL_ID,0x008))
#define ADCISC_REG(CHANNEL_ID)                       *((ADC_RegAddType)ADC_REG_ADDRESS(CHANNEL_ID,0x00C))
#define ADCOSTAT_REG(CHANNEL_ID)                     *((ADC_RegAddType)ADC_REG_ADDRESS(CHANNEL_ID,0x010))
#define ADCEMUX_REG(CHANNEL_ID)                      *((ADC_RegAddType)ADC_REG_ADDRESS(CHANNEL_ID,0x014))
#define ADCUSTAT_REG(CHANNEL_ID)                     *((ADC_RegAddType)ADC_REG_ADDRESS(CHANNEL_ID,0x018))
#define ADCTSSEL_REG(CHANNEL_ID)                     *((ADC_RegAddType)ADC_REG_ADDRESS(CHANNEL_ID,0x01C))
#define ADCSSPRI_REG(CHANNEL_ID)                     *((ADC_RegAddType)ADC_REG_ADDRESS(CHANNEL_ID,0x020))
#define ADCPSSI_REG(CHANNEL_ID)                      *((ADC_RegAddType)ADC_REG_ADDRESS(CHANNEL_ID,0x024))
#define ADCDCISC_REG(CHANNEL_ID)                     *((ADC_RegAddType)ADC_REG_ADDRESS(CHANNEL_ID,0x028))

#define ADCSSMUX_REG(CHANNEL_ID,SEQUENCER)           *((ADC_RegAddType)ADC_REG_ADDRESS(CHANNEL_ID,(0x040+(SEQUENCER*0x020))))
#define ADCSSCTL_REG(CHANNEL_ID,SEQUENCER)           *((ADC_RegAddType)ADC_REG_ADDRESS(CHANNEL_ID,(0x044+(SEQUENCER*0x020))))
#define ADCSSFIFO_REG(CHANNEL_ID,SEQUENCER)          *((ADC_RegAddType)ADC_REG_ADDRESS(CHANNEL_ID,(0x048+(SEQUENCER*0x020))))
#define ADCSSFSTAT_REG(CHANNEL_ID,SEQUENCER)         *((ADC_RegAddType)ADC_REG_ADDRESS(CHANNEL_ID,(0x04C+(SEQUENCER*0x020))))
#define ADCSSOP_REG(CHANNEL_ID,SEQUENCER)            *((ADC_RegAddType)ADC_REG_ADDRESS(CHANNEL_ID,(0x050+(SEQUENCER*0x020))))
#define ADCSSDC_REG(CHANNEL_ID,SEQUENCER)            *((ADC_RegAddType)ADC_REG_ADDRESS(CHANNEL_ID,(0x054+(SEQUENCER*0x020))))

#define ADCDCRIC_REG(CHANNEL_ID)                     *((ADC_RegAddType)ADC_REG_ADDRESS(CHANNEL_ID,0xD00))

#define ADCDCCTL_REG(CHANNEL_ID,COMPARATOR)          *((ADC_RegAddType)ADC_REG_ADDRESS(CHANNEL_ID,(0xE00+(COMPARATOR*0x04))))
#define ADCDCCMP_REG(CHANNEL_ID,COMPARATOR)          *((ADC_RegAddType)ADC_REG_ADDRESS(CHANNEL_ID,(0xE40+(COMPARATOR*0x04))))

static uint8_t ADC_GroupState[ADC_GROUPS_NUMBER] = {0};


/*****************************************************************************************************
 *************************************  Functions Dfinition  *****************************************
 *****************************************************************************************************/

ADC_CheckType ADC_Init(void)
{
    uint8_t LoopIndex;
    uint8_t ErrorFlag = 0;
    ADC_CheckType RetVar;
    const ADC_CfgType * CfgPtr;

    for(LoopIndex = 0; (LoopIndex < ADC_GROUPS_NUMBER) && (ErrorFlag == 0); LoopIndex ++)
    {
        if(ADC_ConfigParam[LoopIndex].ADC_Num < CHANNELS_NUMBER)
        {
            CfgPtr = & ADC_ConfigParam[LoopIndex];

            /* Enabling ADC module */
            ADC_Clk(CfgPtr->ADC_Num);

            /* Setting sample sequencer priority */
            ADCSSPRI_REG(CfgPtr->ADC_Num) |= (CfgPtr->SSpriority << ((CfgPtr->SS_Num) * 4));

            /* Disabling sample sequencer */
            ADCACTSS_REG(CfgPtr->ADC_Num) &=~ (1 << CfgPtr->SS_Num);

            /* Trigger event */
            ADCEMUX_REG(CfgPtr->ADC_Num) |= (CfgPtr->triggerEvent << ((CfgPtr->SS_Num) * 4));
            /* Trigger source select in case of PWM trigger event */
            if((CfgPtr->triggerEvent > 0x5) && (CfgPtr->triggerEvent < 0xA))
            {
                switch(CfgPtr->triggerEvent)
                {
                    case 0x6 : ADCTSSEL_REG(CfgPtr->ADC_Num) |= CfgPtr->triggerSS << 4;
                               break;
                    case 0x7 : ADCTSSEL_REG(CfgPtr->ADC_Num) |= CfgPtr->triggerSS << 12;
                               break;
                    case 0x8 : ADCTSSEL_REG(CfgPtr->ADC_Num) |= CfgPtr->triggerSS << 20;
                               break;
                    case 0x9 : ADCTSSEL_REG(CfgPtr->ADC_Num) |= CfgPtr->triggerSS << 28;
                               break;
                }
            }

            /* Input for sample sequencer */
            ADCSSMUX_REG(CfgPtr->ADC_Num,CfgPtr->SS_Num) |= (CfgPtr->analogInput) << (CfgPtr->sampleNum);
            /* Configuring sample control bits */
            ADCSSCTL_REG(CfgPtr->ADC_Num,CfgPtr->SS_Num) |= (CfgPtr->diff) << (CfgPtr->sampleNum *4)
                                                           |(CfgPtr->end) << ((CfgPtr->sampleNum *4) + 1)
                                                           |(CfgPtr->intEnable) << ((CfgPtr->sampleNum *4) + 2);
            /* Choosing digital comparator or sample sequencer */
            ADCSSOP_REG(CfgPtr->ADC_Num,CfgPtr->SS_Num) |= (CfgPtr->sendToComp) << (CfgPtr->sampleNum * 4);
            /* Assigning digital comparator for the signal */
            ADCSSDC_REG(CfgPtr->ADC_Num,CfgPtr->SS_Num) |= (CfgPtr->comparatorNum) << (CfgPtr->sampleNum * 4);
            /* Controling digital comparator */
            ADCDCCTL_REG(CfgPtr->ADC_Num,CfgPtr->comparatorNum) |= (CfgPtr->CTE << 12)
                                                                  |(CfgPtr->CTC << 10)
                                                                  |(CfgPtr->CTM << 8)
                                                                  |(CfgPtr->CIE << 4)
                                                                  |(CfgPtr->CIC << 2)
                                                                  |(CfgPtr->CIM);

            /* Setting compare values */
            ADCDCCMP_REG(CfgPtr->ADC_Num,CfgPtr->comparatorNum) |= (CfgPtr->comp0)
                                                                  |(CfgPtr->comp1) << 16;

            /* Enabling sample sequencer */
            ADCACTSS_REG(CfgPtr->ADC_Num) |= (1 << CfgPtr->SS_Num);

            ADC_GroupState[LoopIndex] = 1;
            RetVar = ADC_OK;

        }
        else
        {
            /* Invalid Module ID */
            RetVar = ADC_NOK;
            ErrorFlag = 1;
        }
    }

    return RetVar;
}

/***********************************************************************************************************/

ADC_CheckType ADC_Clk(uint8_t moduleId)
{
    ADC_CheckType RetVar = ADC_NOK;

    if(moduleId < CHANNELS_NUMBER)
    {
        RCGCADC_REG |= 1 << moduleId;
        RetVar = ADC_OK;
    }
    return RetVar;
}

/***********************************************************************************************************/

ADC_CheckType ADC_Read(uint8_t groupId , uint16_t *Data , uint8_t Blocking)
{
    ADC_CheckType RetVar;
    const ADC_CfgType * CfgRead;

    if(groupId < ADC_GROUPS_NUMBER)
    {
        if(ADC_GroupState[groupId] == 1)
        {
            CfgRead = & ADC_ConfigParam[groupId];

            if(Blocking == BLOCKING)
            {
                while((ADCSSFSTAT_REG(CfgRead->ADC_Num,CfgRead->SS_Num) & (1 << 8)) != 0);
                *Data = ADCSSFIFO_REG(CfgRead->ADC_Num,CfgRead->SS_Num);
                RetVar = ADC_OK;
            }
            else if(Blocking == NON_BLOCKING)
            {
                if((ADCSSFSTAT_REG(CfgRead->ADC_Num,CfgRead->SS_Num) & (1 << 8)) == 0)
                {
                    *Data = ADCSSFIFO_REG(CfgRead->ADC_Num,CfgRead->SS_Num);
                    RetVar = ADC_OK;
                }
            }
            else
            {
                RetVar = ADC_NOK;
            }
        }
        else
        {
            RetVar = ADC_NOK;
        }
    }
    else
    {
        RetVar = ADC_NOK;
    }
}

/***********************************************************************************************************/


