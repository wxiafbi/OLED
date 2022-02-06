#include "cat1.h"
#include "usart.h"
#include <string.h>

/*-------------------------------------------------*/
/*函数名：复位4G模块                               */
/*参  数：timeout： 超时时间（1s的倍数）           */
/*返回值：无                                       */
/*-------------------------------------------------*/
char CAT1_Reset(int timeout)
{

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);
    HAL_Delay(600);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);
    // printf("接受第1%c\r\n",u3_rxbuffer[0]);
    // printf("接受第2%c\r\n",u3_rxbuffer[1]);
    // printf("接受第3%c\r\n",u3_rxbuffer[2]);
    // printf("接受第4%c\r\n",u3_rxbuffer[3]);
    // printf("接受第5%c\r\n",u3_rxbuffer[4]);
    while (timeout--)
    {
        /* code */
        HAL_Delay(1000);
        if (strstr(u3_rxbuffer, "WH-GM5"))
        {
            printf("重启成功\r\n");
            break;
        }
        printf("%d\t\n", timeout);
    }
    printf("\r\n");
    if (timeout <= 0)
    {
        printf("重启失败\r\n");
        return 1;
    }
    return 0;
}
/*-------------------------------------------------*/
/*函数名：4G模块退出透传                           */
/*参  数：timeout： 超时时间（100ms的倍数）        */
/*返回值：无                                       */
/*-------------------------------------------------*/
char CAT1_ExitTrans(int timeout)
{
    //uint8_t k[] = {'+', '+', '+'};
    // CAT1_RxCounter=0;                         //接收数据量变量清零
    u3_count = 0;
    memset(u3_rxbuffer, 0, u3_rxsize); //清空接收缓冲区
    // CAT1_printf("+++");                //发送指令
    HAL_UART_Transmit(&huart3, (uint8_t *)"+++", 3, 0xfff);
    while (timeout--)
    {                                 //等待超时时间到0
        HAL_Delay(100);               //延时100ms
        if (strstr(u3_rxbuffer, "a")) //如果接收到a表示成功
            break;                    //主动跳出while循环
        printf("%d ", timeout);       //串口输出现在的超时时间
    }
    printf("\r\n"); //串口输出信息
    if (timeout <= 0)
        return 1; //如果timeout<=0，说明超时时间到了，也没能收到a，返回1
    else
    {
        // CAT1_RxCounter = 0;                       //接收数据量变量清零
        u3_count = 0;
        memset(u3_rxbuffer, 0, u3_rxsize); //清空接收缓冲区
        // CAT1_printf("a");                         //发送指令
        HAL_UART_Transmit(&huart3, "a", 2, 0xfff);
        while (timeout--)
        {                                   //等待超时时间到0
            HAL_Delay(100);                 //延时100ms
            if (strstr(u3_rxbuffer, "+ok")) //如果接收到+ok表示成功
                break;                      //主动跳出while循环
            printf("%d ", timeout);         //串口输出现在的超时时间
        }
        printf("\r\n"); //串口输出信息
        if (timeout <= 0)
            return 2; //如果timeout<=0，说明超时时间到了，也没能收到+ok，返回1
    }
    return 0; //正确 返回0
}