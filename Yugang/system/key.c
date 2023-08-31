#include "stm32f10x.h"                  // Device header
#include "Delay.h"

void Key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_Initstructure;
	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Initstructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_Initstructure);
	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Initstructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_Initstructure);
	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Initstructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_Initstructure);
	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Initstructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_Initstructure);
}

uint8_t Key_GetNum6(void)
{
	
	uint8_t keynum = 0;
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6) == 0)
	{
		Delay_ms(20);
		while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6) == 0);
		Delay_ms(20);
		keynum = 1;
	}
	return keynum;
}

uint8_t Key_GetNum7(void)
{
	
	uint8_t keynum = 0;
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7) == 0)
	{
		Delay_ms(20);
		while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7) == 0);
		Delay_ms(20);
		keynum = 1;
	}
	return keynum;
}

uint8_t Key_GetNum8(void)
{
	
	uint8_t keynum = 0;
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8) == 0)
	{
		Delay_ms(20);
		while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8) == 0);
		Delay_ms(20);
		keynum = 1;
	}
	return keynum;
}

uint8_t Key_GetNum9(void)
{
	
	uint8_t keynum = 0;
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9) == 0)
	{
		Delay_ms(20);
		while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9) == 0);
		Delay_ms(20);
		keynum = 1;
	}
	return keynum;
}
