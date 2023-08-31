#ifndef __ONEWIRE_H
#define __ONEWIRE_H
u8 DS18B20_Start(void);
void DS18B20_IN_OUT(u8 cmd);
void DS18B20_Init(void);
void DS18B20_Write(u8 data);
u8 DS18B20_Read(void);
u16 DS18B20_Read_Temp(void);
#endif

