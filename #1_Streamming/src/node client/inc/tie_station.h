#include "tie_define.h"
#include "tie_tcp_server.h"

//#define CONFIG_EXAMPLE_WIFI_SSID	"Thanh Nhung"
//#define CONFIG_EXAMPLE_WIFI_PASSWORD	"12021971"

// #define CONFIG_EXAMPLE_WIFI_SSID		"Khanh Nhu"
// #define CONFIG_EXAMPLE_WIFI_PASSWORD	"23082011"

#define CONFIG_EXAMPLE_WIFI_SSID		"tieonly"
#define CONFIG_EXAMPLE_WIFI_PASSWORD	""

void wifi_init_sta(void);
void wifi_init_sta_reconfig(const char* ssid, const char* key);
