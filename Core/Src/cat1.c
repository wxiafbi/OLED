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
    memset(u3_rxbuffer, 0x00, u3_rxsize); //��ս��ջ�����
    // CAT1_printf("+++");                //����ָ��
    HAL_UART_Transmit(&huart3, (uint8_t *)"+++", 3, 0xfff);
    HAL_UART_Transmit(&huart1, (uint8_t *)"+++\r\n", sizeof("+++"), 0xfff);
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
    // printf("111111\r\n"); //���������Ϣ
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
        memset(u3_rxbuffer, 0x00, u3_rxsize); //��ս��ջ�����
        // CAT1_printf("a");                         //����ָ��
        HAL_UART_Transmit(&huart3, (uint8_t *)"a", 1, 0xfff);
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
    printf("�˳�͸��ģʽ�ɹ�\r\n");
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
    u3_count = 0;                         // CAT1������������������
    memset(u3_rxbuffer, 0x00, u3_rxsize); //���CAT1���ջ�����
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
    char temp[20];
    char csq[20];

    printf("�����ѯ�ź�ǿ��\r\n");
    u3_count = 0;                         // CAT1������������������
    memset(u3_rxbuffer, 0x00, u3_rxsize); //���CAT1���ջ�����
    memset(temp, 0x00, 20);               //���CAT1���ջ�����
    // CAT1_printf("usr.cn#AT+CSQ\r\n");  //����ָ��
    HAL_UART_Transmit(&huart3, (uint8_t *)"AT+CSQ\r\n", 8, 0xfff);
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
        OLED_ShowString(81, 0, (uint8_t *)csq);
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

    u3_count = 0;                         // CAT1������������������
    memset(u3_rxbuffer, 0x00, u3_rxsize); //���CAT1���ջ�����
    // printf("AT+SYSINFO\r\n");          //����ָ��
    HAL_UART_Transmit(&huart3, (uint8_t *)"AT+SYSINFO\r\n", 12, 0xfff);
    while (timeout--)
    {                                 //�ȴ���ʱʱ�䵽0
        HAL_Delay(100);               //��ʱ100ms
        if (strstr(u3_rxbuffer, ",")) //������յ�,��ʾָ��ɹ�
            break;                    //��������whileѭ��
        printf("%d ", timeout);       //����������ڵĳ�ʱʱ��
    }
    printf("\r\n"); //���������Ϣ
    if (timeout <= 0)
        return 1; //���timeout<=0��˵����ʱʱ�䵽�ˣ�Ҳû���յ�,����1
    else
    {
        sscanf(u3_rxbuffer, "%[^:]:%[^,],%[^\r]", temp, temp, info); //�������
        for (size_t i = 0; i <100; i++)
        {
            /* code */
            printf("%c",u3_rxbuffer[i]);
        }
        
        printf("��ǰ������ʽ:%s\r\n", info);                         //������ʾ��Ϣ
    }
    return 0; //��ȷ ����0
}
char CAT1_LBSN(int timeout)
{
    char temp[20];
    char info[25];

    u3_count = 0;                         // CAT1������������������
    memset(u3_rxbuffer, 0x00, u3_rxsize); //���CAT1���ջ�����
    // printf("AT+SYSINFO\r\n");          //����ָ��
    HAL_UART_Transmit(&huart3, (uint8_t *)"AT+SN\r\n", 7, 0xfff);
    while (timeout--)
    {                                 //�ȴ���ʱʱ�䵽0
        HAL_Delay(100);               //��ʱ100ms
        if (strstr(u3_rxbuffer, ":")) //������յ�,��ʾָ��ɹ�
            break;                    //��������whileѭ��
        printf("%d ", timeout);       //����������ڵĳ�ʱʱ��
    }
    printf("\r\n"); //���������Ϣ
    if (timeout <= 0)
        return 1; //���timeout<=0��˵����ʱʱ�䵽�ˣ�Ҳû���յ�,����1
    else
    {
        sscanf(u3_rxbuffer, "%[^:]:%[^\r]", temp,info); //�������
        for (size_t i = 0; i <100; i++)
        {
            /* code */
            printf("%c",u3_rxbuffer[i]);
        }
        
        printf("��ǰ������ʽ:%s\r\n", info);                         //������ʾ��Ϣ
    }
    return 0; //��ȷ ����0
}
/*-------------------------------------------------*/
/*��������4Gģ�����ӷ�����                         */
/*��  ����timeout�� ��ʱʱ�䣨100ms�ı�����        */
/*����ֵ��0����ȷ   ����������                     */
/*-------------------------------------------------*/
// char CAT1_Connect_IoTServer(int timeout)
// {
//     char temp[256];

//     u1_printf("׼����λģ��\r\n"); //������ʾ����
//     if (CAT1_Reset(20))
//     {                                        //��λ��1s��ʱ��λ���ܼ�20s��ʱʱ��
//         u1_printf("��λʧ�ܣ�׼������\r\n"); //���ط�0ֵ������if��������ʾ����
//         return 1;                            //����1
//     }
//     else
//         u1_printf("��λ�ɹ�\r\n"); //������ʾ����

