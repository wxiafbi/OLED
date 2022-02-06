#include "ath10.h"
#include "usart.h"
#include "i2c.h"

void AHT10_Reset(void)
{
    uint8_t RESER_CMD[1] = {0xBA};
    HAL_I2C_Master_Transmit(&hi2c1, 0X70, &RESER_CMD[0], 1, 0XFF);
}
/*-------------------------------------------------*/
/*����������ѯ����ģʽ��У׼λ��ȷ���             */
/*��  ������                                       */
/*����ֵ��0��ȷ 1ʧ��                              */
/*-------------------------------------------------*/
char AHT10_Read_CalEnable(void)
{
    char val = 0;

    val = AHT10_ReadStatus(); //��ȡ״̬�Ĵ���
    if ((val & 0x68) == 0x08) //�жϹ���ģʽ��У׼����Ƿ���Ч
        return 0;             //��ȷ����0
    else
        return 1; //ʧ�ܷ���1
}
/*-------------------------------------------------*/
/*����������ȡAHT10��״̬�Ĵ���                    */
/*��  ������                                       */
/*����ֵ���Ĵ���ֵ                                 */
/*-------------------------------------------------*/
char AHT10_ReadStatus(void)
{

    uint8_t Status;
    printf("Status��ʼ״̬=%d\r\n", Status);

    HAL_Delay(200);
    HAL_I2C_Master_Receive(&hi2c1, 0x71, (uint8_t *)&Status, 1, 0xff);

    return Status;
}
/*-------------------------------------------------*/
/*����������AHT10���Ͷ���ʪ�����ݵ�����            */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void AHT10_ReadData(void)
{
    uint8_t ReadData_CMD[3] = {0xAC, 0x33, 0x00};
    HAL_I2C_Mem_Write(&hi2c1, 0x70, 0, I2C_MEMADD_SIZE_8BIT, ReadData_CMD, 3, 1000);
}
/*-------------------------------------------------*/
/*����������ʼ��AHT10                              */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
char AHT10_Init(void)
{
    char times;
    uint8_t calibration_cmd[3] = {0xE1, 0x08, 0x00};
    HAL_I2C_Mem_Write(&hi2c1, 0x70, 0, I2C_MEMADD_SIZE_8BIT, calibration_cmd, 3, 1000);
    HAL_Delay(500); //��ʱ

    while (AHT10_Read_CalEnable() == 1) //��ѯ����ģʽ��У׼λ��ȷ���
    {
        AHT10_Reset(); //��λAHT10
                       // HAL_Delay(100);                 //��ʱ
        HAL_I2C_Mem_Write(&hi2c1, 0x70, 0, I2C_MEMADD_SIZE_8BIT, calibration_cmd, 3, 1000);
        times++;        //�ظ�����+1
        HAL_Delay(500); //��ʱ
        if (times >= 10)
            return 1; //�ظ���ʱ����10�ˣ�ʧ�ܷ���1
    }
    return 0; //��ȷ����0
}
/*-------------------------------------------------*/
/*��������������������                             */
/*��  ����temp �����¶�����                        */
/*��  ����humi ����ʪ������                        */
/*����ֵ������������                               */
/*-------------------------------------------------*/
void AHT10_Data(double *temp, double *humi)
{
    int tdata = 0;
    char Redata[6] = {0};

    AHT10_ReadData(); //������
    HAL_Delay(500);   //�ӳ�
    HAL_I2C_Mem_Read(&hi2c1, 0x71, 0, I2C_MEMADD_SIZE_8BIT, (uint8_t *)Redata, 6, 100);
    //HAL_I2C_Master_Receive(&hi2c1, 0x71, (uint8_t *)Redata, 1, 0xff);
    HAL_Delay(5); //�ӳ�

    tdata = (tdata | Redata[1]) << 8;
    tdata = (tdata | Redata[2]) << 8;
    tdata = (tdata | Redata[3]);
    tdata = (tdata >> 4);
    *humi = tdata * 100.0 / 1024.0 / 1024.0;

    tdata = 0;
    tdata = (tdata | Redata[3]) << 8;
    tdata = (tdata | Redata[4]) << 8;
    tdata = (tdata | Redata[5]);
    tdata = tdata & 0xfffff;
    *temp = tdata * 200.0 / 1024.0 / 1024.0 - 50.0;
}
