#include"TIME.h"
#include "led.h"


//----------------------------------------
//TIM1定时器中断(超声波定制配置)
void Tim1_Init(u16 arr,u16 psc)
{
	RCC->APB2ENR |= 1<<11;		//使能TIM1
	TIM1->ARR=arr;				//TIM1预装值
	TIM1->PSC=psc;				//TIM1分频
	TIM1->DIER |=1<<0;			//允许更新中断
	TIM1->CR1  |=0x01;			//使能定时器1
	MY_NVIC_Init(1,3,TIM1_UP_IRQn,2);	//抢占1，子优先级3，组2
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
//TIM3用于捕获超声波的值，PWM捕获
//TIM3_4--->PB1
void TIM3_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR |= 1<<1;	//开启定时器3时钟
	RCC->APB2ENR |= 1<<3|1<<2;	//开启PB/PA时钟
	
	GPIOB ->CRL  &=0xffffff00;
	GPIOB ->CRL  |=0x00000088;   //PB1/PB0浮空输入
	GPIOB ->ODR  |=0<<1|0<<0;			//PB1/PB0下拉
	
	GPIOA ->CRL  &=0x00ffffff;
	GPIOA ->CRL  |=0x88000000;   //PA6/PA7浮空输入
	GPIOA ->ODR  |=0<<6|0<<7;			//PA6/PA7下拉
	
	
	TIM3->ARR=arr;				//TIM3预装值
	TIM3->PSC=psc;				//TIM3分频
	
	TIM3->CCMR2|=1<<0|1<<8;
	TIM3->CCMR2|=1<<4|1<<12; //IC4F=0001 配置滤波器 以 Fck_int 采样， 2 个事件后有效
	TIM3->CCMR2|=0<<3|0<<11; //IC2PS=00 配置输入分频,不分频
	
		
	TIM3->CCMR1|=1<<0|1<<8;
	TIM3->CCMR1|=1<<4|1<<12; //IC4F=0001 配置滤波器 以 Fck_int 采样， 2 个事件后有效
	TIM3->CCMR1|=0<<3|0<<11; //IC2PS=00 配置输入分频,不分频
	
	
	TIM3->CCER|=0<<13|0<<9|0<<5|0<<1; //CC4P=0 上升沿捕获
	TIM3->CCER|=1<<12|1<<8|1<<4|1<<0; //CC4E=1 允许捕获计数器的值到捕获寄存器中
	
	TIM3->DIER|=1<<4|1<<3|1<<2|1<<1; //允许捕获中断
	TIM3->DIER|=1<<0; //允许更新中断
	
	
	TIM3->CR1|=0x01; //使能定时器 3

	MY_NVIC_Init(2,0,TIM3_IRQn,2);//抢占 2，子优先级 0，组 2
	
}




u8  TIM3CH1_CAPTURE_STA=0; //输入捕获状态 
u16 TIM3CH1_CAPTURE_VAL;//输入捕获值
u8  TIM3CH2_CAPTURE_STA=0; //输入捕获状态 
u16 TIM3CH2_CAPTURE_VAL;//输入捕获值
u8  TIM3CH3_CAPTURE_STA=0; //输入捕获状态 
u16 TIM3CH3_CAPTURE_VAL;//输入捕获值
u8  TIM3CH4_CAPTURE_STA=0; //输入捕获状态 
u16 TIM3CH4_CAPTURE_VAL;//输入捕获值

