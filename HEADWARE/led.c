#include "led.h"
#include "sys.h"

void Led_Init(void)
{
	RCC->APB2ENR |=1<<3;	//使能PORTB时钟
		
	
//	GPIOA->CRH &=0x0FFFFFFF;
//	GPIOA->CRH |=0x30000000;	//PA15推挽输出
//	
	GPIOB->CRH &=0xF0FFFFFF; 
	GPIOB->CRH |=0x03000000;	//PB14.15推挽输出
	
//	GPIOA->ODR |=1<<15;			//PA15/PB3/PB4输出高
	GPIOB->ODR |=1<<14;
}
