# 基于STM32的智能鱼缸系统

##  实现了环境监测、自动控制等功能

1.  检测鱼缸环境（温度、浊度、水位距离）
2.  控制外设定的开关，通过设定阈值实现自动控制。（自动喂食、自动控温、自动换水）
3.  实现了微信小程序上位机，能够通过WIFI模块远程控制。主要通过KEIL软件和微信小程序开发平台实现单片机和上位机，其中单片机由OLED显示环境数据、蜂鸣器警报、按键进行控制。通信使用MQTT协议。

## PCB原理图
![Schematic_Fish_2023-08-31](https://github.com/Tsunegit/yugang/assets/80894635/538f3cae-11c9-44eb-acef-8a2f6a13ba1e)

![PCB_PCB_Fish_2023-08-31](https://github.com/Tsunegit/yugang/assets/80894635/2eb3c7d8-9fb0-4824-8a48-3d25ed664a0d)
