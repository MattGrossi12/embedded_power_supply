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
//     LCD模块 				     TFTLCD插槽引脚        STM32连接引脚 
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
#include "lcd.h"
#include "stdlib.h"
#include "delay.h"	 

	   
//管理LCD重要参数
//默认为竖屏
_lcd_dev lcddev;

//画笔颜色,背景颜色
u16 POINT_COLOR = 0x0000,BACK_COLOR = 0xFFFF;  
u16 DeviceCode;	 

void LCD_write(u16 VAL)
{
	LCD_CS_CLR;  
	DATAOUT(VAL);
	LCD_WR_CLR; 
	LCD_WR_SET; 
	LCD_CS_SET;
}

u16 LCD_read(void)
{
	u16 data;
	LCD_CS_CLR;
	LCD_RD_CLR;
	delay_us(1);//延时1us	
	data = DATAIN;
	LCD_RD_SET;
	LCD_CS_SET;
	return data;
}

/*****************************************************************************
 * @name       :void LCD_WR_REG(u16 data)
 * @date       :2018-08-09 
 * @function   :Write an 16-bit command to the LCD screen
 * @parameters :data:Command value to be written
 * @retvalue   :None
******************************************************************************/
void LCD_WR_REG(u16 data)
{ 
   LCD_RS_CLR;     
	 LCD_write(data);
}

/*****************************************************************************
 * @name       :void LCD_WR_DATA(u16 data)
 * @date       :2018-08-09 
 * @function   :Write an 16-bit data to the LCD screen
 * @parameters :data:data value to be written
 * @retvalue   :None
******************************************************************************/
void LCD_WR_DATA(u16 data)
{
	 LCD_RS_SET;
	 LCD_write(data);
}

/*****************************************************************************
 * @name       :u16 LCD_RD_DATA(void)
 * @date       :2018-11-13 
 * @function   :Read an 16-bit value from the LCD screen
 * @parameters :None
 * @retvalue   :read value
******************************************************************************/
u16 LCD_RD_DATA(void)
{
	LCD_RS_SET; 
	return LCD_read();
}

/*****************************************************************************
 * @name       :void LCD_WriteReg(u16 LCD_Reg, u16 LCD_RegValue)
 * @date       :2018-08-09 
 * @function   :Write data into registers
 * @parameters :LCD_Reg:Register address
                LCD_RegValue:Data to be written
 * @retvalue   :None
******************************************************************************/
void LCD_WriteReg(u16 LCD_Reg, u16 LCD_RegValue)
{	
	LCD_WR_REG(LCD_Reg);  
	LCD_WR_DATA(LCD_RegValue);	    		 
}	   

/*****************************************************************************
 * @name       :u16 LCD_ReadReg(u16 LCD_Reg)
 * @date       :2018-11-13 
 * @function   :read value from specially registers
 * @parameters :LCD_Reg:Register address
 * @retvalue   :read value
******************************************************************************/
void LCD_ReadReg(u16 LCD_Reg,u8 *Rval,int n)
{
	LCD_WR_REG(LCD_Reg); 
	GPIOB->CRL=0X88888888; //PB0-7  上拉输入
	GPIOB->CRH=0X88888888; //PB8-15 上拉输入
	GPIOB->ODR=0XFFFF;     //全部输出高
	while(n--)
	{		
		*(Rval++) = LCD_RD_DATA();
	}
	
	GPIOB->CRL=0X33333333; 		//PB0-7  上拉输出
	GPIOB->CRH=0X33333333; 		//PB8-15 上拉输出
	GPIOB->ODR=0XFFFF;    		//全部输出高  
}

