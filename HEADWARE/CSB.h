#ifndef __CSB_H__
#define __CSB_H__

#include "led.h"
#include "sys.h"

#define Trig1 PAout(4)
#define Trig2 PAout(5)
#define Trig3 PBout(8)
#define Trig4 PBout(9)
#define Trig5 PAout(1)

void CSB_Init(void);
u16 CSB_Cal();
void  CSB_Filter(u16 *Dist);
	
#endif
