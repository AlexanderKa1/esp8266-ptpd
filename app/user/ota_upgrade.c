#include "ota_upgrade.h"
#include "upgrade.h"
#include "osapi.h"
#include "mem.h"
#include "custom_defines.h"

struct espconn upgrade_connection;
struct _esp_tcp upgrade_tcp;
struct upgrade_server_info upgrade_server;

static void ICACHE_FLASH_ATTR
check_upgrade_result(void *arg)
{
    struct upgrade_server_info *server = arg;

    if (server->upgrade_flag == true)
    {
        os_printf("Upgraded successfully. Rebooting...\r\n");
        system_upgrade_reboot();
    }
    else
        os_printf("Upgrade failed.\r\n");
}

void ICACHE_FLASH_ATTR
ota_upgrade(remot_info *phost_info)
{
    char user_bin[32] = {0};

    upgrade_connection.proto.tcp = &upgrade_tcp;
    upgrade_connection.type = ESPCONN_TCP;
    upgrade_connection.state = ESPCONN_NONE;

    /* Upgrade server is the TCP client that sent upgrade command to esp8266 */
    os_memcpy(upgrade_tcp.remote_ip, phost_info->remote_ip, 4); 
    upgrade_tcp.remote_port = 80;
    
    os_memcpy(upgrade_server.ip, phost_info->remote_ip, 4);
    upgrade_server.port = 80;

    upgrade_server.check_cb = check_upgrade_result;
    upgrade_server.check_times = 120000;

    upgrade_server.url = (uint8_t *)os_zalloc(512);

    if (system_upgrade_userbin_check() == UPGRADE_FW_BIN1)
        os_memcpy(user_bin, OTA_USER2_BIN_FILENAME, 21);
    else if (system_upgrade_userbin_check() == UPGRADE_FW_BIN2)
        os_memcpy(user_bin, OTA_USER1_BIN_FILENAME, 21);

    os_sprintf(upgrade_server.url, OTA_UPGRADE_HTTP_REQUEST);

    //os_printf("OTA upgrade request:\r\n%s\r\n", upgrade_server.url);

    if (!system_upgrade_start(&upgrade_server))
        os_printf("Upgrade already started.\r\n");
}
