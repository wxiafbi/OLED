#include "cat1.h"
#include "usart.h"
#include <string.h>
#include "oled.h"
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
    // uint8_t k[] = {'+', '+', '+'};
    //  CAT1_RxCounter=0;                         //接收数据量变量清零
    u3_count = 0;
    memset(u3_rxbuffer, 0, u3_rxsize); //清空接收缓冲区
    // CAT1_printf("+++");                //发送指令
    HAL_UART_Transmit(&huart3, (uint8_t *)"+++", 3, 0xfff);
    HAL_UART_Transmit(&huart1, (uint8_t *)"+++\r\n", 3, 0xfff);
    while (timeout--)
    {                                 //等待超时时间到0
        HAL_Delay(100);               //延时100ms
        if (strstr(u3_rxbuffer, "a")) //如果接收到a表示成功
        {
            HAL_UART_Transmit(&huart1, (uint8_t *)"收到a\r\n", sizeof("收到a\r\n"), 0xfff);
            break; //主动跳出while循环
        }
        printf("%d ", timeout); //串口输出现在的超时时间
    }
    printf("111111\r\n"); //串口输出信息
    if (timeout <= 0)
    {
        HAL_UART_Transmit(&huart1, (uint8_t *)"没有收到a\r\n", 8, 0xfff);
        return 1; //如果timeout<=0，说明超时时间到了，也没能收到a，返回1
    }
    else
    {
        // CAT1_RxCounter = 0;                       //接收数据量变量清零
        HAL_UART_Transmit(&huart1, (uint8_t *)"进入else分支\r\n", 12, 0xfff);
        u3_count = 0;
        memset(u3_rxbuffer, 0, u3_rxsize); //清空接收缓冲区
        // CAT1_printf("a");                         //发送指令
        HAL_UART_Transmit(&huart3, "a", 1, 0xfff);
        while (timeout--)
        {                                   //等待超时时间到0
            HAL_Delay(100);                 //延时100ms
            if (strstr(u3_rxbuffer, "+ok")) //如果接收到+ok表示成功
            {
                HAL_UART_Transmit(&huart1, (uint8_t *)"收到+ok\r\n", 12, 0xfff);
                break; //主动跳出while循环
            }
            printf("%d ", timeout); //串口输出现在的超时时间
        }
        printf("\r\n"); //串口输出信息
        if (timeout <= 0)
            return 2; //如果timeout<=0，说明超时时间到了，也没能收到+ok，返回1
    }
    return 0; //正确 返回0
}
/*-------------------------------------------------*/
/*函数名：4G模块发送设置指令                       */
/*参  数：cmd：指令                                */
/*参  数：timeout： 超时时间（100ms的倍数）        */
/*返回值：无                                       */
/*-------------------------------------------------*/
char CAT1_SendCmd(char *cmd, int timeout)
{
    u3_count = 0;                      // CAT1接收数据量变量清零
    memset(u3_rxbuffer, 0, u3_rxsize); //清空CAT1接收缓冲区
    // CAT1_printf("%s\r\n",cmd);                  //发送指令
    HAL_UART_Transmit(&huart3, (uint8_t *)cmd, sizeof(cmd), 0xfff);
    while (timeout--)
    {                                  //等待超时时间到0
        HAL_Delay(100);                //延时100ms
        if (strstr(u3_rxbuffer, "OK")) //如果接收到OK表示指令成功
            break;                     //主动跳出while循环
        printf("%d ", timeout);        //串口输出现在的超时时间
    }
    printf("\r\n"); //串口输出信息
    if (timeout <= 0)
        return 1; //如果timeout<=0，说明超时时间到了，也没能收到OK，返回1
    else
        return 0; //反之，表示正确，说明收到OK，通过break主动跳出while
}
/*-------------------------------------------------*/
/*函数名：4G模块信号检测                           */
/*参  数：timeout： 超时时间（100ms的倍数）        */
/*返回值：无                                       */
/*-------------------------------------------------*/
char CAT1_CSQ(int timeout)
{
    char temp[20] = {0};
    char csq[20] = {0};
    u3_count = 0;                      // CAT1接收数据量变量清零
    memset(u3_rxbuffer, 0, u3_rxsize); //清空CAT1接收缓冲区
    // CAT1_printf("usr.cn#AT+CSQ\r\n");  //发送指令
    HAL_UART_Transmit(&huart3, (uint8_t *)"usr.cn#AT+CSQ\r\n", sizeof("usr.cn#AT+CSQ\r\n"), 0xfff);
    // CAT1_SendCmd("AT+CSQ\r\n",timeout);
    while (timeout--)
    {                                 //等待超时时间到0
        HAL_Delay(100);               //延时100ms
        if (strstr(u3_rxbuffer, ",")) //如果接收到,表示指令成功
            break;                    //主动跳出while循环
        printf("%d ", timeout);       //串口输出现在的超时时间
                                      // Delay_Ms(100);
    }
    printf("\r\n"); //串口输出信息
    if (timeout <= 0)
        return 1; //如果timeout<=0，说明超时时间到了，也没能收到,返回1
    else
    {
        sscanf(u3_rxbuffer, "%[^:]:%[^,],%[^,],%[^\r]", temp, csq, temp, temp); //拆分数据
        printf("信号强度:%s\r\n", csq);                                         //串口显示信息
        // OLED_Clear();
        OLED_ShowString(0, 42, (uint8_t *)csq);
    }
    return 0; //正确 返回0
}
/*-------------------------------------------------*/
/*函数名：4G模块查询网络信息                       */
/*参  数：timeout： 超时时间（100ms的倍数）        */
/*返回值：无                                       */
/*-------------------------------------------------*/
char CAT1_SYSINFO(int timeout)
{
    char temp[20];
    char info[20];

    u3_count = 0;                      // CAT1接收数据量变量清零
    memset(u3_rxbuffer, 0, u3_rxsize); //清空CAT1接收缓冲区
    printf("AT+SYSINFO\r\n");          //发送指令
    while (timeout--)
    {                                 //等待超时时间到0
        HAL_Delay(100);                //延时100ms
        if (strstr(u3_rxbuffer, ",")) //如果接收到,表示指令成功
            break;                    //主动跳出while循环
        printf("%d ", timeout);    //串口输出现在的超时时间
    }
    printf("\r\n"); //串口输出信息
    if (timeout <= 0)
        return 1; //如果timeout<=0，说明超时时间到了，也没能收到,返回1
    else
    {
        sscanf(u3_rxbuffer, "%[^:]:%[^,],%[^\r]", temp, temp, info); //拆分数据
        printf("当前网络形式:%s\r\n", info);                         //串口显示信息
    }
    return 0; //正确 返回0
}