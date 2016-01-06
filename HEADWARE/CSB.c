#include "csb.h"
#include "TIME.h"

//-------------------------------------------------------------------
//超声波初始化
void CSB_Init(void)
{
	RCC->APB2ENR |= 1<<3; //使能PB；
	
	GPIOB -> CRL &= 0xFFFFFFF0;
	GPIOB -> CRL |= 0x00000003;	//PB0推挽输出，用于发送Trig波形；
	
	GPIOB -> ODR &=~(1<<0); 	//初始化PB0=0；
	
	Tim1_Init(5999,719);		//使能定时器1,10ms进一次中断，用于产生驱动超声波的驱动波形；
	
	TIM3_Init(0XFFFF,720-1); //以 1Mhz 的频率计数
}

//-------------------------------------------------------------------
//计算超声波的返回值

u16 CSB_Cal()
{
	static u16 value; 
	if(TIM3CH4_CAPTURE_STA&0X80)//成功捕获到了一次高电平
	{
		value=TIM3CH4_CAPTURE_STA&0X3F;
		value*=65536; //溢出时间总和
		value+=TIM3CH4_CAPTURE_VAL; //得到总的高电平时间
		value =(u16)(value*0.17);
		TIM3CH4_CAPTURE_STA=0; //开启下一次捕获
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
			Data_f[i]=Data_f[i+1];//所有数据左移，地位扔掉

			Sum_f += Data_f[i];//求和
		}
		
		*Dist  =  Sum_f/N;//求平均

}
