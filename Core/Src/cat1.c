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
    memset(u3_rxbuffer, 0x00, u3_rxsize); //清空接收缓冲区
    // CAT1_printf("+++");                //发送指令
    HAL_UART_Transmit(&huart3, (uint8_t *)"+++", 3, 0xfff);
    HAL_UART_Transmit(&huart1, (uint8_t *)"+++\r\n", sizeof("+++"), 0xfff);
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
    // printf("111111\r\n"); //串口输出信息
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
        memset(u3_rxbuffer, 0x00, u3_rxsize); //清空接收缓冲区
        // CAT1_printf("a");                         //发送指令
        HAL_UART_Transmit(&huart3, (uint8_t *)"a", 1, 0xfff);
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
    printf("退出透传模式成功\r\n");
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
    u3_count = 0;                         // CAT1接收数据量变量清零
    memset(u3_rxbuffer, 0x00, u3_rxsize); //清空CAT1接收缓冲区
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
    char temp[20];
    char csq[20];

    printf("进入查询信号强度\r\n");
    u3_count = 0;                         // CAT1接收数据量变量清零
    memset(u3_rxbuffer, 0x00, u3_rxsize); //清空CAT1接收缓冲区
    memset(temp, 0x00, 20);               //清空CAT1接收缓冲区
    // CAT1_printf("usr.cn#AT+CSQ\r\n");  //发送指令
    HAL_UART_Transmit(&huart3, (uint8_t *)"AT+CSQ\r\n", 8, 0xfff);
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
        OLED_ShowString(81, 0, (uint8_t *)csq);
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

    u3_count = 0;                         // CAT1接收数据量变量清零
    memset(u3_rxbuffer, 0x00, u3_rxsize); //清空CAT1接收缓冲区
    // printf("AT+SYSINFO\r\n");          //发送指令
    HAL_UART_Transmit(&huart3, (uint8_t *)"AT+SYSINFO\r\n", 12, 0xfff);
    while (timeout--)
    {                                 //等待超时时间到0
        HAL_Delay(100);               //延时100ms
        if (strstr(u3_rxbuffer, ",")) //如果接收到,表示指令成功
            break;                    //主动跳出while循环
        printf("%d ", timeout);       //串口输出现在的超时时间
    }
    printf("\r\n"); //串口输出信息
    if (timeout <= 0)
        return 1; //如果timeout<=0，说明超时时间到了，也没能收到,返回1
    else
    {
        sscanf(u3_rxbuffer, "%[^:]:%[^,],%[^\r]", temp, temp, info); //拆分数据
        for (size_t i = 0; i <100; i++)
        {
            /* code */
            printf("%c",u3_rxbuffer[i]);
        }
        
        printf("当前网络形式:%s\r\n", info);                         //串口显示信息
    }
    return 0; //正确 返回0
}
char CAT1_LBSN(int timeout)
{
    char temp[20];
    char info[25];

    u3_count = 0;                         // CAT1接收数据量变量清零
    memset(u3_rxbuffer, 0x00, u3_rxsize); //清空CAT1接收缓冲区
    // printf("AT+SYSINFO\r\n");          //发送指令
    HAL_UART_Transmit(&huart3, (uint8_t *)"AT+SN\r\n", 7, 0xfff);
    while (timeout--)
    {                                 //等待超时时间到0
        HAL_Delay(100);               //延时100ms
        if (strstr(u3_rxbuffer, ":")) //如果接收到,表示指令成功
            break;                    //主动跳出while循环
        printf("%d ", timeout);       //串口输出现在的超时时间
    }
    printf("\r\n"); //串口输出信息
    if (timeout <= 0)
        return 1; //如果timeout<=0，说明超时时间到了，也没能收到,返回1
    else
    {
        sscanf(u3_rxbuffer, "%[^:]:%[^\r]", temp,info); //拆分数据
        for (size_t i = 0; i <100; i++)
        {
            /* code */
            printf("%c",u3_rxbuffer[i]);
        }
        
        printf("当前网络形式:%s\r\n", info);                         //串口显示信息
    }
    return 0; //正确 返回0
}
/*-------------------------------------------------*/
/*函数名：4G模块连接服务器                         */
/*参  数：timeout： 超时时间（100ms的倍数）        */
/*返回值：0：正确   其他：错误                     */
/*-------------------------------------------------*/
// char CAT1_Connect_IoTServer(int timeout)
// {
//     char temp[256];

//     u1_printf("准备复位模块\r\n"); //串口提示数据
//     if (CAT1_Reset(20))
//     {                                        //复位，1s超时单位，总计20s超时时间
//         u1_printf("复位失败，准备重启\r\n"); //返回非0值，进入if，串口提示数据
//         return 1;                            //返回1
//     }
//     else
//         u1_printf("复位成功\r\n"); //串口提示数据

