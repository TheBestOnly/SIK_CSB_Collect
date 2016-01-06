#include "led.h"
#include "sys.h"

void Led_Init(void)
{
	RCC->APB2ENR |= 1<<2| 1<<3;	//ʹ��PORTAʱ��
	
	//�ر�jtagʹ��
	RCC->APB2ENR|=1<<0;     //��������ʱ��	   
	AFIO->MAPR&=0XF8FFFFFF; //���MAPR�Ĺر�[26:24]
	AFIO->MAPR|=0x04000000;       //����jtagģʽ
	
	
	GPIOA->CRH &=0x0FFFFFFF;
	GPIOA->CRH |=0x30000000;	//PA15�������
	
	GPIOB->CRL &=0xFFF00FFF; 
	GPIOB->CRL |=0x00033000;	//PB3/PB4�������
	
	GPIOA->ODR |=1<<15;			//PA15/PB3/PB4�����
	GPIOB->ODR |=1<<3|1<<4;
}
