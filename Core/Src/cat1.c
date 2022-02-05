#include "cat1.h"
#include "usart.h"
#include <string.h>

char CAT1_Reset(int timeout)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);
    HAL_Delay(600);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);

    while (timeout--)
    {
        /* code */
        HAL_Delay(1000);
        if (strstr(u3_rxbuffer, "WH-GM5"))
            break;
        printf("%d\t\n", timeout);
    }
    printf("重启成功\r\n");
    if (timeout <= 0)
    {
        printf("重启失败\r\n");
        return 1;
    }
    return 0;
}