/*****************************************************************************
 * @name       :void LCD_WriteRAM_Prepare(void)
 * @date       :2018-08-09 
 * @function   :Write GRAM
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	 
void LCD_WriteRAM_Prepare(void)
{
	LCD_WR_REG(lcddev.wramcmd);
}	 

/*****************************************************************************
 * @name       :void LCD_ReadRAM_Prepare(void)
 * @date       :2018-11-13 
 * @function   :Read GRAM
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	 
void LCD_ReadRAM_Prepare(void)
{
	LCD_WR_REG(lcddev.rramcmd);
}

/*****************************************************************************
 * @name       :void Lcd_WriteData_16Bit(u16 Data)
 * @date       :2018-08-09 
 * @function   :Write an 16-bit command to the LCD screen
 * @parameters :Data:Data to be written
 * @retvalue   :None
******************************************************************************/	 
void Lcd_WriteData_16Bit(u16 Data)
{	
   LCD_RS_SET; 
#if LCD_USE8BIT_MODEL
		LCD_CS_CLR;
		DATAOUT((Data>>8)&0xF8);
		LCD_WR_CLR; 
		LCD_WR_SET;
		DATAOUT((Data>>3)&0xFC);
		LCD_WR_CLR; 
		LCD_WR_SET;
		DATAOUT(Data<<3);
		LCD_WR_CLR; 
		LCD_WR_SET;
		LCD_CS_SET;
#else
	 LCD_write(Data);
#endif
}

u16 Color_To_565(u8 r, u8 g, u8 b)
{
	return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3);
}

/*****************************************************************************
 * @name       :u16 Lcd_ReadData_16Bit(void)
 * @date       :2018-11-13 
 * @function   :Read an 16-bit value from the LCD screen
 * @parameters :None
 * @retvalue   :read value
******************************************************************************/	
u16 Lcd_ReadData_16Bit(void)
{
	u16 r=0;
#if LCD_USE8BIT_MODEL	
	u16 g=0,b=0;
#endif	
	LCD_RS_SET;
	LCD_CS_CLR;	
	//16bit: red green and blue data
	//8bit:red data
	LCD_RD_CLR;
	delay_us(1);//延时1us	
	r = DATAIN;
	LCD_RD_SET;
#if LCD_USE8BIT_MODEL		
	//8bit:green data
	LCD_RD_CLR;
	delay_us(1);//延时1us	
	g = DATAIN;
	LCD_RD_SET;
	
	//8bit:blue data
	LCD_RD_CLR;
	delay_us(1);//延时1us	
	b = DATAIN;
	LCD_RD_SET;	
	r=Color_To_565(r, g, b);
#endif
	LCD_CS_SET;
	return r;
}

/*****************************************************************************
 * @name       :void LCD_DrawPoint(u16 x,u16 y)
 * @date       :2018-08-09 
 * @function   :Write a pixel data at a specified location
 * @parameters :x:the x coordinate of the pixel
                y:the y coordinate of the pixel
 * @retvalue   :None
******************************************************************************/	
void LCD_DrawPoint(u16 x,u16 y)
{
	LCD_SetCursor(x,y);//设置光标位置 
	Lcd_WriteData_16Bit(POINT_COLOR); 
}

/*****************************************************************************
 * @name       :u16 LCD_ReadPoint(u16 x,u16 y)
 * @date       :2018-11-13 
 * @function   :Read a pixel color value at a specified location
 * @parameters :x:the x coordinate of the pixel
                y:the y coordinate of the pixel
 * @retvalue   :the read color value
******************************************************************************/	
u16 LCD_ReadPoint(u16 x,u16 y)
{
	u16 color;
	if(x>=lcddev.width||y>=lcddev.height)
	{
		return 0;	//超过了范围,直接返回	
	}
	LCD_SetCursor(x,y);//设置光标位置 
	LCD_ReadRAM_Prepare();
	GPIOB->CRL=0X88888888; //PB0-7  上拉输入
	GPIOB->CRH=0X88888888; //PB8-15 上拉输入
	GPIOB->ODR=0XFFFF;     //全部输出高
	
	color = Lcd_ReadData_16Bit();
	
	GPIOB->CRL=0X33333333; 		//PB0-7  上拉输出
	GPIOB->CRH=0X33333333; 		//PB8-15 上拉输出
	GPIOB->ODR=0XFFFF;    		//全部输出高  
	return color;
}

