#ifndef _UTASK_H
#define _UTASK_H

#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"

#include "FreeRTOS.h"
#include "task.h"


#define my_debug(format, ...) printf("Line:[%06d]: " format, __LINE__, ##__VA_ARGS__)

void utask_init(void);

#endif
