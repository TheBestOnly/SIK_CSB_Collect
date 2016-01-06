#ifndef __TIME_H
#define __TIME_H


#include "sys.h"
#include "CSB.h"
#include "delay.h"


extern u8 TIM3CH4_CAPTURE_STA; //输入捕获状态 
extern u16 TIM3CH4_CAPTURE_VAL;//输入捕获值



void Tim1_Init(u16 arr,u16 psc);
void TIM3_Init(u16 arr,u16 psc);



#endif