/*****************************************************************************
 * @name       :void LCD_Clear(u16 Color)
 * @date       :2018-08-09 
 * @function   :Full screen filled LCD screen
 * @parameters :color:Filled color
 * @retvalue   :None
******************************************************************************/	
void LCD_Clear(u16 Color)
{
  unsigned int i;//,m;  
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);
	LCD_RS_SET; 
	LCD_CS_CLR;
	for(i=0;i<lcddev.height*lcddev.width;i++)
	{
#if LCD_USE8BIT_MODEL
		DATAOUT((Color>>8)&0xF8);
		LCD_WR_CLR; 
		LCD_WR_SET;
		DATAOUT((Color>>3)&0xFC);
		LCD_WR_CLR; 
		LCD_WR_SET;
		DATAOUT(Color<<3);
		LCD_WR_CLR; 
		LCD_WR_SET;
#else
	 	DATAOUT(Color);
		LCD_WR_CLR; 
		LCD_WR_SET;
#endif
	}
	LCD_CS_SET;
} 

/*****************************************************************************
 * @name       :void LCD_PWM_BackLightSet(u8 pwm)
 * @date       :2020-05-13
 * @function   :Set the backlight brightness from PWM
 * @parameters :pwm:pwm value 0~100, The greater the value, the brighter
 * @retvalue   :None
******************************************************************************/
void LCD_PWM_BackLightSet(u8 pwm)
{	
	LCD_WR_REG(0xBE);	//配置PWM输出
	LCD_WR_DATA(0x05);	//1设置PWM频率
	LCD_WR_DATA(pwm*2.55);//2设置PWM占空比
	LCD_WR_DATA(0x01);	//3设置C
	LCD_WR_DATA(0xFF);	//4设置D
	LCD_WR_DATA(0x00);	//5设置E
	LCD_WR_DATA(0x00);	//6设置F
}

/*****************************************************************************
 * @name       :void LCD_GPIOInit(void)
 * @date       :2018-08-09 
 * @function   :Initialization LCD screen GPIO
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	
void LCD_GPIOInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	      
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10| GPIO_Pin_7| GPIO_Pin_8| GPIO_Pin_9 | GPIO_Pin_6 | GPIO_Pin_4; //GPIOC10,6,7,8,9,4
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //推挽输出
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	GPIO_SetBits(GPIOC,GPIO_Pin_10|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_6|GPIO_Pin_4);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;	//  
	GPIO_Init(GPIOB, &GPIO_InitStructure); //GPIOB
	GPIO_SetBits(GPIOB,GPIO_Pin_All);	
}

/*****************************************************************************
 * @name       :void LCD_RESET(void)
 * @date       :2018-08-09 
 * @function   :Reset LCD screen
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	
void LCD_RESET(void)
{
	LCD_RST_CLR;
	delay_ms(100);	
	LCD_RST_SET;
	delay_ms(50);
}

/*****************************************************************************
 * @name       :void LCD_Init(void)
 * @date       :2018-08-09 
 * @function   :Initialization LCD screen
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	 	 
void LCD_Init(void)
{  
	LCD_GPIOInit();//LCD GPIO初始化	
 	LCD_RESET(); //LCD 复位
//************* SSD1963初始化**********//	
	LCD_WR_REG(0xE2);		//Set PLL with OSC = 10MHz (hardware),	Multiplier N = 35, 250MHz < VCO < 800MHz = OSC*(N+1), VCO = 300MHz
	LCD_WR_DATA(0x1D);		//参数1 
	LCD_WR_DATA(0x02);		//参数2 Divider M = 2, PLL = 300/(M+1) = 100MHz
	LCD_WR_DATA(0x04);		//参数3 Validate M and N values   
	delay_us(100);
	LCD_WR_REG(0xE0);		// Start PLL command
	LCD_WR_DATA(0x01);		// enable PLL
	delay_ms(10);
	LCD_WR_REG(0xE0);		// Start PLL command again
	LCD_WR_DATA(0x03);		// now, use PLL output as system clock	
	delay_ms(12);  
	LCD_WR_REG(0x01);		//软复位
	delay_ms(10);
		
	LCD_WR_REG(0xE6);		//设置像素频率,33Mhz
	LCD_WR_DATA(0x03);
	LCD_WR_DATA(0xFF);
	LCD_WR_DATA(0xFF);
		
	LCD_WR_REG(0xB0);		//设置LCD模式
	LCD_WR_DATA(0x00);		//18位模式
	LCD_WR_DATA(0x00);		//TFT 模式 
	
	LCD_WR_DATA((SSD_HOR_RESOLUTION-1)>>8);//设置LCD水平像素
	LCD_WR_DATA(SSD_HOR_RESOLUTION-1);		 
	LCD_WR_DATA((SSD_VER_RESOLUTION-1)>>8);//设置LCD垂直像素
	LCD_WR_DATA(SSD_VER_RESOLUTION-1);		 
	LCD_WR_DATA(0x00);		//RGB序列 
		
	LCD_WR_REG(0xB4);		//Set horizontal period
	LCD_WR_DATA((SSD_HT-1)>>8);
	LCD_WR_DATA(SSD_HT-1);
	LCD_WR_DATA(SSD_HPS>>8);
	LCD_WR_DATA(SSD_HPS);
	LCD_WR_DATA(SSD_HOR_PULSE_WIDTH-1);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_REG(0xB6);		//Set vertical period
	LCD_WR_DATA((SSD_VT-1)>>8);
	LCD_WR_DATA(SSD_VT-1);
	LCD_WR_DATA(SSD_VPS>>8);
	LCD_WR_DATA(SSD_VPS);
	LCD_WR_DATA(SSD_VER_FRONT_PORCH-1);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
		
	LCD_WR_REG(0xF0);	//设置SSD1963与CPU接口为16bit  
