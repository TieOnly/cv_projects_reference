#include "tie_tcp_server.h"

static TieAID tie_staton_roster[MAX_STATION_ACCEPT];
static uint32_t tie_empty_idx_aid = 0xffffffff;
static int8_t tie_new_station_idx = -1;
static tcp_process_cback tie_tcp_process_cb = NULL;
static tcp_proc_remove_sock_cback tie_tcp_proc_remove_sock_cb = NULL;
static int tie_listen_sock = -1;

static TieRegisTask tie_regis_task_roster[2] = {
	{	.task_info = "task_src", 
		.process_cb = NULL },
	
	{ 	.task_info = "task_view", 
		.process_cb = NULL },
};
bool tie_set_tcp_task_proc_cback(const char* task_info, void* cb)
{
	for ( int i = 0; i < 2; i++ )
	{
		if (strcmp(task_info, tie_regis_task_roster[i].task_info) == 0)
		{
			tie_regis_task_roster[i].process_cb = cb;
			printf("[TCP_SET_TASK_CB] --- Add %s\n", task_info);
			return 1;
		}
	}
	printf("[TCP_SET_TASK_CB] --- Not available for [%s] task info\n", task_info);
	return 0;
}
void* tie_get_tcp_task_proc_cback(const char* task_info)
{
	for ( int i = 0; i < 2; i++ )
	{
		if (strcmp(task_info, tie_regis_task_roster[i].task_info) == 0)
		{
			printf("[TCP_ASSIGN_TASK_CB] --- Assign %s\n", task_info);
			return tie_regis_task_roster[i].process_cb;
		}
	}
	return NULL;
}
void tie_set_tcp_process_cback(void* cb)
{
	tie_tcp_process_cb = cb;
}
void tie_set_tcp_process_remove_sock_cback(void* cb)
{
	tie_tcp_proc_remove_sock_cb = cb;
}

//add a new station
int tie_station_add_aid()
{
	for( uint8_t i = 0; i < MAX_STATION_ACCEPT; ++i )
	{
		if (bitRead(tie_empty_idx_aid, i))
		{
			printf("[Tie_ADD_AID] --- aid: %d\n", i+1);
			TieAID tie_aid = { 
				.aid = i+1, 
				.empty_idx_sock = 0xffffffff,
				.proc_remove_sock_cb = tie_tcp_proc_remove_sock_cb
			};
			tie_staton_roster[i] = tie_aid;
			tie_new_station_idx = i;
			bitClear(tie_empty_idx_aid, i);
			return i;
		}
	}
	return -1;
}

//get a station aid struct
TieAID* tie_get_aid_struct(int aid)
{
	for( uint8_t i = 0; i < MAX_STATION_ACCEPT; ++i )
	{
		if (bitRead(tie_empty_idx_aid, i) == 0)
		{
			if (tie_staton_roster[i].aid == aid) 
				return &tie_staton_roster[i];
		}
	}
	printf("[Tie_GET_AID] -- this %d no match with any availble station aid\n", aid);
	return NULL;
}
TieAID* tie_get_aid_struct_by_addr(const char* addr)
{
	for( uint8_t i = 0; i < MAX_STATION_ACCEPT; ++i )
	{
		if (bitRead(tie_empty_idx_aid, i) == 0)
		{
			if (strcmp((const char*)tie_staton_roster[i].add_str, addr) == 0)
				return &tie_staton_roster[i];
		}
	}
	printf("[Tie_GET_AID] -- this %s no match with any availble station addr\n", addr);
	return NULL;
}

//remove station and all its socket
void tie_station_remove_aid(int aid)
{
	TieAID* avai_aid = tie_get_aid_struct(aid);
	for( uint8_t i = 0; i < MAX_STATION_SOCKET; ++i )
	{
		if (bitRead(avai_aid->empty_idx_sock, i) == 0)
		{
			avai_aid->proc_remove_sock_cb( avai_aid->sockets[i], aid );
			shutdown(avai_aid->sockets[i], 0);
			close(avai_aid->sockets[i]);
			printf("[Tie_DEL_AID] --- remove sock: %d of aid: %d\n", avai_aid->sockets[i],aid);
			avai_aid->sockets[i] = -1;
			avai_aid->num_of_sock = 0;
		}
	}
	avai_aid->empty_idx_sock = 0xffffffff;
	printf("[Tie_DEL_AID] --- aid: %d, n_sk_rest: %d\n", aid, avai_aid->num_of_sock);
}