//     u1_printf("׼���˳�͸��\r\n"); //������ʾ����
//     if (CAT1_ExitTrans(50))
//     {                                            //�˳�͸����100ms��ʱ��λ���ܼ�5s��ʱʱ��
//         u1_printf("�˳�͸��ʧ�ܣ�׼������\r\n"); //���ط�0ֵ������if��������ʾ����
//         return 2;                                //����2
//     }
//     else
//         u1_printf("�˳�͸���ɹ�\r\n"); //������ʾ����

//     u1_printf("׼������NETģʽ\r\n"); //������ʾ����
//     if (CAT1_SendCmd("AT+WKMOD=NET", 50))
//     {                                               //����NETģʽ��100ms��ʱ��λ���ܼ�5s��ʱʱ��
//         u1_printf("����NETģʽʧ�ܣ�׼������\r\n"); //���ط�0ֵ������if��������ʾ����
//         return 5;                                   //����5
//     }
//     else
//         u1_printf("����NETģʽ�ɹ�\r\n"); //������ʾ����

//     u1_printf("׼��ʹ������A\r\n"); //������ʾ����
//     if (CAT1_SendCmd("AT+SOCKAEN=ON", 50))
//     {                                             //ʹ������A��100ms��ʱ��λ���ܼ�5s��ʱʱ��
//         u1_printf("ʹ������Aʧ�ܣ�׼������\r\n"); //���ط�0ֵ������if��������ʾ����
//         return 6;                                 //����6
//     }
//     else
//         u1_printf("ʹ������A�ɹ�\r\n"); //������ʾ����

//     u1_printf("׼���ر�����B\r\n"); //������ʾ����
//     if (CAT1_SendCmd("AT+SOCKBEN=OFF", 50))
//     {                                             //�ر�����B��100ms��ʱ��λ���ܼ�5s��ʱʱ��
//         u1_printf("�ر�����Bʧ�ܣ�׼������\r\n"); //���ط�0ֵ������if��������ʾ����
//         return 7;                                 //����7
//     }
//     else
//         u1_printf("�ر�����B�ɹ�\r\n"); //������ʾ����

//     u1_printf("׼����������A������\r\n"); //������ʾ����
//     if (CAT1_SendCmd("AT+SOCKASL=LONG", 50))
//     {                                                   //��������A�����ӣ�100ms��ʱ��λ���ܼ�5s��ʱʱ��
//         u1_printf("��������A������ʧ�ܣ�׼������\r\n"); //���ط�0ֵ������if��������ʾ����
//         return 8;                                       //����8
//     }
//     else
//         u1_printf("��������A�����ӳɹ�\r\n"); //������ʾ����

//     u1_printf("׼���ر��Դ���������\r\n"); //������ʾ����
//     if (CAT1_SendCmd("AT+HEARTEN=OFF", 50))
//     {                                                    //�ر��Դ��������ܣ�100ms��ʱ��λ���ܼ�5s��ʱʱ��
//         u1_printf("�ر��Դ���������ʧ�ܣ�׼������\r\n"); //���ط�0ֵ������if��������ʾ����
//         return 9;                                        //����9
//     }
//     else
//         u1_printf("�ر��Դ��������ܳɹ�\r\n"); //������ʾ����

//     u1_printf("׼����������A���ӷ�����\r\n");                  //������ʾ����
//     sprintf(temp, "AT+SOCKA=TCP,%s,%d", ServerIP, ServerPort); //��������
//     if (CAT1_SendCmd(temp, 50))
//     {                                                       //��������A���ӷ�������100ms��ʱ��λ���ܼ�5s��ʱʱ��
//         u1_printf("��������A���ӷ�����ʧ�ܣ�׼������\r\n"); //���ط�0ֵ������if��������ʾ����
//         return 10;                                          //����10
//     }
//     else
//         u1_printf("��������A���ӷ������ɹ�\r\n"); //������ʾ����

//     u1_printf("�������ã�Ȼ������ģ��\r\n"); //������ʾ����
//     if (CAT1_SendCmd("AT+S", 50))
//     {                                            //�������ã�Ȼ������ģ�飬100ms��ʱ��λ���ܼ�5s��ʱʱ��
//         u1_printf("��������ʧ�ܣ�׼������\r\n"); //���ط�0ֵ������if��������ʾ����
//         return 11;                               //����11
//     }
//     else
//         u1_printf("�������óɹ�\r\n"); //������ʾ����

//     u1_printf("�ȴ����������ӷ�����\r\n"); //������ʾ����
//     while (timeout--)
//     {                              //�ȴ���ʱʱ�䵽0
//         Delay_Ms(1000);            //��ʱ1s
//         if (LINKA_STA == 0)        //��LINKA_STA PC1�ǵ͵�ƽ�ˣ���ʾ���ӳɹ�
//             break;                 //����whileѭ��
//         u1_printf("%d ", timeout); //����������ڵĳ�ʱʱ��
//     }
//     u1_printf("\r\n"); //���������Ϣ
//     if (timeout <= 0)
//         return 12; //��ʱ���󣬷���12

//     Delay_Ms(500); //��ʱ
//     return 0;      //��ȷ����0
// }