#if LCD_USE8BIT_MODEL 
	LCD_WR_DATA(0x00);	//8-bit(565 format) data for 16bpp 
#else
	LCD_WR_DATA(0x03);	//16-bit(565 format) data for 16bpp 
#endif
	LCD_WR_REG(0x29);	//开启显示
	//设置PWM输出  背光通过占空比可调 
	LCD_WR_REG(0xD0);	//设置自动白平衡DBC
	LCD_WR_DATA(0x00);	//disable
	
	LCD_WR_REG(0xBE);	//配置PWM输出
	LCD_WR_DATA(0x05);	//1设置PWM频率
	LCD_WR_DATA(0xFE);	//2设置PWM占空比
	LCD_WR_DATA(0x01);	//3设置C
	LCD_WR_DATA(0x00);	//4设置D
	LCD_WR_DATA(0x00);	//5设置E 
	LCD_WR_DATA(0x00);	//6设置F 
		
	LCD_WR_REG(0xB8);	//设置GPIO配置
	LCD_WR_DATA(0x07);	//2个IO口设置成输出
	LCD_WR_DATA(0x01);	//GPIO使用正常的IO功能 
	LCD_WR_REG(0xBA);
	LCD_WR_DATA(0X01);	//GPIO[1:0]=01,控制LCD方向
		
	LCD_PWM_BackLightSet(100);//背光设置为最亮

  LCD_direction(USE_HORIZONTAL);//设置LCD显示方向
	LCD_LED=1;//点亮背光	 
	LCD_Clear(WHITE);//清全屏白色
}
 
