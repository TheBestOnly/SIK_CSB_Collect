#include "csb.h"
#include "TIME.h"

//-------------------------------------------------------------------
//超声波初始化
void CSB_Init(void)
{
	RCC->APB2ENR |= 1<<3|1<<2; //使能PB/PA；
	
	GPIOB -> CRH &= 0xFFFFFF00;
	GPIOB -> CRH |= 0x00000033;	//PB8/PB9推挽输出，用于发送Trig波形；
	
	GPIOA -> CRL &= 0xFF00FF0F;
	GPIOA -> CRL |= 0x00330030;	//PA1/PA4/A5推挽输出，用于发送Trig波形；
	
	
	GPIOB -> ODR &=~(1<<8|1<<9); 	//初始化PB8/PB9=0；
	GPIOA -> ODR &=~(1<<1|1<<4|1<<5); 	//初始化 PA1/PA4/PA5=0；
	
	Tim1_Init(5999,719);		//使能定时器1,10ms进一次中断，用于产生驱动超声波的驱动波形；
	
	TIM3_Init(0XFFFF,720-1); //以 1Mhz 的频率计数
}

//-------------------------------------------------------------------
//计算超声波的返回值
u16 CSB_Val[6];
void CSB_Cal()
{
	u16 value; 
	if(TIM3CH1_CAPTURE_STA&0X80)//成功捕获到了一次高电平
	{
		CSB_Val[0]=TIM3CH1_CAPTURE_STA&0X3F;
		CSB_Val[0]*=65536; //溢出时间总和
		CSB_Val[0]+=TIM3CH1_CAPTURE_VAL; //得到总的高电平时间
		CSB_Val[0] =(u16)(CSB_Val[0]*0.17);
		TIM3CH1_CAPTURE_STA=0; //开启下一次捕获
		LED0 =~LED0;
	}
	
		if(TIM3CH2_CAPTURE_STA&0X80)//成功捕获到了一次高电平
	{
		CSB_Val[1]=TIM3CH2_CAPTURE_STA&0X3F;
		CSB_Val[1]*=65536; //溢出时间总和
		CSB_Val[1]+=TIM3CH2_CAPTURE_VAL; //得到总的高电平时间
		CSB_Val[1] =(u16)(CSB_Val[1]*0.17);
		TIM3CH2_CAPTURE_STA=0; //开启下一次捕获
		LED0 =~LED0;
	}
	
		if(TIM3CH3_CAPTURE_STA&0X80)//成功捕获到了一次高电平
	{
		CSB_Val[2]=TIM3CH3_CAPTURE_STA&0X3F;
		CSB_Val[2]*=65536; //溢出时间总和
		CSB_Val[2]+=TIM3CH3_CAPTURE_VAL; //得到总的高电平时间
		CSB_Val[2] =(u16)(CSB_Val[2]*0.17);
		TIM3CH3_CAPTURE_STA=0; //开启下一次捕获
		LED0 =~LED0;
	}
	
		if(TIM3CH4_CAPTURE_STA&0X80)//成功捕获到了一次高电平
	{
		CSB_Val[3]=TIM3CH4_CAPTURE_STA&0X3F;
		CSB_Val[3]*=65536; //溢出时间总和
		CSB_Val[3]+=TIM3CH4_CAPTURE_VAL; //得到总的高电平时间
		CSB_Val[3] =(u16)(CSB_Val[3]*0.17);
		TIM3CH4_CAPTURE_STA=0; //开启下一次捕获
		LED0 =~LED0;
	}
	
	
	CSB_Filter(CSB_Val,4) ;

}


//sum的值要小于20
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
				Data_f[i][j]=Data_f[i][j+1];//所有数据左移，地位扔掉
				Sum_f[i] += Data_f[i][j];//求和
			}
			*(Dist+i)=Sum_f[i]/N;
		}
		
//		temp_f =*Dist;
//	
//		Data_f[0][N]=temp_f;
//		Sum_f=0;

//		for(j=0;j<N;i++)
//		{
//			Data_f[0][j]=Data_f[i][j+1];//所有数据左移，地位扔掉

//			Sum_f += Data_f[i];//求和
//		}
//		
//		*Dist  =  Sum_f/N;//求平均

}
