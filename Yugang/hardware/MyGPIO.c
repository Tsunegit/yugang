#include "stm32f10x.h"                  // Device header


void MyGPIO_Init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitSturcture;
	

	
	GPIO_InitSturcture.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitSturcture.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitSturcture.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA,&GPIO_InitSturcture);
	GPIO_ResetBits(GPIOA,GPIO_Pin_12);
	
	GPIO_InitSturcture.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitSturcture.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitSturcture.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA,&GPIO_InitSturcture);
	GPIO_SetBits(GPIOA,GPIO_Pin_5);
	
	GPIO_InitSturcture.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitSturcture.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitSturcture.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB,&GPIO_InitSturcture);
	GPIO_SetBits(GPIOB,GPIO_Pin_5);
	
}