//     u1_printf("准备退出透传\r\n"); //串口提示数据
//     if (CAT1_ExitTrans(50))
//     {                                            //退出透传，100ms超时单位，总计5s超时时间
//         u1_printf("退出透传失败，准备重启\r\n"); //返回非0值，进入if，串口提示数据
//         return 2;                                //返回2
//     }
//     else
//         u1_printf("退出透传成功\r\n"); //串口提示数据

//     u1_printf("准备设置NET模式\r\n"); //串口提示数据
//     if (CAT1_SendCmd("AT+WKMOD=NET", 50))
//     {                                               //设置NET模式，100ms超时单位，总计5s超时时间
//         u1_printf("设置NET模式失败，准备重启\r\n"); //返回非0值，进入if，串口提示数据
//         return 5;                                   //返回5
//     }
//     else
//         u1_printf("设置NET模式成功\r\n"); //串口提示数据

//     u1_printf("准备使能链接A\r\n"); //串口提示数据
//     if (CAT1_SendCmd("AT+SOCKAEN=ON", 50))
//     {                                             //使能链接A，100ms超时单位，总计5s超时时间
//         u1_printf("使能链接A失败，准备重启\r\n"); //返回非0值，进入if，串口提示数据
//         return 6;                                 //返回6
//     }
//     else
//         u1_printf("使能链接A成功\r\n"); //串口提示数据

//     u1_printf("准备关闭链接B\r\n"); //串口提示数据
//     if (CAT1_SendCmd("AT+SOCKBEN=OFF", 50))
//     {                                             //关闭链接B，100ms超时单位，总计5s超时时间
//         u1_printf("关闭链接B失败，准备重启\r\n"); //返回非0值，进入if，串口提示数据
//         return 7;                                 //返回7
//     }
//     else
//         u1_printf("关闭链接B成功\r\n"); //串口提示数据

//     u1_printf("准备设置链接A长连接\r\n"); //串口提示数据
//     if (CAT1_SendCmd("AT+SOCKASL=LONG", 50))
//     {                                                   //设置链接A长连接，100ms超时单位，总计5s超时时间
//         u1_printf("设置链接A长连接失败，准备重启\r\n"); //返回非0值，进入if，串口提示数据
//         return 8;                                       //返回8
//     }
//     else
//         u1_printf("设置链接A长连接成功\r\n"); //串口提示数据

//     u1_printf("准备关闭自带心跳功能\r\n"); //串口提示数据
//     if (CAT1_SendCmd("AT+HEARTEN=OFF", 50))
//     {                                                    //关闭自带心跳功能，100ms超时单位，总计5s超时时间
//         u1_printf("关闭自带心跳功能失败，准备重启\r\n"); //返回非0值，进入if，串口提示数据
//         return 9;                                        //返回9
//     }
//     else
//         u1_printf("关闭自带心跳功能成功\r\n"); //串口提示数据

//     u1_printf("准备设置链接A链接服务器\r\n");                  //串口提示数据
//     sprintf(temp, "AT+SOCKA=TCP,%s,%d", ServerIP, ServerPort); //构建命令
//     if (CAT1_SendCmd(temp, 50))
//     {                                                       //设置链接A链接服务器，100ms超时单位，总计5s超时时间
//         u1_printf("设置链接A链接服务器失败，准备重启\r\n"); //返回非0值，进入if，串口提示数据
//         return 10;                                          //返回10
//     }
//     else
//         u1_printf("设置链接A链接服务器成功\r\n"); //串口提示数据

//     u1_printf("保存设置，然后重启模块\r\n"); //串口提示数据
//     if (CAT1_SendCmd("AT+S", 50))
//     {                                            //保存设置，然后重启模块，100ms超时单位，总计5s超时时间
//         u1_printf("保存设置失败，准备重启\r\n"); //返回非0值，进入if，串口提示数据
//         return 11;                               //返回11
//     }
//     else
//         u1_printf("保存设置成功\r\n"); //串口提示数据

//     u1_printf("等待重启并连接服务器\r\n"); //串口提示数据
//     while (timeout--)
//     {                              //等待超时时间到0
//         Delay_Ms(1000);            //延时1s
//         if (LINKA_STA == 0)        //如LINKA_STA PC1是低电平了，表示连接成功
//             break;                 //跳出while循环
//         u1_printf("%d ", timeout); //串口输出现在的超时时间
//     }
//     u1_printf("\r\n"); //串口输出信息
//     if (timeout <= 0)
//         return 12; //超时错误，返回12

//     Delay_Ms(500); //延时
//     return 0;      //正确返回0
// }
