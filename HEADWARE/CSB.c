#include "csb.h"
#include "TIME.h"

//-------------------------------------------------------------------
//��������ʼ��
void CSB_Init(void)
{
	RCC->APB2ENR |= 1<<3|1<<2; //ʹ��PB/PA��
	
	GPIOB -> CRH &= 0xFFFFFF00;
	GPIOB -> CRH |= 0x00000033;	//PB8/PB9������������ڷ���Trig���Σ�
	
	GPIOA -> CRL &= 0xFF00FF0F;
	GPIOA -> CRL |= 0x00330030;	//PA1/PA4/A5������������ڷ���Trig���Σ�
	
	
	GPIOB -> ODR &=~(1<<8|1<<9); 	//��ʼ��PB8/PB9=0��
	GPIOA -> ODR &=~(1<<1|1<<4|1<<5); 	//��ʼ�� PA1/PA4/PA5=0��
	
	Tim1_Init(5999,719);		//ʹ�ܶ�ʱ��1,10ms��һ���жϣ����ڲ����������������������Σ�
	
	TIM3_Init(0XFFFF,720-1); //�� 1Mhz ��Ƶ�ʼ���
}

//-------------------------------------------------------------------
//���㳬�����ķ���ֵ

u16 CSB_Cal()
{
	static u16 value; 
	if(TIM3CH4_CAPTURE_STA&0X80)//�ɹ�������һ�θߵ�ƽ
	{
		value=TIM3CH4_CAPTURE_STA&0X3F;
		value*=65536; //���ʱ���ܺ�
		value+=TIM3CH4_CAPTURE_VAL; //�õ��ܵĸߵ�ƽʱ��
		value =(u16)(value*0.17);
		TIM3CH4_CAPTURE_STA=0; //������һ�β���
		LED0 =~LED0;
	}
	CSB_Filter(&value) ;
	return value;
}


#define N 10
unsigned int Data_f[20];
void  CSB_Filter(u16 *Dist)
{
      unsigned char i;
	  unsigned int Sum_f=0;
	  unsigned int  temp_f;
	
		temp_f =*Dist;
	
		Data_f[N]=temp_f;
		Sum_f=0;

		for(i=0;i<N;i++)
		{
			Data_f[i]=Data_f[i+1];//�����������ƣ���λ�ӵ�

			Sum_f += Data_f[i];//���
		}
		
		*Dist  =  Sum_f/N;//��ƽ��

}
