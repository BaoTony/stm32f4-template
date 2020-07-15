/*
 * @Author: TonyJia
 * @Date: 2020-07-15 20:22:02
 * @LastEditTime: 2020-07-15 21:39:39
 * @LastEditors: TonyJia
 * @Description: 
 */
#include "network.h"

uint8_t gDATABUF[DATA_BUF_SIZE];
// Default Network Configuration
wiz_NetInfo gWIZNETINFO = {.mac = {0x00, 0x08, 0xdc, 0x00, 0xab, 0xcd},
                           .ip = {192, 168, 1, 123},
                           .sn = {255, 255, 255, 0},
                           .gw = {192, 168, 1, 1},
                           .dns = {61, 128, 128, 68}, //修改成自己网络的DNS
                           .dhcp = NETINFO_STATIC};
uint8_t domain_ip[4] = {115, 29, 225, 215};
uint8_t domain_name[] = "www.embed-net.com";

void platform_init()
{
    SPI_Configuration(); //spi init
    time3_init();
}

/**
 * @Name: w5500_init
 * @Fuction: 
 * @Param: None
 * @Return: None
 */
void w5500_init(void)
{
    uint8_t tmpstr[6];
    uint8_t tmp;
    uint8_t memsize[2][8] = {{2, 2, 2, 2, 2, 2, 2, 2}, {2, 2, 2, 2, 2, 2, 2, 2}};
    //Host dependent peripheral initialized
    platform_init();
    // First of all, Should register SPI callback functions implemented by user for accessing WIZCHIP
    /* Critical section callback */
    reg_wizchip_cris_cbfunc(SPI_CrisEnter, SPI_CrisExit); //注册临界区函数
                                                          /* Chip selection call back */
#if _WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_SPI_VDM_
    reg_wizchip_cs_cbfunc(SPI_CS_Select, SPI_CS_Deselect); //注册SPI片选信号函数
#elif _WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_SPI_FDM_
    reg_wizchip_cs_cbfunc(SPI_CS_Select, SPI_CS_Deselect); // CS must be tried with LOW.
#else
#if (_WIZCHIP_IO_MODE_ & _WIZCHIP_IO_MODE_SIP_) != _WIZCHIP_IO_MODE_SIP_
#error "Unknown _WIZCHIP_IO_MODE_"
#else
    reg_wizchip_cs_cbfunc(wizchip_select, wizchip_deselect);
#endif
#endif
    /* SPI Read & Write callback function */
    reg_wizchip_spi_cbfunc(SPI_ReadByte, SPI_WriteByte); //注册读写函数
    //Resets WIZCHIP by softly
    ctlwizchip(CW_RESET_WIZCHIP, (void *)NULL);
    /* WIZCHIP SOCKET Buffer initialize */
    if (ctlwizchip(CW_INIT_WIZCHIP, (void *)memsize) == -1)
    {
        printf("WIZCHIP Initialized fail.\r\n");
        while (1)
        {
            ;
        }
    }
    /* PHY link status check */
    do
    {
        if (ctlwizchip(CW_GET_PHYLINK, (void *)&tmp) == -1)
        {
            printf("Unknown PHY Link stauts.\r\n");
        }
    } while (tmp == PHY_LINK_OFF);

    ctlnetwork(CN_SET_NETINFO, (void *)&gWIZNETINFO);

    // Display Network Information
    ctlwizchip(CW_GET_ID, (void *)tmpstr);
    printf("\r\n=== %s NET CONF ===\r\n", (char *)tmpstr);
    printf("MAC: %02X:%02X:%02X:%02X:%02X:%02X\r\n", gWIZNETINFO.mac[0], gWIZNETINFO.mac[1], gWIZNETINFO.mac[2],
           gWIZNETINFO.mac[3], gWIZNETINFO.mac[4], gWIZNETINFO.mac[5]);
    printf("SIP: %d.%d.%d.%d\r\n", gWIZNETINFO.ip[0], gWIZNETINFO.ip[1], gWIZNETINFO.ip[2], gWIZNETINFO.ip[3]);
    printf("GAR: %d.%d.%d.%d\r\n", gWIZNETINFO.gw[0], gWIZNETINFO.gw[1], gWIZNETINFO.gw[2], gWIZNETINFO.gw[3]);
    printf("SUB: %d.%d.%d.%d\r\n", gWIZNETINFO.sn[0], gWIZNETINFO.sn[1], gWIZNETINFO.sn[2], gWIZNETINFO.sn[3]);
    printf("DNS: %d.%d.%d.%d\r\n", gWIZNETINFO.dns[0], gWIZNETINFO.dns[1], gWIZNETINFO.dns[2], gWIZNETINFO.dns[3]);
    printf("======================\r\n");
}

/**
 * @Name: dns_init
 * @Fuction: 
 * @Param: None
 * @Return: None
 */
void dns_init(void)
{
    int32_t ret = 0;
    /* DNS client initialization */
    DNS_init(SOCK_DNS, gDATABUF);
    Timer_Start();
    /* DNS procssing */
    if ((ret = DNS_run(gWIZNETINFO.dns, domain_name, domain_ip)) > 0)
    { // try to 1st DNS
        printf("> 1st DNS Reponsed\r\n");
    }
    else if (ret == -1)
    {
        printf("> MAX_DOMAIN_NAME is too small. Should be redefined it.\r\n");
        Timer_Stop();
        while (1)
        {
            ;
        }
    }
    else
    {
        printf("> DNS Failed\r\n");
        Timer_Stop();
        while (1)
        {
            ;
        }
    }
    //DNS解析成功
    if (ret > 0)
    {
        printf("> Translated %s to %d.%d.%d.%d\r\n", domain_name, domain_ip[0], domain_ip[1], domain_ip[2], domain_ip[3]);
    }
    Timer_Stop();
}

/**
 * @Name: network_init
 * @Fuction: 初始化网络
 * @Param: None
 * @Return: None
 */
void network_init(void)
{
    w5500_init();
    dns_init();
}

/*********************************END OF FILE**********************************/
