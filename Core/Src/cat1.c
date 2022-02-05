#include "cat1.h"
#include "usart.h"
#include <string.h>

char CAT1_Reset(int timeout)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);
    HAL_Delay(500);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);

    while (timeout--)
    {
        /* code */
        HAL_Delay(1000);
        if (strstr(u3_rxbuffer, "WH-GM5"))
            break;
        printf("%d\t\n", timeout);
    }
    printf("÷ÿ∆Ù≥…π¶\r\n");
    memset(u3_rxbuffer,0x00,sizeof(u3_rxbuffer));
    if (timeout <= 0)
        return 1; 
    return 0;    
}