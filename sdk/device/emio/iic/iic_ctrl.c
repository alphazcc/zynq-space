/**
 * Copyright (c) 2022-2023，HelloAlpha
 *
 * Change Logs:
 * Date           Author       Notes
 */
#include "iic_ctrl.h"
#include "sleep.h"

#define I2C_DELAY(...)	usleep(__VA_ARGS__)

void IIC_Start(void)
{
    SDA_OUT();
    SDA_HIGH();
    SCL_HIGH();
    I2C_DELAY(4);
    SDA_LOW();
    I2C_DELAY(4);
    SCL_LOW();
    I2C_DELAY(2);
}
 
void IIC_Stop(void)
{
    SDA_OUT();
    SCL_LOW();
    SDA_LOW();
    I2C_DELAY(4);
    SCL_HIGH();
    SDA_HIGH();
    I2C_DELAY(4);
}
 
// 1，接收应答失败
// 0，接收应答成功
uint8_t IIC_Wait_Ack(void)
{
    uint8_t ucErrTime=0;

    SDA_OUT();
    SDA_HIGH();
    I2C_DELAY(1);
    SCL_HIGH();
    I2C_DELAY(1);
    SDA_IN();
    while(SDA_READ())
    {
        ucErrTime++;
        if(ucErrTime > 250)
        {
            IIC_Stop();
            return 1;
        }
    }
    SCL_LOW();
    return 0;  
}

//产生ACK应答
void IIC_Ack(void)
{
    SCL_LOW();
    SDA_OUT();
    SDA_LOW();
    I2C_DELAY(2);
    SCL_HIGH();
    I2C_DELAY(2);
    SCL_LOW();
}
//不产生ACK应答
void IIC_NAck(void)
{
    SCL_LOW();
    SDA_OUT();
    SDA_HIGH();
    I2C_DELAY(2);
    SCL_HIGH();
    I2C_DELAY(2);
    SCL_LOW();
}				

void IIC_Write_Byte(uint8_t data)
{
    uint8_t  i = 0;
 
    SDA_OUT();
    SCL_LOW();
    for(i=0; i<8; i++)
    {
        if(data & 0x80 )
            SDA_HIGH();
        else
            SDA_LOW();
        data <<= 1;
        I2C_DELAY(2);
        SCL_HIGH();
        I2C_DELAY(2);
        SCL_LOW();
        I2C_DELAY(2);
    }
}

// ack = 1，发送 ACK
// ack = 0，发送 nACK
uint8_t IIC_Read_Byte(uint8_t ack)
{
    uint8_t i = 0, data = 0;
 
    SDA_IN();
    for(i = 0; i < 8; i++)
    {
        SCL_LOW();
        I2C_DELAY(2);
         SCL_HIGH();
        data <<= 1;
        if(SDA_READ())
            data++;
        I2C_DELAY(1);
    }
    if(!ack)
        IIC_NAck();
    else
        IIC_Ack();
    return data;
}
 
void IIC_Write_UINT16(uint8_t dev_addr, uint8_t wr_addr, uint16_t data)
{
    IIC_Start();
    IIC_Write_Byte(dev_addr);
    IIC_Wait_Ack(); 
    IIC_Write_Byte(wr_addr);
    IIC_Wait_Ack(); 
    IIC_Write_Byte(data >> 8);
    IIC_Wait_Ack(); 
    IIC_Write_Byte(data & 0xFF);
    IIC_Wait_Ack();  	
    IIC_Stop();
    I2C_DELAY(800);
}

uint16_t IIC_Read_UINT16(uint8_t dev_addr, uint8_t rd_addr)
{
    uint16_t data;

    IIC_Start();
    IIC_Write_Byte(dev_addr);
    IIC_Wait_Ack();
    IIC_Write_Byte(rd_addr);
    IIC_Wait_Ack(); 

    IIC_Start();  	
    IIC_Write_Byte(dev_addr + 1);
    IIC_Wait_Ack();
    data = IIC_Read_Byte(1);
    data = (data <<8 ) + IIC_Read_Byte(0);
    IIC_Stop();

    return data;
}
