/*** 
 * @Author: TonyJia
 * @Date: 2020-07-13 20:19:59
 * @LastEditTime: 2020-07-15 20:05:22
 * @LastEditors: TonyJia
 * @Description: spi header
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SPI_H
#define __SPI_H

/* Includes ------------------------------------------------------------------*/
#include "sys.h"

/* Exported Functions --------------------------------------------------------*/

void SPI_Configuration(void);
void SPI_WriteByte(uint8_t TxData);
uint8_t SPI_ReadByte(void);
void SPI_CrisEnter(void);
void SPI_CrisExit(void);
void SPI_CS_Select(void);
void SPI_CS_Deselect(void);

#endif /* __SPI_H */

/*********************************END OF FILE**********************************/
