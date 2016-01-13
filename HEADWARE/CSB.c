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
u16 CSB_Val[6];
void CSB_Cal()
{
	u16 value; 
	if(TIM3CH1_CAPTURE_STA&0X80)//�ɹ�������һ�θߵ�ƽ
	{
		CSB_Val[0]=TIM3CH1_CAPTURE_STA&0X3F;
		CSB_Val[0]*=65536; //���ʱ���ܺ�
		CSB_Val[0]+=TIM3CH1_CAPTURE_VAL; //�õ��ܵĸߵ�ƽʱ��
		CSB_Val[0] =(u16)(CSB_Val[0]*0.17);
		TIM3CH1_CAPTURE_STA=0; //������һ�β���
		LED0 =~LED0;
	}
	
		if(TIM3CH2_CAPTURE_STA&0X80)//�ɹ�������һ�θߵ�ƽ
	{
		CSB_Val[1]=TIM3CH2_CAPTURE_STA&0X3F;
		CSB_Val[1]*=65536; //���ʱ���ܺ�
		CSB_Val[1]+=TIM3CH2_CAPTURE_VAL; //�õ��ܵĸߵ�ƽʱ��
		CSB_Val[1] =(u16)(CSB_Val[1]*0.17);
		TIM3CH2_CAPTURE_STA=0; //������һ�β���
		LED0 =~LED0;
	}
	
		if(TIM3CH3_CAPTURE_STA&0X80)//�ɹ�������һ�θߵ�ƽ
	{
		CSB_Val[2]=TIM3CH3_CAPTURE_STA&0X3F;
		CSB_Val[2]*=65536; //���ʱ���ܺ�
		CSB_Val[2]+=TIM3CH3_CAPTURE_VAL; //�õ��ܵĸߵ�ƽʱ��
		CSB_Val[2] =(u16)(CSB_Val[2]*0.17);
		TIM3CH3_CAPTURE_STA=0; //������һ�β���
		LED0 =~LED0;
	}
	
		if(TIM3CH4_CAPTURE_STA&0X80)//�ɹ�������һ�θߵ�ƽ
	{
		CSB_Val[3]=TIM3CH4_CAPTURE_STA&0X3F;
		CSB_Val[3]*=65536; //���ʱ���ܺ�
		CSB_Val[3]+=TIM3CH4_CAPTURE_VAL; //�õ��ܵĸߵ�ƽʱ��
		CSB_Val[3] =(u16)(CSB_Val[3]*0.17);
		TIM3CH4_CAPTURE_STA=0; //������һ�β���
		LED0 =~LED0;
	}
	
	
	CSB_Filter(CSB_Val,4) ;

}


//sum��ֵҪС��20
#define N 10
unsigned int Data_f[20][20];
void  CSB_Filter(u16 *Dist,u8 sum)
{
      unsigned char i,j;
	  unsigned int Sum_f[20];
	
		for(i=0;i<sum;i++)
		{
			Data_f[i][N-1]=*(Dist+i);
			for(j=0;j<N;j++)
			{
				Data_f[i][j]=Data_f[i][j+1];//�����������ƣ���λ�ӵ�
				Sum_f[i] += Data_f[i][j];//���
			}
			*(Dist+i)=Sum_f[i]/N;
		}
		
//		temp_f =*Dist;
//	
//		Data_f[0][N]=temp_f;
//		Sum_f=0;

//		for(j=0;j<N;i++)
//		{
//			Data_f[0][j]=Data_f[i][j+1];//�����������ƣ���λ�ӵ�

//			Sum_f += Data_f[i];//���
//		}
//		
//		*Dist  =  Sum_f/N;//��ƽ��

}
