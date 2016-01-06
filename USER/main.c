#include "delay.h"
#include "led.h"
#include "sys.h"
#include "can.h"
#include "usart.h"
#include "TIME.h"
#include "CSB.h"


//		    b=a>>8;
//			c=a&0x0ff;
//			a=(c<<8)|b;

int main()
{
	u8 mode=0;	//CAN工作模式;0,普通模式;1,环回模式
	u8 Tx_Dat[4];
	u16 temp,t;
	
	
	Stm32_Clock_Init(9);
	delay_init(72);
	Led_Init();
	CSB_Init();	
	uart_init(72,115200);	 	//串口初始化为9600
  CAN1_Init(1,8,15,15,mode);	//CAN普通模式初始化,普通模式,波特率100Kbps
	
	
	while(1)
	{
		
		LED0=~LED0;
		delay_ms(500);
		
	}
}
