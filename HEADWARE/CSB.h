#ifndef __CSB_H__
#define __CSB_H__

#include "led.h"
#include "sys.h"

#define Trig PBout(0)

void CSB_Init(void);
u16 CSB_Cal();
void  CSB_Filter(u16 *Dist);
	
#endif