//定时器 2 中断服务程序
void TIM3_IRQHandler(void)
{ 
	u16 tsr;
	tsr=TIM3->SR;
////////////////////////////////////////////////////
//通道1	
	if((TIM3CH1_CAPTURE_STA&0X80)==0)//还未成功捕获
	{
		if(tsr&0X01)//溢出
		{ 
			if(TIM3CH1_CAPTURE_STA&0X40)//已经捕获到高电平了
			{
				if((TIM3CH1_CAPTURE_STA&0X3F)==0X3F)//高电平太长了
				{
					TIM3CH1_CAPTURE_STA|=0X80;//标记成功捕获了一次
					TIM3CH1_CAPTURE_VAL=0XFFFF;
				}else TIM3CH4_CAPTURE_STA++;
			}
		}
		if(tsr&0x10)//捕获 1 发生捕获事件
		{
			if(TIM3CH1_CAPTURE_STA&0X40) //捕获到一个下降沿
			{ 
				TIM3CH1_CAPTURE_STA|=0X80; //标记成功捕获到一次高电平脉宽
				TIM3CH1_CAPTURE_VAL = TIM3->CCR1;//获取当前的捕获值.
				TIM3->CCER&=~(1<<1); //CC1P=0 设置为上升沿捕获
			}else //还未开始,第一次捕获上升沿
			{
					TIM3CH1_CAPTURE_VAL=0;
					TIM3CH1_CAPTURE_STA=0X40; //标记捕获到了上升沿
					TIM3->CNT=0; //计数器清空
					TIM3->CCER|=1<<1; //CC1P=1 设置为下降沿捕获
			} 
		} 
	}
////////////////////////////////////////////////////
//通道2
		if((TIM3CH2_CAPTURE_STA&0X80)==0)//还未成功捕获
	{
		if(tsr&0X01)//溢出
		{ 
			if(TIM3CH2_CAPTURE_STA&0X40)//已经捕获到高电平了
			{
				if((TIM3CH2_CAPTURE_STA&0X3F)==0X3F)//高电平太长了
				{
					TIM3CH2_CAPTURE_STA|=0X80;//标记成功捕获了一次
					TIM3CH2_CAPTURE_VAL=0XFFFF;
				}else TIM3CH2_CAPTURE_STA++;
			}
		}
		if(tsr&0x10)//捕获 2 发生捕获事件
		{
			if(TIM3CH2_CAPTURE_STA&0X40) //捕获到一个下降沿
			{ 
				TIM3CH2_CAPTURE_STA|=0X80; //标记成功捕获到一次高电平脉宽
				TIM3CH2_CAPTURE_VAL = TIM3->CCR2;//获取当前的捕获值.
				TIM3->CCER&=~(1<<5); //CC2P=0 设置为上升沿捕获
			}else //还未开始,第一次捕获上升沿
			{
					TIM3CH2_CAPTURE_VAL=0;
					TIM3CH2_CAPTURE_STA=0X40; //标记捕获到了上升沿
					TIM3->CNT=0; //计数器清空
					TIM3->CCER|=1<<5; //CC2P=1 设置为下降沿捕获
			} 
		} 
	}
////////////////////////////////////////////////////
//通道3	
		if((TIM3CH3_CAPTURE_STA&0X80)==0)//还未成功捕获
	{
		if(tsr&0X01)//溢出
		{ 
			if(TIM3CH3_CAPTURE_STA&0X40)//已经捕获到高电平了
			{
				if((TIM3CH3_CAPTURE_STA&0X3F)==0X3F)//高电平太长了
				{
					TIM3CH3_CAPTURE_STA|=0X80;//标记成功捕获了一次
					TIM3CH3_CAPTURE_VAL=0XFFFF;
				}else TIM3CH3_CAPTURE_STA++;
			}
		}
		if(tsr&0x10)//捕获 4 发生捕获事件
		{
			if(TIM3CH3_CAPTURE_STA&0X40) //捕获到一个下降沿
			{ 
				TIM3CH3_CAPTURE_STA|=0X80; //标记成功捕获到一次高电平脉宽
				TIM3CH3_CAPTURE_VAL = TIM3->CCR3;//获取当前的捕获值.
				TIM3->CCER&=~(1<<9); //CC3P=0 设置为上升沿捕获
			}else //还未开始,第一次捕获上升沿
			{
					TIM3CH3_CAPTURE_VAL=0;
					TIM3CH3_CAPTURE_STA=0X40; //标记捕获到了上升沿
					TIM3->CNT=0; //计数器清空
					TIM3->CCER|=1<<9; //CC3P=1 设置为下降沿捕获
			} 
		} 
	}
////////////////////////////////////////////////////
//通道4	
		if((TIM3CH4_CAPTURE_STA&0X80)==0)//还未成功捕获
	{
		if(tsr&0X01)//溢出
		{ 
			if(TIM3CH4_CAPTURE_STA&0X40)//已经捕获到高电平了
			{
				if((TIM3CH4_CAPTURE_STA&0X3F)==0X3F)//高电平太长了
				{
					TIM3CH4_CAPTURE_STA|=0X80;//标记成功捕获了一次
					TIM3CH4_CAPTURE_VAL=0XFFFF;
				}else TIM3CH4_CAPTURE_STA++;
			}
		}
		if(tsr&0x10)//捕获 4 发生捕获事件
		{
			if(TIM3CH4_CAPTURE_STA&0X40) //捕获到一个下降沿
			{ 
				TIM3CH4_CAPTURE_STA|=0X80; //标记成功捕获到一次高电平脉宽
				TIM3CH4_CAPTURE_VAL = TIM3->CCR4;//获取当前的捕获值.
				TIM3->CCER&=~(1<<13); //CC4P=0 设置为上升沿捕获
			}else //还未开始,第一次捕获上升沿
			{
					TIM3CH4_CAPTURE_VAL=0;
					TIM3CH4_CAPTURE_STA=0X40; //标记捕获到了上升沿
					TIM3->CNT=0; //计数器清空
					TIM3->CCER|=1<<13; //CC4P=1 设置为下降沿捕获
			} 
		} 
	}
	
	
	TIM3->SR=0;//清除中断标志位 
}