//add socket for station, return own aid
int tie_station_add_socket(int sock, const char* addr)
{
	printf("[Tie_ADD_SK] --- addr: %s\n", addr);
	int own_aid = -1;
	//no  new station, just add new socket
	if (tie_new_station_idx < 0)
	{
		TieAID* avai_aid = tie_get_aid_struct_by_addr(addr);
		if (avai_aid == NULL) return own_aid;
		for( uint8_t i = 0; i < MAX_STATION_SOCKET; ++i )
		{
			if (bitRead(avai_aid->empty_idx_sock, i))
			{
				avai_aid->sockets[i] = sock;
				avai_aid->num_of_sock += 1;
				bitClear(avai_aid->empty_idx_sock, i);
				printf("[Tie_ADD_SK] --- aid: %d, addr: %s, n_sk: %d\n", avai_aid->aid, avai_aid->add_str, avai_aid->num_of_sock);
				own_aid = avai_aid->aid;
				break;
			}
		}
		if (avai_aid->empty_idx_sock == 0)
			printf("[Tie_ADD_SK] -- no idx empty for add sock into aid: %d\n", avai_aid->aid);
	}
	//already add new station, so add a new address and socket
	else
	{
		TieAID* avai_aid = &tie_staton_roster[tie_new_station_idx];
		strcpy(avai_aid->add_str, addr);
		for( uint8_t i = 0; i < MAX_STATION_SOCKET; ++i )
		{
			if (bitRead(avai_aid->empty_idx_sock, i))
			{
				avai_aid->sockets[i] = sock;
				avai_aid->num_of_sock += 1;
				bitClear(avai_aid->empty_idx_sock, i);
				break;
			}
		}
		own_aid = avai_aid->aid;
		printf("[Tie_ADD_SK] --- aid: %d, addr: %s, sk: %d, n_sk: %d\n", avai_aid->aid, avai_aid->add_str, sock, avai_aid->num_of_sock);
	}

	//must be reset value new station
	tie_new_station_idx = -1;
	return own_aid;
}

void tie_station_remove_socket(int sock, const int own_aid)
{
	printf("[TIE_DEL_SK] --- sock: %d, own_aid: %d\n", sock, own_aid);
	TieAID* avai_aid = tie_get_aid_struct(own_aid);
	if (avai_aid == NULL)
	{
		printf("[TIE_DEL_SK] --- no exist %d aid in staion roster\n", own_aid);
		return;
	}

	for( uint8_t i = 0; i < MAX_STATION_SOCKET; ++i )
	{
		if (bitRead(avai_aid->empty_idx_sock, i) == 0)
		{
			// printf("[TIE_DEL_SK] --- sk %d available idx %d\n", avai_aid->sockets[i], i);
			if (avai_aid->sockets[i] == sock)
			{
				avai_aid->proc_remove_sock_cb( sock, own_aid );
				shutdown(avai_aid->sockets[i], 0);
				close(avai_aid->sockets[i]);
				avai_aid->sockets[i] = -1;
				avai_aid->num_of_sock -= 1;
				bitSet(avai_aid->empty_idx_sock, i);
				printf("[TIE_DEL_SK] --- done del, sock: %d, own_aid: %d, n_sk: %d\n", sock, own_aid, avai_aid->num_of_sock);
				return;
			}
			else printf("[TIE_DEL_SK] --- this %d no match with %d, own_aid: %d\n", sock, avai_aid->sockets[i], own_aid);
		}
	}
}

void tie_tcp_sk_send(const TieRecv* sk, const char* data)
{
	sendto(sk->sock, data, strlen(data), 0, (struct sockaddr *)&sk->sourceAddr, sizeof(sk->sourceAddr));
}

