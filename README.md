# esp8266-ptpd
IEEE-1588-2008 implementation on ESP8266, ported from [PTPd](http://ptpd.sourceforge.net/) 2.3.1.

Currently based on [ESP non-OS SDK v1.3.0](http://bbs.espressif.com/viewtopic.php?f=46&t=919), with [lwIP library](http://bbs.espressif.com/viewtopic.php?f=46&t=951) released by [Espressif](http://espressif.com/).

Sample `./app/include/custom_defines.h` :

    #ifndef __CUSTOM_DEFINES_H__
    #define __CUSTOM_DEFINES_H__
    
    #define UART_BAUDRATE   921600
    
    #define STATION_MODE_AP_SSID        "SensorNetwork"
    #define STATION_MODE_AP_PASSWORD    "123456"
    #define STATION_MODE_AP_HAS_BSSID   0
    #define STATION_MODE_AP_BSSID       {0, 0, 0, 0, 0, 0}
    
    #define DHCP_CLIENT_HOSTNAME    "Node-%02X%02X%02X", macaddr[3], macaddr[4], macaddr[5]
    
    #define OTA_UPGRADE_COMMAND     "update\n"
    #define TCP_SERVER_LOCAL_PORT   4321
    
    #define OTA_UPGRADE_HTTP_REQUEST "GET /ota/%s HTTP/1.0\r\nHost: "IPSTR":%d\r\nConnection: keep-alive\r\nCache-Control: no-cache\r\n\r\n",\
                    user_bin,\
                    IP2STR(upgrade_server.ip),\
                    upgrade_server.port
    
    /* 31 characters max */
    #define OTA_USER1_BIN_FILENAME  "user1.1024.new.2.bin"
    #define OTA_USER2_BIN_FILENAME  "user2.1024.new.2.bin"
    
    #endif
