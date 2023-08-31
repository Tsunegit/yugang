#include "stm32f10x.h"
#include "beep.h"

BEEP_INFO beepInfo = {0};


void Beep_Init(void)
{

	GPIO_InitTypeDef gpioInitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		//打开GPIOB的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
   GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);	//禁止JTAG功能
	gpioInitStruct.GPIO_Mode = GPIO_Mode_Out_PP;				//设置为输出
	gpioInitStruct.GPIO_Pin = GPIO_Pin_11;						//将初始化的Pin脚
	gpioInitStruct.GPIO_Speed = GPIO_Speed_50MHz;				//可承载的最大频率
	
	GPIO_Init(GPIOB, &gpioInitStruct);							//初始化GPIO
	
	Beep_Set(BEEP_OFF);											//初始化完成后，关闭蜂鸣器

}


void Beep_Set(_Bool status)
{
	
	GPIO_WriteBit(GPIOB, GPIO_Pin_11, status == BEEP_ON ? Bit_SET : Bit_RESET);		//如果status等于BEEP_ON，则返回Bit_SET，否则返回Bit_RESET
	
	beepInfo.Beep_Status = status;
	
	
}
