/************************************************************************************
//STM32连接引脚是指TFTLCD插槽引脚内部连接的STM32引脚
//=========================================电源接线================================================//
//     LCD模块             TFTLCD插槽引脚        STM32连接引脚
//      VDD       --->           5V                  DC5V             //电源
//      GND       --->          GND                  GND              //电源地
//=======================================液晶屏数据线接线==========================================//
//本模块默认数据总线类型为16位并口总线
//     LCD模块             TFTLCD插槽引脚        STM32连接引脚
//     D0~D15     --->         D0~D15               PB0~PB15          //液晶屏16位并口数据信号（8位模式：DB0~DB7接D0~D7）
//=======================================液晶屏控制线接线==========================================//
//     LCD模块 			       TFTLCD插槽引脚        STM32连接引脚 
//       WR       --->          WR                   PC7             //液晶屏写数据控制信号
//       RD       --->          RD                   PC6             //液晶屏读数据控制信号
//       RS       --->          RS                   PC8             //液晶屏数据/命令控制信号
//       RST      --->          RST                  PC4             //液晶屏复位控制信号
//       CS       --->          CS                   PC9             //液晶屏片选控制信号
//       BL       --->          BL                   PC10            //液晶屏背光控制信号
//=========================================触摸屏触接线=========================================//
//如果模块不带触摸功能或者带有触摸功能，但是不需要触摸功能，则不需要进行触摸屏接线
//	   LCD模块             TFTLCD插槽引脚        STM32连接引脚 
//      INI       --->          PEN                  PC1             //触摸屏触摸中断信号
//      SDA       --->          MOSI                 PC3             //触摸屏IIC总线数据信号
//      CRST      --->          TCS                  PC13            //触摸屏复位信号
//      CLK       --->          CLK                  PC0             //触摸屏IIC总线时钟信号
**************************************************************************************************/			
#ifndef __CTPIIC_H
#define __CTPIIC_H
#include "sys.h"	    
  	   		   
//IIC IO方向设置
#define CTP_SDA_IN()  {GPIOC->CRL&=0XFFFF0FFF;GPIOC->CRL|=8<<4*3;}
#define CTP_SDA_OUT() {GPIOC->CRL&=0XFFFF0FFF;GPIOC->CRL|=3<<4*3;}

//IO操作函数	 
#define CTP_IIC_SCL    PCout(0) 			//SCL     
#define CTP_IIC_SDA    PCout(3) 			//SDA	 
#define CTP_READ_SDA   PCin(3)  			//输入SDA 

//IIC所有操作函数
void CTP_IIC_Init(void);                	//初始化IIC的IO口				 
void CTP_IIC_Start(void);				//发送IIC开始信号
void CTP_IIC_Stop(void);	  				//发送IIC停止信号
void CTP_IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 CTP_IIC_Read_Byte(unsigned char ack);	//IIC读取一个字节
u8 CTP_IIC_Wait_Ack(void); 				//IIC等待ACK信号
void CTP_IIC_Ack(void);					//IIC发送ACK信号
void CTP_IIC_NAck(void);					//IIC不发送ACK信号

#endif







