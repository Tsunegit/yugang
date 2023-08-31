
//工具函数
#include "stm32f10x.h"   
#include "Delay.h"
#include "time3time.h"
#include "time1Btn.h"

//硬件
#include "usart.h"
#include "OLED.h"
#include "PWM.h"
#include "DS18B20.h"
#include "hcsr04.h"
#include "beep.h"
#include "tsw30.h"
#include "MyGPIO.h"

//#include "ButtonExtern.h"
#include "key.h"

//网络
#include "esp8266.h"	
#include "onenet.h"


//C库
#include <string.h>
#include <stdio.h>

//数据库
#include "scopedata.h"

//网络参数 
char PUB_BUF[128];
const char *devSubTopic[] = {"/yugang/sub"};
const char devPubTopic[] = {"/yugang/pub"};


//传感器参数
float length;
u16 temperature,Tu;



//状态值
u8 MODE = 2;

void heater(_Bool SET)
{
//	GPIO_ResetBits(GPIOA,GPIO_Pin_12);
	if(SET)
	{
//		OLED_ShowString(4,7,"Heater_ON");
		GPIO_ResetBits(GPIOA,GPIO_Pin_12);
	}
	else
	{
//		OLED_ShowString(4,5,"Heater_FF");
		GPIO_SetBits(GPIOA,GPIO_Pin_12);
	}
}
void waterPu(_Bool SET)
{
	if(!SET)
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_5);
	}
	else
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_5);
	}
}


int main()
{	
	//arg
	unsigned short timeCount = 0;	//发送间隔变量
	unsigned char *dataPtr =NULL;	
	
	
	
	//Init-----------------------------------------------------------
	OLED_Init();
	DS18B20_Init();
	Hcsr04Init();
	PWM_Init();
	Beep_Init();
	AD_Init();
	MyGPIO_Init();
	Key_Init();
//	ButtonExtern_Init();
	
	
	//连接wifi
	OLED_ShowString(1,1,"WIFI CONNECT...");

	Usart1_Init(115200);		//debug
	Usart2_Init(115200);		//8266
	
	
	ESP8266_Init();		//初始化8266
	
	TIM3_Int_Init(4999,14399);

	OLED_ShowString(2,1,"WIFI CONNECT SUCCESS!");
	Delay_ms(500);
	
	OLED_ShowString(3,1,"MQTT CONNECT...");
	//连接服务器
	while(OneNet_DevLink())
	{
		Delay_ms(500);
	}
	
	Beep_Set(BEEP_ON);				//鸣叫提示接入成功
	Delay_ms(250);
	Beep_Set(BEEP_OFF);
	
	OLED_ShowString(4,1,"Welcome");
	Delay_ms(250);
	OLED_Clear();
	
	OneNet_Subscribe(devSubTopic, 1);
	
	//arg
	u16 temph,templ,currentS,userSetS;
	u16 turbidity;
	u16 lengthScope;
	u16 tempScope;
	_Bool f = 1;
	_Bool h = 1;
	
	
	
	//test-----------------------------------------------------------
	//UsartPrintf(USART1,"Init OK!");
	
	
	while(1)
	{
		
	if(Key_GetNum9())
	{
		MODE ++;
		if(MODE >2)
		{
			MODE = 1;
		}
		Beep_Set(1);
		Delay_ms(250);
		Beep_Set(0);
	}
		
	
		
		//获取环境数据
		length = Hcsr04GetLength();
		temperature = DS18B20_Read_Temp();
		temph = temperature /10;
		templ = temperature %10;
		Tu = 100 - get_TUValue();
		
			
		if(MODE == 1)
		{
			OLED_ShowString(3,8,"M1");
			
			if(Key_GetNum6())
			{
				heater(h);
				h = !h;
				UsartPrintf(USART_DEBUG,"666666666666666");
			}
			if(Key_GetNum7())
			{
				
				waterPu(f);
				f = !f;
				UsartPrintf(USART_DEBUG,"777777777777");
			}
			if(Key_GetNum8())
			{
				Steering();
				UsartPrintf(USART_DEBUG,"88888888888888");
			}
			
			
			
		}	
			
		
		/**
		
			自动模式
		
		*/
		if(MODE == 2)
		{
			
			OLED_ShowString(3,8,"M2");
			
			//获取阈值数据---------------------------------
			tempScope = getTempScope();
			lengthScope = getLengthScope();
			turbidity = getTurbidity();
			userSetS = getMinScope()*60 + (getHourScope()*60*60);
			currentS = sec_return+min_return*60 + (hour_return*60*60);
			
			
			if(currentS>=userSetS)
			{
				if(currentS % userSetS == 0)
				{
					UsartPrintf(USART_DEBUG,"currentTime:[%d],userSetTime:[%d]",currentS,userSetS);
						Beep_Set(BEEP_ON);				//鸣叫提示时间到了
						Steering();
						Beep_Set(BEEP_OFF);
				}
			}

			
			if(Tu>turbidity)
			{
				waterPu(1);
			}else
			{
				waterPu(0);
			}

			if(length>lengthScope)
			{
				waterPu(1);
			}
			else
			{
				waterPu(0);
			}

			if((tempScope*10) > temperature)
			{
				heater(1);
			}
			else
			{
				heater(0);
			}
			
		}
		
		
					//显示部分--------------------------------------------
			//时间显示
			OLED_ShowString(4,3,":");
			OLED_ShowNum(4,1,hour_return,2);
			OLED_ShowString(4,6,":");
			OLED_ShowNum(4,4,min_return,2);
			OLED_ShowNum(4,7,sec_return,2);
		
			OLED_ShowNum(4,10,getHourScope(),2);
			OLED_ShowString(4,12,":");
			OLED_ShowNum(4,13,getMinScope(),2);

			
		
		
//			OLED_ShowString(4,1,MODE == 1?"hard":"auto");
		//环境参数显示
						//温度显示
			OLED_ShowString(1,1,"T_M:");
			OLED_ShowNum(1,5,temph,2);
			OLED_ShowString(1,7,".");
			OLED_ShowNum(1,8,templ,1);
			OLED_ShowString(1,10,"[");
			OLED_ShowNum(1,11,tempScope,2);
			OLED_ShowString(1,13,"]");
		
				//水位显示
			OLED_ShowString(2,1,"LEN:");
			OLED_ShowNum(2,5,length,3);
			OLED_ShowString(2,10,"[");
			OLED_ShowNum(2,11,lengthScope,2);
			OLED_ShowString(2,13,"]");
			
			//浑浊度
			OLED_ShowString(3,1,"T_U:");
			OLED_ShowNum(3,5,Tu,2);
			OLED_ShowString(3,10,"[");
			OLED_ShowNum(3,11,turbidity,2);
			OLED_ShowString(3,13,"]");
			
		if(timeCount% 18 == 0)		// 1000/161=6.2...	大约1s显示
		{
			UsartPrintf(USART_DEBUG, "OneNet_Publish\r\n");
			sprintf(PUB_BUF,"{\"Temp\":%d.%d,\"Pos\":%.2f,\"Water\":%d,\"TempScope\":%d,\"LengthScope\":%d,\"Turbidity\":%d}"
			,temph,templ,length,Tu,tempScope,lengthScope,turbidity);

			OneNet_Publish(devPubTopic, PUB_BUF);
			memset(PUB_BUF,0,128);
			ESP8266_Clear();	
		}


		//网络
			//5s发送 环境数据
		if(++timeCount >= 31)		// 5000/25=200	5s间隔
		{
			
		}
		
		dataPtr = ESP8266_GetIPD(3);		
		if(dataPtr != NULL)
			OneNet_RevPro(dataPtr);
		Delay_ms(10);							
			
			//经过测试 一次循环大概161毫秒
		}

}


