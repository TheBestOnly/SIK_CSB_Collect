#ifndef __TIME_H
#define __TIME_H


#include "sys.h"
#include "CSB.h"
#include "delay.h"


extern u8 TIM3CH4_CAPTURE_STA; //���벶��״̬ 
extern u16 TIM3CH4_CAPTURE_VAL;//���벶��ֵ



void Tim1_Init(u16 arr,u16 psc);
void TIM3_Init(u16 arr,u16 psc);



#endif
