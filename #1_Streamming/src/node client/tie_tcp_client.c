#include "tie_tcp_client.h"

#define HOST_IP_ADDR "192.168.4.1"
#define PORT 6123
static const char *TAG = "TCP_CLIENT";
static const char *payload = "task_view";
static int sock_available = -1;
static tcp_client_proc_cback_t p_client_cb = NULL;

void tie_tcp_client_task(void *p)
{
	char addr_str[128];
	int addr_family;
	int ip_protocol;

	while (1) {

		struct sockaddr_in destAddr;
		destAddr.sin_addr.s_addr = inet_addr(HOST_IP_ADDR);
		destAddr.sin_family = AF_INET;
		destAddr.sin_port = htons(PORT);
		addr_family = AF_INET;
		ip_protocol = IPPROTO_IP;
		inet_ntoa_r(destAddr.sin_addr, addr_str, sizeof(addr_str) - 1);

		int sock =  socket(addr_family, SOCK_STREAM, ip_protocol);
		if (sock < 0) {
			ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
			vTaskDelay(2000 / portTICK_PERIOD_MS);
			break;
		}
		ESP_LOGI(TAG, "Socket created");

		int err = connect(sock, (struct sockaddr *)&destAddr, sizeof(destAddr));
		if (err != 0) {
			ESP_LOGE(TAG, "Socket unable to connect: errno %d", errno);
			close(sock);
			vTaskDelay(2000 / portTICK_PERIOD_MS);
			continue;
		}
		ESP_LOGI(TAG, "Successfully connected");
		
		int err_send = send(sock, payload, strlen(payload), 0);
		if (err_send < 0) {
			ESP_LOGE(TAG, "Error occured during sending: errno %d", errno);
			if (sock != -1) {
				ESP_LOGE(TAG, "Shutting down socket and restarting...");
				shutdown(sock, 0);
				close(sock);
			}
			vTaskDelay(2000 / portTICK_PERIOD_MS);
			continue;
		}
		
		sock_available = sock;
		char rx_buffer[1024];
		while (1) 
		{
			int len = recv(sock, rx_buffer, sizeof(rx_buffer)-1, 0);

			if (len < 0) 
			{
				ESP_LOGE(TAG, "recv failed: errno %d", errno);
				break;
			}
			else 
			{
				rx_buffer[len] = 0; // Null-terminate whatever we received and treat like a string
				p_client_cb( rx_buffer );
			}
		}

		if (sock != -1) {
			ESP_LOGE(TAG, "Shutting down socket and restarting...");
			sock_available = -1;
			shutdown(sock, 0);
			close(sock);
		}
	}
	vTaskDelete(NULL);
}
void tie_tcp_client_set_cback(void *p)
{
	p_client_cb = p;
}
void tie_tcp_client_send(const char* data)
{
	int err = send(sock_available, data, strlen(data), 0);
	if (err < 0) {
		ESP_LOGE(TAG, "Error occured during sending: errno %d", errno);
	}
}
