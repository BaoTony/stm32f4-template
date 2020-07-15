/*
 * @Author: TonyJia
 * @Date: 2020-07-13 20:19:48
 * @LastEditTime: 2020-07-15 20:18:19
 * @LastEditors: TonyJia
 * @Description: config spi
 */
/* Includes ------------------------------------------------------------------*/
#include "spi.h"

/**
 * @Name: SPI_RCC_Configuration
 * @Fuction: 使能SPI时钟
 * @Param: None
 * @Return: None
 */
static void SPI_RCC_Configuration(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); //使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);  //使能SPI1时钟
}

/**
 * @Name: SPI_GPIO_Configuration
 * @Fuction: 配置指定SPI的引脚
 * @Param: None
 * @Return: None
 */
static void SPI_GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1); //PA5复用SPI1
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1); //PA6复用SPI1
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1); //PA7复用SPI1

	//GPIOA5,6,7初始化设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7; //PB5~7复用功能输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;						//复用功能
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;						//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;					//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;						//上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);								//初始化

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	   //输出功能
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	   //上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);			   //初始化
	GPIO_SetBits(GPIOA, GPIO_Pin_4);
}

/**
 * @Name: SPI_Configuration
 * @Fuction: 根据外部SPI设备配置SPI相关参数
 * @Param: None
 * @Return: None
 */
void SPI_Configuration(void)
{
	SPI_InitTypeDef SPI_InitStructure;
	SPI_RCC_Configuration();

	//这里只针对SPI口初始化
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1, ENABLE);  //复位SPI1
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1, DISABLE); //停止复位SPI1

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;					   //设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;				   //设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;						   //串行同步时钟的空闲状态为低电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;					   //串行同步时钟的第一个跳变沿（上升或下降）数据被采样
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;						   //NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2; //定义波特率预分频的值:APB2=84Mhz,波特率预分频值为2,即42Mhz
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;				   //指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;						   //CRC值计算的多项式
	SPI_Init(SPI1, &SPI_InitStructure);								   //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器

	SPI_GPIO_Configuration();
	SPI_SSOutputCmd(SPI1, ENABLE);
	SPI_Cmd(SPI1, ENABLE); //使能SPI外设
}

/**
 * @Name: SPI_WriteByte
 * @Fuction: 写1字节数据到SPI总线
 * @Param: TxData 写到总线的数据
 * @Return: None
 */
void SPI_WriteByte(uint8_t TxData)
{
	while ((SPI1->SR & SPI_I2S_FLAG_TXE) == 0)
	{
		; //等待发送区空
	}
	SPI1->DR = TxData; //发送一个byte
	while ((SPI1->SR & SPI_I2S_FLAG_RXNE) == 0)
	{
		; //等待接收完一个byte
	}
	SPI1->DR;
}

/**
 * @Name: SPI_ReadByte
 * @Fuction: 从SPI总线读取1字节数据
 * @Param: None
 * @Return: 读到的数据
 */
uint8_t SPI_ReadByte(void)
{
	while ((SPI1->SR & SPI_I2S_FLAG_TXE) == 0)
	{
		; //等待发送区空
	}
	SPI1->DR = 0xFF; //发送一个空数据产生输入数据的时钟
	while ((SPI1->SR & SPI_I2S_FLAG_RXNE) == 0)
	{
		; //等待接收完一个byte
	}
	return SPI1->DR;
}

/**
 * @Name: SPI_CrisEnter
 * @Fuction: 进入临界区
 * @Param: None
 * @Return: None
 */
void SPI_CrisEnter(void)
{
	__set_PRIMASK(1);
}

/**
 * @Name: SPI_CrisExit
 * @Fuction: 退出临界区
 * @Param: None
 * @Return: None
 */
void SPI_CrisExit(void)
{
	__set_PRIMASK(0);
}

/**
 * @Name: SPI_CS_Select
 * @Fuction: 片选信号输出低电平
 * @Param: None
 * @Return: None
 */
void SPI_CS_Select(void)
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);
}

/**
 * @Name: SPI_CS_Deselect
 * @Fuction: 片选信号输出高电平
 * @Param: None
 * @Return: None
 */
void SPI_CS_Deselect(void)
{
	GPIO_SetBits(GPIOA, GPIO_Pin_4);
}

/*********************************END OF FILE**********************************/
