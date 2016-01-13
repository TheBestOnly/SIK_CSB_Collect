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
	u8 mode=0;	//CAN����ģʽ;0,��ͨģʽ;1,����ģʽ
	u16 Tx_Dat[4]={213,235,55,33};
	u16 temp,t;
	
	
	Stm32_Clock_Init(9);
	delay_init(72);
	Led_Init();
	CSB_Init();	
	uart_init(72,115200);	 	//���ڳ�ʼ��Ϊ9600
    CAN1_Init(1,8,15,15,mode);	//CAN��ͨģʽ��ʼ��,��ͨģʽ,������100Kbps
	
	
	while(1)
	{
		CSB_Cal();
	//	LED0=~LED0;
		delay_ms(500);
		printf("asd");
		
	}
}
