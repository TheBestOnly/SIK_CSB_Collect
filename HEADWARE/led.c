#include "led.h"
#include "sys.h"

void Led_Init(void)
{
	RCC->APB2ENR |=1<<3;	//ʹ��PORTBʱ��
		
	
//	GPIOA->CRH &=0x0FFFFFFF;
//	GPIOA->CRH |=0x30000000;	//PA15�������
//	
	GPIOB->CRH &=0xF0FFFFFF; 
	GPIOB->CRH |=0x03000000;	//PB14.15�������
	
//	GPIOA->ODR |=1<<15;			//PA15/PB3/PB4�����
	GPIOB->ODR |=1<<14;
}
