#include "stm32f10x.h"                  // Device header
#include "Delay.h"

#define DS18B20 GPIO_Pin_0
#define DS18B20_PORT GPIOA
#define DS18B20_LOW GPIO_ResetBits(DS18B20_PORT,DS18B20)
#define DS18B20_HIGH GPIO_SetBits(DS18B20_PORT,DS18B20)

void DS18B20_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_InitSturcture;
	GPIO_InitSturcture.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitSturcture.GPIO_Pin = DS18B20;
	GPIO_InitSturcture.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(DS18B20_PORT,&GPIO_InitSturcture);
	
}

void DS18B20_IN_OUT(u8 cmd)
{
	GPIO_InitTypeDef GPIO_InitSturcture;
	if(cmd)
	{
		GPIO_InitSturcture.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitSturcture.GPIO_Pin = DS18B20;
	}
	else
	{
		GPIO_InitSturcture.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_InitSturcture.GPIO_Pin = DS18B20;
	}	
	GPIO_InitSturcture.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(DS18B20_PORT,&GPIO_InitSturcture);
}

u8 DS18B20_Start(void)
{
	DS18B20_IN_OUT(1);
	u8 data;
	DS18B20_LOW;
	Delay_us(480);
	DS18B20_HIGH;
	Delay_us(30);
	DS18B20_IN_OUT(0);
	data = GPIO_ReadInputDataBit(DS18B20_PORT,DS18B20);
	Delay_us(200);
	return data; 
}

void DS18B20_Write(u8 data)
{
	for(u8 i =0; i<8;i++)
	{
		DS18B20_IN_OUT(1);
		DS18B20_LOW;
		Delay_us(2);
		(data&0x01) ? DS18B20_HIGH:DS18B20_LOW;
		Delay_us(45);
		DS18B20_HIGH;
		
		data >>= 1;
		
	}
}

u8 DS18B20_Read(void)
{
	u8 data;
	
	for(u8 i=0;i<8;i++)
	{
		data>>=1;
		DS18B20_IN_OUT(1);
		DS18B20_LOW;
		Delay_us(2);
		DS18B20_HIGH;
		DS18B20_IN_OUT(0);
		if(GPIO_ReadInputDataBit(DS18B20_PORT,DS18B20) == SET)
		{
			data |= 0x80;
			
		}
		Delay_us(45);
	}
	return data;
}

u16 DS18B20_Read_Temp()
{
	u16 data;
	u8 LSB=0,HSB=0;
	u16 temp=0;
	DS18B20_Start();
	DS18B20_Write(0xCC);	//跳过ROM
	DS18B20_Write(0x44);	//温度转换
	Delay_ms(75);
	DS18B20_Start();
	DS18B20_Write(0xCC);
	DS18B20_Write(0xbe);	//读取温度
	
	LSB = DS18B20_Read();
	HSB = DS18B20_Read();
	
	temp = (HSB << 8) | LSB;
	
	//前五位表示正负
	if((temp&0xF800) == 0xF800)
	{
		//-
		data = (((~temp+0x01) * -0.0625)+0.5) * 10.0;
	}
	else
	{
		//+
		data = ((temp *0.0625)+0.5) * 10.0;
	}
	
	return data;
}
