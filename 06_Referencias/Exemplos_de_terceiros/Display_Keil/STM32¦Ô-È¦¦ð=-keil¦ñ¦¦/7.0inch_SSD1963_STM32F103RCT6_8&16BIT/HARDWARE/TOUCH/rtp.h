/************************************************************************************
//此模块可以直接插入正点原子MiniSTM32开发板TFTLCD插槽，无需手动接线
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
			
#ifndef __RTP_H__
#define __RTP_H__
#include "sys.h"

  	
//与电阻触摸屏芯片连接引脚	   
#define PEN  PCin(1)     //PC1  INT
#define DOUT PCin(2)     //PC2  MISO		 PC2--PB14
#define TDIN PCout(3)    //PC3  MOSI		 PC3--PB15
#define TCLK PCout(0)    //PC0  SCLK	   PC0--PB13
#define TCS  PCout(13)   //PC13 CS    
     
void RTP_Write_Byte(u8 num);						//向控制芯片写入一个数据
u16 RTP_Read_AD(u8 CMD);							//读取AD转换值
u16 RTP_Read_XOY(u8 xy);							//带滤波的坐标读取(X/Y)
u8 RTP_Read_XY(u16 *x,u16 *y);					//双方向读取(X+Y)
u8 RTP_Read_XY2(u16 *x,u16 *y);					//带加强滤波的双方向坐标读取
void RTP_Drow_Touch_Point(u16 x,u16 y,u16 color);//画一个坐标校准点
void RTP_Draw_Big_Point(u16 x,u16 y,u16 color);	//画一个大点
u8 RTP_Scan(u8 tp);								//扫描
void RTP_Save_Adjdata(void);						//保存校准参数
u8 RTP_Get_Adjdata(void);						//读取校准参数
void RTP_Adjust(void);							//触摸屏校准
u8 RTP_Init(void);								//初始化
																 
void RTP_Adj_Info_Show(u16 x0,u16 y0,u16 x1,u16 y1,u16 x2,u16 y2,u16 x3,u16 y3,u16 fac);//显示校准信息
																 		  
#endif