void tie_task_socket(void *p)
{
	TieSocket sk = *(TieSocket*)(p);
	char C_TAG[20]; sprintf(C_TAG, "[Process-SK-%d]", sk.sock);
	printf("\n[Process-SK-%d] --- aid: %d, port: %d\n", sk.sock, sk.own_aid, sk.sourceAddr.sin_port);

	char rx_buffer[1024];
	TieRecv sk_recv = {
		.sock = sk.sock,
		.aid = sk.own_aid,
		.recv = rx_buffer,
		.sourceAddr = sk.sourceAddr,
	};
	
	int len = recv(sk.sock, rx_buffer, sizeof(rx_buffer)-1, 0);
	if (len <= 0) 
	{
		printf("%s: Recv failed\n", C_TAG);
		if (sk.sock != -1) tie_station_remove_socket(sk.sock, sk.own_aid);		
		printf("%s: Kill this sock task of aid %d\n", C_TAG, sk.own_aid);
		return vTaskDelete(NULL);
	}
	else
	{
		rx_buffer[len] = 0;
		sk.process_cb = tie_get_tcp_task_proc_cback(rx_buffer);
		if( sk.process_cb != NULL )
		{
			printf("%s: Change task process call back: [%s]\n", C_TAG, rx_buffer);
		}
		else 
		{
			printf("%s: No match with any task call back, choose temp task process\n", C_TAG);
			sk.process_cb = tie_tcp_process_cb;
		}
		tie_tcp_sk_send(&sk_recv, "none");
	}

	while (1) 
	{
		int len = recv(sk.sock, rx_buffer, sizeof(rx_buffer)-1, 0);

		if (len <= 0) 
		{
			printf("%s: Recv failed\n", C_TAG);
			break;
		}
		else
		{
			rx_buffer[len] = 0;
			sk.timing = 0;
			sk.process_cb( &sk_recv );
		}
	}

	if (sk.sock != -1) tie_station_remove_socket(sk.sock, sk.own_aid);		
	printf("%s: Kill this sock task of aid %d\n", C_TAG, sk.own_aid);
	vTaskDelete(NULL);
}
void tie_tcp_server_task(void *pvParameters)
{
	int addr_family;
	int ip_protocol;
	char addr_str[128];
	const char* TAG = "TCP_Server";

	while (1) {

		struct sockaddr_in destAddr;
		destAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		destAddr.sin_family = AF_INET;
		destAddr.sin_port = htons(PORT);
		addr_family = AF_INET;
		ip_protocol = IPPROTO_IP;
		inet_ntoa_r(destAddr.sin_addr, addr_str, sizeof(addr_str) - 1);

		int listen_sock = socket(addr_family, SOCK_STREAM, ip_protocol);
		if (listen_sock < 0) {
			ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
			break;
		}
		tie_listen_sock = listen_sock;
		ESP_LOGI(TAG, "Socket created");

		int err = bind(listen_sock, (struct sockaddr *)&destAddr, sizeof(destAddr));
		if (err != 0) {
			ESP_LOGE(TAG, "Socket unable to bind: errno %d", errno);
			break;
		}
		ESP_LOGI(TAG, "Socket binded");

		err = listen(listen_sock, 1);
		if (err != 0) {
			ESP_LOGE(TAG, "Error occured during listen: errno %d", errno);
			break;
		}
		ESP_LOGI(TAG, "Socket listening, listen socket: %d", tie_listen_sock);

		while(1)
		{
			struct sockaddr_in sourceAddr;
			uint32_t addrLen = sizeof(sourceAddr);
			int sock = accept(listen_sock, (struct sockaddr *)&sourceAddr, &addrLen);
			if (sock < 0) {
				ESP_LOGE(TAG, "Unable to accept connection: errno %d", errno);
				break;
			}

			inet_ntoa_r(((struct sockaddr_in *)&(sourceAddr))->sin_addr.s_addr, addr_str, sizeof(addr_str) - 1);
			int own_aid = tie_station_add_socket(sock, addr_str);
			if (own_aid == -1)
			{
				own_aid = tie_station_add_aid();
				if (own_aid == -1)
				{
					ESP_LOGE(TAG, "Full roster of aid or some err with addr: %s\n", addr_str);
					continue;
				}
				own_aid = tie_station_add_socket(sock, addr_str);
			}
			
			ESP_LOGI(TAG, "Socket accepted: %d - addr: %s, aid: %d, port: %d\n", sock, addr_str, own_aid, sourceAddr.sin_port);
			
			TieSocket tie_sock =  { 
				.sock = sock, 
				.sourceAddr = sourceAddr, 
				.process_cb = tie_tcp_process_cb,
				.own_aid = own_aid
			};
			sprintf(tie_sock.addr_str, (const char*)addr_str);
			xTaskCreate(tie_task_socket, "tcp-sk", 2048, &tie_sock, 4, NULL);
		}
		if (tie_listen_sock < 0) break;
	}
	ESP_LOGI(TAG, "Kill TCP server task");
    vTaskDelete(NULL);
}
void tie_tcp_server_task_delete()
{
	for( uint8_t i = 0; i < MAX_STATION_ACCEPT; ++i )
	{
		if (bitRead(tie_empty_idx_aid, i) == 0)
			tie_station_remove_aid(tie_staton_roster[i].aid);
	}
	int sock_del = tie_listen_sock;
	tie_listen_sock = -1;
	shutdown(sock_del, 0);
	close(sock_del);
	printf("[TCP-Task] -- Try delete task\n");
}
bool tie_tcp_server_task_alive()
{
	return tie_listen_sock != -1;
}
