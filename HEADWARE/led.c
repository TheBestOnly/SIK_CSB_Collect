#include "led.h"
#include "sys.h"

void Led_Init(void)
{
	RCC->APB2ENR |= 1<<2| 1<<3;	//使能PORTA时钟
	
	//关闭jtag使能
	RCC->APB2ENR|=1<<0;     //开启辅助时钟	   
	AFIO->MAPR&=0XF8FFFFFF; //清除MAPR的关闭[26:24]
	AFIO->MAPR|=0x04000000;       //设置jtag模式
	
	
	GPIOA->CRH &=0x0FFFFFFF;
	GPIOA->CRH |=0x30000000;	//PA15推挽输出
	
	GPIOB->CRL &=0xFFF00FFF; 
	GPIOB->CRL |=0x00033000;	//PB3/PB4推挽输出
	
	GPIOA->ODR |=1<<15;			//PA15/PB3/PB4输出高
	GPIOB->ODR |=1<<3|1<<4;
}
