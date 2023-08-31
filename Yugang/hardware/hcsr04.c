#include "stm32f10x.h"                  // Device header
#include "Delay.h"

//超声波硬件接口定义
#define HCSR04_PORT     GPIOA
#define HCSR04_CLK      RCC_APB2Periph_GPIOA
#define HCSR04_TRIG     GPIO_Pin_6
#define HCSR04_ECHO     GPIO_Pin_7

u16 msHcCount = 0;
void Hcsr04Init(void)
{
	
		GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(HCSR04_CLK, ENABLE);
   
    GPIO_InitStructure.GPIO_Pin =HCSR04_TRIG;      
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(HCSR04_PORT, &GPIO_InitStructure);
    GPIO_ResetBits(HCSR04_PORT,HCSR04_TRIG);
     
    GPIO_InitStructure.GPIO_Pin =   HCSR04_ECHO;     
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(HCSR04_PORT, &GPIO_InitStructure);  
    GPIO_ResetBits(HCSR04_PORT,HCSR04_ECHO);	
	
	//开启时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	//配置时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 49999;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	//中断输出控制
	TIM_ClearFlag(TIM2, TIM_FLAG_Update); //清楚标志位，在配置时基单元时会自动进入一次更新中断
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	
	//NVIC
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	
	NVIC_Init(&NVIC_InitStructure);
	
	//中断使能
	TIM_Cmd(TIM2,ENABLE);
}

uint16_t getCount(void)
{
	return TIM_GetCounter(TIM2);
}

//定时器2中断
void TIM2_IRQHandler(void)  
{
   if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  
   {
       TIM_ClearITPendingBit(TIM2, TIM_IT_Update); 
       msHcCount++;
   }
}
 
	int num = 0;
//通过定时器4计数器值推算距离
float Hcsr04GetLength(void)
{
	u32 t = 0;

	float lengthTemp = 0;
	
	GPIO_SetBits(HCSR04_PORT,HCSR04_TRIG);
	Delay_us(20);
	GPIO_ResetBits(HCSR04_PORT,HCSR04_TRIG);
	
	while(GPIO_ReadInputDataBit(HCSR04_PORT,HCSR04_ECHO) == 0);      
	TIM2->CNT=0;
	while(GPIO_ReadInputDataBit(HCSR04_PORT,HCSR04_ECHO) == 1);    

	t = TIM2->CNT;
	lengthTemp = ((float)t)/58;//cm
    return lengthTemp;
}

