#include <stdio.h>
#include "stm32f10x.h"                  // Device header


u16 tempScope = 30;
u16 lengthScope = 40;
u16 turbidity = 50;
u16 hourScope = 0;
u16 minScope = 1;


void setTempScope(u16 value)
{
	tempScope = value;
}

u16 getTempScope()
{
	return tempScope;
}

void setLengthScope(u16 value)
{
	lengthScope = value;
}

u16 getLengthScope()
{
	return lengthScope;
}

void setTurbidity(u16 value)
{
	turbidity = value;
}

u16 getTurbidity()
{
	return turbidity;
}

void setHourScope(u16 hour){
	hourScope = hour;
}
u16 getHourScope()
{
	return hourScope;
}

void setMinScope(u16 min){
	minScope = min;
}
u16 getMinScope()
{
	return minScope;
}

