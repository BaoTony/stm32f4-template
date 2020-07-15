/*** 
 * @Author: TonyJia
 * @Date: 2020-07-15 20:22:14
 * @LastEditTime: 2020-07-15 21:27:58
 * @LastEditors: TonyJia
 * @Description: 
 */
#ifndef _NETWORK_H
#define _NETWORK_H

#include "string.h"
#include "spi.h"
#include "w5500.h"
#include "time.h"
#include "usart.h"

#define SOCK_TCPS         0
#define SOCK_DNS          1
#define DATA_BUF_SIZE   2048

void network_init(void);

#endif

/*********************************END OF FILE**********************************/
