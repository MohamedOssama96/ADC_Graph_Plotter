#include "GPIO.h"
#include "GPIO_Cfg.h"
const GPIO_CfgType GPIO_ConfigParam [GPIO_GROUPS_NUMBER] =
{
#if 0
    {
        5 , 1 << 0 ,
        0 , 0 , 0 ,
        0 ,
        0 , 0xff , 0 ,
        0 , 0 , 0 ,
        0xff ,
        0
    }
    ,
    {
        5 , 1 << 4 ,
        0 , 0 , 0 ,
        0 ,
        0 , 0xff , 0 ,
        0 , 0 , 0 ,
        0xff ,
        0
    }
    ,
    {
        5 , 1 << 2 ,
        0 , 0 , 0 ,
        0xff ,
        0 , 0 , 0 ,
        0xff , 0 , 0 ,
        0xff ,
        0
    }
    ,
    {
        5 , 1 << 1 ,
        0 , 0 , 0 ,
        0xff ,
        0 , 0 , 0 ,
        0xff , 0 , 0 ,
        0xff ,
        0
    }
    ,
    {
        2 , 1 << 4 ,
        0xff , 0 , 0 ,
        0xff ,
        0xff , 0 , 0 ,
        0 , 0 , 0 ,
        0xff ,
        0
    }
    ,
    {
        1 , 1 << 4 ,
        0xff , 0 , 0 ,
        0 ,
        0 , 0 , 0 ,
        0 , 0 , 0 ,
        0xff ,
        0
    }
    ,
    {
        1 , 1 << 5 ,
        0xff , 0 , 0 ,
        0xff ,
        0 , 0 , 0 ,
        0 , 0 , 0 ,
        0xff ,
        0
    }
#endif

    {
        0 , 1 << 1 ,
        0xff , 0 , 0 ,
        0xff ,
        0 , 0 , 0 ,
        0 , 0 , 0 ,
        0xff ,
        0
    }
    ,
    {
        4 , 1 << 1 ,
        0xff , 0 , 0 ,
        0 ,
        0 , 0 , 0 ,
        0 , 0 , 0 ,
        0 ,
        0
    }
};
