/******************************************************************************
 * Copyright 2013-2014 Espressif Systems (Wuxi)
 *
 * FileName: user_main.c
 *
 * Description: entry file of user application
 *
 * Modification history:
 *     2015/1/23, v1.0 create this file.
 *******************************************************************************/

#include "osapi.h"
#include "user_interface.h"
#include "tcp_server.h"
#include "driver/uart.h"
#include "custom_defines.h"

static os_timer_t test_timer;

void ICACHE_FLASH_ATTR user_check_ip(void)
{
    struct ip_info ipconfig;

    UART_SetBaudrate(0, UART_BAUDRATE); // To prevent being modified to 76800 by SDK
    //os_printf("Entering timer callback\r\n");

    /* Disarm timer first */
    os_timer_disarm(&test_timer);

    /* Get IP info of ESP8266 station */
    wifi_get_ip_info(STATION_IF, &ipconfig);

    if ( (wifi_station_get_connect_status() == STATION_GOT_IP) && (ipconfig.ip.addr != 0) )
    {
        //os_printf("Got IP address.\r\n");

        tcpserver_init();
    }
    else
    {
        /* If haven't got IP yet, check WiFi status, if WiFi connected, check IP again later */
        uint8 wifi_station_status;
        wifi_station_status = wifi_station_get_connect_status();
        if ( wifi_station_status == STATION_WRONG_PASSWORD ||
                wifi_station_status == STATION_NO_AP_FOUND ||
                wifi_station_status == STATION_CONNECT_FAIL )
        {
            os_printf("WiFi Connect Fail!\r\n");
        }
        else
        {
            //os_printf("IP address not obtained yet, retrying...\r\n");
            os_timer_setfn(&test_timer, (os_timer_func_t *)user_check_ip, NULL);
            os_timer_arm(&test_timer, 100, 0);
        }
    }
}

void ICACHE_FLASH_ATTR wifi_station_init(void)
{
    struct station_config config = {STATION_MODE_AP_SSID, 
        STATION_MODE_AP_PASSWORD,
        STATION_MODE_AP_HAS_BSSID,
        STATION_MODE_AP_BSSID};

    char hostname[21];
    uint8 macaddr[6];

    wifi_set_opmode(STATIONAP_MODE); //Station + SoftAP
    wifi_set_opmode_current(STATION_MODE); //Station mode
    wifi_station_set_reconnect_policy(true); //Reconnect after connection lost

    wifi_get_macaddr(STATION_IF, macaddr);
    os_sprintf(hostname, DHCP_CLIENT_HOSTNAME);
    wifi_station_set_hostname(hostname);

    /* Apply the setting and connect */
    wifi_station_set_config_current(&config);

    /* Set a timer to check whether got ip from router succeed or not. */
    os_timer_disarm(&test_timer);
    os_timer_setfn(&test_timer, (os_timer_func_t *)user_check_ip, NULL);
    os_timer_arm(&test_timer, 100, 0);
}

void ICACHE_FLASH_ATTR user_rf_pre_init(void)
{
}

void user_init(void)
{
    //system_update_cpu_freq(160);
    UART_SetBaudrate(0, UART_BAUDRATE);
    wifi_station_init();
}