//外部中断
//void EXTI9_5_IRQHandler(void)
//{
//	if(EXTI_GetITStatus(EXTI_Line6)!=RESET)
//	{	  
//		TIM1_Int_Init(99,7199);//72M/(7199+1)=10Khz计数频率，(999+1)/10KHz=100ms定时时间 
//		EXTI_ClearITPendingBit(EXTI_Line6);
//	}
//	if(EXTI_GetITStatus(EXTI_Line7)!=RESET)
//	{
//		TIM1_Int_Init(99,7199);//72M/(7199+1)=10Khzw计数频率，(999+1)/10KHz=100ms定时时间 
//		EXTI_ClearITPendingBit(EXTI_Line7);
//	}
//	if(EXTI_GetITStatus(EXTI_Line8)!=RESET)
//	{
//		TIM1_Int_Init(99,7199);//72M/(7199+1)=10Khzw计数频率，(999+1)/10KHz=100ms定时时间 
//		EXTI_ClearITPendingBit(EXTI_Line8);
//	}
//	if(EXTI_GetITStatus(EXTI_Line9)!=RESET)
//	{
//		TIM1_Int_Init(99,7199);//72M/(7199+1)=10Khzw计数频率，(999+1)/10KHz=100ms定时时间 
//		EXTI_ClearITPendingBit(EXTI_Line9);
//	}
//	
//}



//void TIM1_IRQHandler(void) 
//{
//	
//	
//	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET) 
//	{		
//		
//		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)==0)      //按键1仍然按下
//		{	
//			UsartPrintf(USART_DEBUG,"按键1仍然按下");
//		    //进行按键处理
//			if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)==SET)
//			{
//				heater(1);
//			}else
//			{
//				heater(0);
//			}
//			
//			TIM_SetCounter(TIM1,0);	 //清零定时器       
//			TIM_Cmd(TIM1, DISABLE);  //失能定时器
//		}
//		
//		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)==SET)      //按键2仍然按下
//		{	
//			UsartPrintf(USART_DEBUG,"按键2仍然按下");
//		    //进行按键处理
//			if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)==RESET)
//			{
//				waterPu(1);
//			}else
//			{
//				waterPu(0);
//			}
//			TIM_SetCounter(TIM1,0);	 //清零定时器       
//			TIM_Cmd(TIM1, DISABLE);  //失能定时器
//		}
//		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)==SET)      //按键3仍然按下
//		{	
//		    //进行按键处理
//			Steering();
//			UsartPrintf(USART_DEBUG,"按键3仍然按下");
//			TIM_SetCounter(TIM1,0);	 //清零定时器       
//			TIM_Cmd(TIM1, DISABLE);  //失能定时器
//		}
//		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)==SET)      //按键4仍然按下
//		{	
//		    //进行按键处理
//			MODE = 1;
//			UsartPrintf(USART_DEBUG,"按键4仍然按下");
//			TIM_SetCounter(TIM1,0);	 //清零定时器       
//			TIM_Cmd(TIM1, DISABLE);  //失能定时器
//		}
//		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);   
//	} 
//}
