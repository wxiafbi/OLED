#include "cat1.h"
#include "usart.h"
#include <string.h>

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
    // printf("���ܵ�1%c\r\n",u3_rxbuffer[0]);
    // printf("���ܵ�2%c\r\n",u3_rxbuffer[1]);
    // printf("���ܵ�3%c\r\n",u3_rxbuffer[2]);
    // printf("���ܵ�4%c\r\n",u3_rxbuffer[3]);
    // printf("���ܵ�5%c\r\n",u3_rxbuffer[4]);
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
    //uint8_t k[] = {'+', '+', '+'};
    // CAT1_RxCounter=0;                         //������������������
    u3_count = 0;
    memset(u3_rxbuffer, 0, u3_rxsize); //��ս��ջ�����
    // CAT1_printf("+++");                //����ָ��
    HAL_UART_Transmit(&huart3, (uint8_t *)"+++", 3, 0xfff);
    while (timeout--)
    {                                 //�ȴ���ʱʱ�䵽0
        HAL_Delay(100);               //��ʱ100ms
        if (strstr(u3_rxbuffer, "a")) //������յ�a��ʾ�ɹ�
            break;                    //��������whileѭ��
        printf("%d ", timeout);       //����������ڵĳ�ʱʱ��
    }
    printf("\r\n"); //���������Ϣ
    if (timeout <= 0)
        return 1; //���timeout<=0��˵����ʱʱ�䵽�ˣ�Ҳû���յ�a������1
    else
    {
        // CAT1_RxCounter = 0;                       //������������������
        u3_count = 0;
        memset(u3_rxbuffer, 0, u3_rxsize); //��ս��ջ�����
        // CAT1_printf("a");                         //����ָ��
        HAL_UART_Transmit(&huart3, "a", 2, 0xfff);
        while (timeout--)
        {                                   //�ȴ���ʱʱ�䵽0
            HAL_Delay(100);                 //��ʱ100ms
            if (strstr(u3_rxbuffer, "+ok")) //������յ�+ok��ʾ�ɹ�
                break;                      //��������whileѭ��
            printf("%d ", timeout);         //����������ڵĳ�ʱʱ��
        }
        printf("\r\n"); //���������Ϣ
        if (timeout <= 0)
            return 2; //���timeout<=0��˵����ʱʱ�䵽�ˣ�Ҳû���յ�+ok������1
    }
    return 0; //��ȷ ����0
}