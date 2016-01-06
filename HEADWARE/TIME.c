#include"TIME.h"
#include "led.h"


//----------------------------------------
//TIM1��ʱ���ж�(��������������)
void Tim1_Init(u16 arr,u16 psc)
{
	RCC->APB2ENR |= 1<<11;		//ʹ��TIM1
	TIM1->ARR=arr;				//TIM1Ԥװֵ
	TIM1->PSC=psc;				//TIM1��Ƶ
	TIM1->DIER |=1<<0;			//��������ж�
	TIM1->CR1  |=0x01;			//ʹ�ܶ�ʱ��1
	MY_NVIC_Init(1,3,TIM1_UP_IRQn,2);	//��ռ1�������ȼ�3����2
}	

void TIM1_UP_IRQHandler(void)
{
	if(TIM1->SR & 0x0001)
	{
		Trig1=1;
		Trig2=1;
		Trig3=1;
		Trig4=1;
		Trig5=1;
		delay_us(12);
		Trig1=0;
		Trig2=0;
		Trig3=0;
		Trig4=0;
		Trig5=0;
//		Trig =~Trig;
	}
	
		TIM1->SR &=~(1<<0);
}

//--------------------------------------------


//--------------------------------------------
//TIM3���ڲ���������ֵ��PWM����
//TIM3_4--->PB1
void TIM3_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR |= 1<<1;	//������ʱ��3ʱ��
	RCC->APB2ENR |= 1<<3;	//����PBʱ��
	
	GPIOB ->CRL  &=0xffffff0f;
	GPIOB ->CRL  |=0x00000080;   //PB1��������
	GPIOB ->ODR |=0<<1;			//PB1����
	
	TIM3->ARR=arr;				//TIM1Ԥװֵ
	TIM3->PSC=psc;				//TIM1��Ƶ
	TIM3->CCMR2 |=1<<8;
	TIM3->CCMR2|=1<<12; //IC4F=0001 �����˲��� �� Fck_int ������ 2 ���¼�����Ч
	TIM3->CCMR2|=0<<11; //IC2PS=00 ���������Ƶ,����Ƶ
	TIM3->CCER|=0<<13; //CC4P=0 �����ز���
	TIM3->CCER|=1<<12; //CC4E=1 �������������ֵ������Ĵ�����
	TIM3->DIER|=1<<4; //�������ж�
	TIM3->DIER|=1<<0; //��������ж�
	TIM3->CR1|=0x01; //ʹ�ܶ�ʱ�� 3
	MY_NVIC_Init(2,0,TIM3_IRQn,2);//��ռ 2�������ȼ� 0���� 2
	
}




u8  TIM3CH4_CAPTURE_STA=0; //���벶��״̬ 
u16 TIM3CH4_CAPTURE_VAL;//���벶��ֵ
//��ʱ�� 2 �жϷ������
void TIM3_IRQHandler(void)
{ 
	u16 tsr;
	tsr=TIM3->SR;
	if((TIM3CH4_CAPTURE_STA&0X80)==0)//��δ�ɹ�����
	{
		if(tsr&0X01)//���
		{ 
			if(TIM3CH4_CAPTURE_STA&0X40)//�Ѿ����񵽸ߵ�ƽ��
			{
				if((TIM3CH4_CAPTURE_STA&0X3F)==0X3F)//�ߵ�ƽ̫����
				{
					TIM3CH4_CAPTURE_STA|=0X80;//��ǳɹ�������һ��
					TIM3CH4_CAPTURE_VAL=0XFFFF;
				}else TIM3CH4_CAPTURE_STA++;
			}
		}
		if(tsr&0x10)//���� 4 ���������¼�
		{
			if(TIM3CH4_CAPTURE_STA&0X40) //����һ���½���
			{ 
				TIM3CH4_CAPTURE_STA|=0X80; //��ǳɹ�����һ�θߵ�ƽ����
				TIM3CH4_CAPTURE_VAL = TIM3->CCR4;//��ȡ��ǰ�Ĳ���ֵ.
				TIM3->CCER&=~(1<<13); //CC4P=0 ����Ϊ�����ز���
			}else //��δ��ʼ,��һ�β���������
			{
					TIM3CH4_CAPTURE_VAL=0;
					TIM3CH4_CAPTURE_STA=0X40; //��ǲ�����������
					TIM3->CNT=0; //���������
					TIM3->CCER|=1<<13; //CC4P=1 ����Ϊ�½��ز���
			} 
		} 
	}
	TIM3->SR=0;//����жϱ�־λ 
}


