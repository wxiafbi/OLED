#include "cat1.h"
#include "usart.h"
#include <string.h>
#include "oled.h"
/*-------------------------------------------------*/
/*����������λ4Gģ��                               */
/*��  ����timeout�� ��ʱʱ�䣨1s�ı�����           */
/*����ֵ����                                       */
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
            printf("�����ɹ�\r\n");
            break;
        }
        printf("%d\t\n", timeout);
    }
    printf("\r\n");
    if (timeout <= 0)
    {
        printf("����ʧ��\r\n");
        return 1;
    }
    return 0;
}
/*-------------------------------------------------*/
/*��������4Gģ���˳�͸��                           */
/*��  ����timeout�� ��ʱʱ�䣨100ms�ı�����        */
/*����ֵ����                                       */
/*-------------------------------------------------*/
char CAT1_ExitTrans(int timeout)
{
    // uint8_t k[] = {'+', '+', '+'};
    //  CAT1_RxCounter=0;                         //������������������
    u3_count = 0;
    memset(u3_rxbuffer, 0, u3_rxsize); //��ս��ջ�����
    // CAT1_printf("+++");                //����ָ��
    HAL_UART_Transmit(&huart3, (uint8_t *)"+++", 3, 0xfff);
    HAL_UART_Transmit(&huart1, (uint8_t *)"+++\r\n", 3, 0xfff);
    while (timeout--)
    {                                 //�ȴ���ʱʱ�䵽0
        HAL_Delay(100);               //��ʱ100ms
        if (strstr(u3_rxbuffer, "a")) //������յ�a��ʾ�ɹ�
        {
            HAL_UART_Transmit(&huart1, (uint8_t *)"�յ�a\r\n", sizeof("�յ�a\r\n"), 0xfff);
            break; //��������whileѭ��
        }
        printf("%d ", timeout); //����������ڵĳ�ʱʱ��
    }
    printf("111111\r\n"); //���������Ϣ
    if (timeout <= 0)
    {
        HAL_UART_Transmit(&huart1, (uint8_t *)"û���յ�a\r\n", 8, 0xfff);
        return 1; //���timeout<=0��˵����ʱʱ�䵽�ˣ�Ҳû���յ�a������1
    }
    else
    {
        // CAT1_RxCounter = 0;                       //������������������
        HAL_UART_Transmit(&huart1, (uint8_t *)"����else��֧\r\n", 12, 0xfff);
        u3_count = 0;
        memset(u3_rxbuffer, 0, u3_rxsize); //��ս��ջ�����
        // CAT1_printf("a");                         //����ָ��
        HAL_UART_Transmit(&huart3, "a", 1, 0xfff);
        while (timeout--)
        {                                   //�ȴ���ʱʱ�䵽0
            HAL_Delay(100);                 //��ʱ100ms
            if (strstr(u3_rxbuffer, "+ok")) //������յ�+ok��ʾ�ɹ�
            {
                HAL_UART_Transmit(&huart1, (uint8_t *)"�յ�+ok\r\n", 12, 0xfff);
                break; //��������whileѭ��
            }
            printf("%d ", timeout); //����������ڵĳ�ʱʱ��
        }
        printf("\r\n"); //���������Ϣ
        if (timeout <= 0)
            return 2; //���timeout<=0��˵����ʱʱ�䵽�ˣ�Ҳû���յ�+ok������1
    }
    return 0; //��ȷ ����0
}
/*-------------------------------------------------*/
/*��������4Gģ�鷢������ָ��                       */
/*��  ����cmd��ָ��                                */
/*��  ����timeout�� ��ʱʱ�䣨100ms�ı�����        */
/*����ֵ����                                       */
/*-------------------------------------------------*/
char CAT1_SendCmd(char *cmd, int timeout)
{
    u3_count = 0;                      // CAT1������������������
    memset(u3_rxbuffer, 0, u3_rxsize); //���CAT1���ջ�����
    // CAT1_printf("%s\r\n",cmd);                  //����ָ��
    HAL_UART_Transmit(&huart3, (uint8_t *)cmd, sizeof(cmd), 0xfff);
    while (timeout--)
    {                                  //�ȴ���ʱʱ�䵽0
        HAL_Delay(100);                //��ʱ100ms
        if (strstr(u3_rxbuffer, "OK")) //������յ�OK��ʾָ��ɹ�
            break;                     //��������whileѭ��
        printf("%d ", timeout);        //����������ڵĳ�ʱʱ��
    }
    printf("\r\n"); //���������Ϣ
    if (timeout <= 0)
        return 1; //���timeout<=0��˵����ʱʱ�䵽�ˣ�Ҳû���յ�OK������1
    else
        return 0; //��֮����ʾ��ȷ��˵���յ�OK��ͨ��break��������while
}
/*-------------------------------------------------*/
/*��������4Gģ���źż��                           */
/*��  ����timeout�� ��ʱʱ�䣨100ms�ı�����        */
/*����ֵ����                                       */
/*-------------------------------------------------*/
char CAT1_CSQ(int timeout)
{
    char temp[20] = {0};
    char csq[20] = {0};
    u3_count = 0;                      // CAT1������������������
    memset(u3_rxbuffer, 0, u3_rxsize); //���CAT1���ջ�����
    // CAT1_printf("usr.cn#AT+CSQ\r\n");  //����ָ��
    HAL_UART_Transmit(&huart3, (uint8_t *)"usr.cn#AT+CSQ\r\n", sizeof("usr.cn#AT+CSQ\r\n"), 0xfff);
    // CAT1_SendCmd("AT+CSQ\r\n",timeout);
    while (timeout--)
    {                                 //�ȴ���ʱʱ�䵽0
        HAL_Delay(100);               //��ʱ100ms
        if (strstr(u3_rxbuffer, ",")) //������յ�,��ʾָ��ɹ�
            break;                    //��������whileѭ��
        printf("%d ", timeout);       //����������ڵĳ�ʱʱ��
                                      // Delay_Ms(100);
    }
    printf("\r\n"); //���������Ϣ
    if (timeout <= 0)
        return 1; //���timeout<=0��˵����ʱʱ�䵽�ˣ�Ҳû���յ�,����1
    else
    {
        sscanf(u3_rxbuffer, "%[^:]:%[^,],%[^,],%[^\r]", temp, csq, temp, temp); //�������
        printf("�ź�ǿ��:%s\r\n", csq);                                         //������ʾ��Ϣ
        // OLED_Clear();
        OLED_ShowString(0, 42, (uint8_t *)csq);
    }
    return 0; //��ȷ ����0
}
/*-------------------------------------------------*/
/*��������4Gģ���ѯ������Ϣ                       */
/*��  ����timeout�� ��ʱʱ�䣨100ms�ı�����        */
/*����ֵ����                                       */
/*-------------------------------------------------*/
char CAT1_SYSINFO(int timeout)
{
    char temp[20];
    char info[20];

    u3_count = 0;                      // CAT1������������������
    memset(u3_rxbuffer, 0, u3_rxsize); //���CAT1���ջ�����
    printf("AT+SYSINFO\r\n");          //����ָ��
    while (timeout--)
    {                                 //�ȴ���ʱʱ�䵽0
        HAL_Delay(100);                //��ʱ100ms
        if (strstr(u3_rxbuffer, ",")) //������յ�,��ʾָ��ɹ�
            break;                    //��������whileѭ��
        printf("%d ", timeout);    //����������ڵĳ�ʱʱ��
    }
    printf("\r\n"); //���������Ϣ
    if (timeout <= 0)
        return 1; //���timeout<=0��˵����ʱʱ�䵽�ˣ�Ҳû���յ�,����1
    else
    {
        sscanf(u3_rxbuffer, "%[^:]:%[^,],%[^\r]", temp, temp, info); //�������
        printf("��ǰ������ʽ:%s\r\n", info);                         //������ʾ��Ϣ
    }
    return 0; //��ȷ ����0
}