/*****************************************************************************
 * @name       :void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd)
 * @date       :2018-08-09 
 * @function   :Setting LCD display window
 * @parameters :xStar:the bebinning x coordinate of the LCD display window
								yStar:the bebinning y coordinate of the LCD display window
								xEnd:the endning x coordinate of the LCD display window
								yEnd:the endning y coordinate of the LCD display window
 * @retvalue   :None
******************************************************************************/ 
void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd)
{	
	switch(lcddev.dir)
	{
		case 0:
			xStar=xStar+xEnd;
			xEnd=xStar-xEnd;
			xStar=xStar-xEnd;
			xStar=lcddev.width-xStar-1;
			xEnd=lcddev.width-xEnd-1;
		break;
		case 2:
			yStar=yStar+yEnd;
			yEnd=yStar-yEnd;
			yStar=yStar-yEnd;
			yStar=lcddev.height-yStar-1;
			yEnd=lcddev.height-yEnd-1;
		break;
		case 3:
			xStar=xStar+xEnd;
			xEnd=xStar-xEnd;
			xStar=xStar-xEnd;
			xStar=lcddev.width-xStar-1;
			xEnd=lcddev.width-xEnd-1;
			yStar=yStar+yEnd;
			yEnd=yStar-yEnd;
			yStar=yStar-yEnd;
			yStar=lcddev.height-yStar-1;
			yEnd=lcddev.height-yEnd-1;
		break;
		default:
		break;
	}
	LCD_WR_REG(lcddev.setxcmd);	
	LCD_WR_DATA(xStar>>8);
	LCD_WR_DATA(0x00FF&xStar);		
	LCD_WR_DATA(xEnd>>8);
	LCD_WR_DATA(0x00FF&xEnd);

	LCD_WR_REG(lcddev.setycmd);	
	LCD_WR_DATA(yStar>>8);
	LCD_WR_DATA(0x00FF&yStar);		
	LCD_WR_DATA(yEnd>>8);
	LCD_WR_DATA(0x00FF&yEnd);
	LCD_WriteRAM_Prepare();	//开始写入GRAM			
}   

/*****************************************************************************
 * @name       :void LCD_SetCursor(u16 Xpos, u16 Ypos)
 * @date       :2018-08-09 
 * @function   :Set coordinate value
 * @parameters :Xpos:the  x coordinate of the pixel
								Ypos:the  y coordinate of the pixel
 * @retvalue   :None
******************************************************************************/ 
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{	  	    			
	LCD_SetWindows(Xpos,Ypos,Xpos,Ypos);	
} 

/*****************************************************************************
 * @name       :void LCD_direction(u8 direction)
 * @date       :2018-08-09 
 * @function   :Setting the display direction of LCD screen
 * @parameters :direction:0-0 degree
                          1-90 degree
													2-180 degree
													3-270 degree
 * @retvalue   :None
******************************************************************************/ 
void LCD_direction(u8 direction)
{ 
	lcddev.wramcmd=0x2C;
	lcddev.rramcmd=0x2E;
	lcddev.dir=direction%4;
	switch(lcddev.dir){		  
		case 0:	
			lcddev.setxcmd=0x2B;
			lcddev.setycmd=0x2A;
			lcddev.width=LCD_W;
			lcddev.height=LCD_H;		
			LCD_WriteReg(0x36,(0<<3)|(1<<7)|(0<<6)|(1<<5));
		break;
		case 1:
			lcddev.setxcmd=0x2A;
			lcddev.setycmd=0x2B;
			lcddev.width=LCD_H;
			lcddev.height=LCD_W;
			LCD_WriteReg(0x36,(0<<3)|(0<<7)|(0<<6)|(0<<5));
		break;
		case 2:
			lcddev.setxcmd=0x2B;
			lcddev.setycmd=0x2A;		
			lcddev.width=LCD_W;
			lcddev.height=LCD_H;	
			LCD_WriteReg(0x36,(0<<3)|(0<<7)|(1<<6)|(1<<5));
		break;
		case 3:
			lcddev.setxcmd=0x2A;
			lcddev.setycmd=0x2B;
			lcddev.width=LCD_H;
			lcddev.height=LCD_W;
			LCD_WriteReg(0x36,(0<<3)|(1<<7)|(1<<6)|(0<<5));
		break;	
		default:break;
	}		
}	 

/*****************************************************************************
 * @name       :u16 LCD_Read_ID(void)
 * @date       :2018-11-13 
 * @function   :Read ID
 * @parameters :None
 * @retvalue   :ID value
******************************************************************************/ 
u16 LCD_Read_ID(void)
{
	u8 val[3] = {0};
	LCD_ReadReg(0xA1,val,3);
	return (val[1]<<8)|val[2];
}
