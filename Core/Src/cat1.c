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
    printf("�����ɹ�\r\n");
    if (timeout <= 0)
    {
        printf("����ʧ��\r\n");
        return 1;
    }
    return 0;
}