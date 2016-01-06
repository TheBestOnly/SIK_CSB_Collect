#ifndef __LED_H__
#define __LED_H__

#include"sys.h"

#define LED0 PAout(15)
#define LED1 PBout(3)
#define LED2 PBout(4)

void Led_Init(void);

#endif
