#include "ath10.h"
#include "usart.h"
#include "i2c.h"

void AHT10_Reset(void)
{
    uint8_t RESER_CMD[1] = {0xBA};
    HAL_I2C_Master_Transmit(&hi2c1, 0X70, &RESER_CMD[0], 1, 0XFF);
}
/*-------------------------------------------------*/
/*函数名：查询工作模式和校准位正确与否             */
/*参  数：无                                       */
/*返回值：0正确 1失败                              */
/*-------------------------------------------------*/
char AHT10_Read_CalEnable(void)
{
    char val = 0;

    val = AHT10_ReadStatus(); //读取状态寄存器
    if ((val & 0x68) == 0x08) //判断工作模式和校准输出是否有效
        return 0;             //正确返回0
    else
        return 1; //失败返回1
}
/*-------------------------------------------------*/
/*函数名：读取AHT10的状态寄存器                    */
/*参  数：无                                       */
/*返回值：寄存器值                                 */
/*-------------------------------------------------*/
char AHT10_ReadStatus(void)
{

    uint8_t Status;
    printf("Status初始状态=%d\r\n", Status);

    HAL_Delay(200);
    HAL_I2C_Master_Receive(&hi2c1, 0x71, (uint8_t *)&Status, 1, 0xff);

    return Status;
}
/*-------------------------------------------------*/
/*函数名：向AHT10发送读温湿度数据的命令            */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void AHT10_ReadData(void)
{
    uint8_t ReadData_CMD[3] = {0xAC, 0x33, 0x00};
    HAL_I2C_Mem_Write(&hi2c1, 0x70, 0, I2C_MEMADD_SIZE_8BIT, ReadData_CMD, 3, 1000);
}
/*-------------------------------------------------*/
/*函数名：初始化AHT10                              */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
char AHT10_Init(void)
{
    char times;
    uint8_t calibration_cmd[3] = {0xE1, 0x08, 0x00};
    HAL_I2C_Mem_Write(&hi2c1, 0x70, 0, I2C_MEMADD_SIZE_8BIT, calibration_cmd, 3, 1000);
    HAL_Delay(500); //延时

    while (AHT10_Read_CalEnable() == 1) //查询工作模式和校准位正确与否
    {
        AHT10_Reset(); //复位AHT10
                       // HAL_Delay(100);                 //延时
        HAL_I2C_Mem_Write(&hi2c1, 0x70, 0, I2C_MEMADD_SIZE_8BIT, calibration_cmd, 3, 1000);
        times++;        //重复次数+1
        HAL_Delay(500); //延时
        if (times >= 10)
            return 1; //重复此时超过10了，失败返回1
    }
    return 0; //正确返回0
}
/*-------------------------------------------------*/
/*函数名：读传感器数据                             */
/*参  数：temp 保存温度数据                        */
/*参  数：humi 保存湿度数据                        */
/*返回值：读到的数据                               */
/*-------------------------------------------------*/
void AHT10_Data(double *temp, double *humi)
{
    int tdata = 0;
    char Redata[6] = {0};

    AHT10_ReadData(); //读数据
    HAL_Delay(500);   //延迟
    HAL_I2C_Mem_Read(&hi2c1, 0x71, 0, I2C_MEMADD_SIZE_8BIT, (uint8_t *)Redata, 6, 100);
    //HAL_I2C_Master_Receive(&hi2c1, 0x71, (uint8_t *)Redata, 1, 0xff);
    HAL_Delay(5); //延迟

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
