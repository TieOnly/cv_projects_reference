#include "tcp_ser.h"

static struct man_t
{

	TieAID staton_roster[MAX_STATION_ACCEPT];
	bool empty_idx_aid[MAX_STATION_ACCEPT];
	tcp_proc_remove_sock_cback tcp_proc_remove_sock_cb;
	int listen_sock;

	TieRegisTask regis_task_roster[MAX_TASK_SERVICE];
	uint8_t regis_task_count;
	tcp_proc_new_task_cback tcp_proc_new_task_cb;
} man;

////////////////////////////////////////////////////////////////////////////////////////

void tcp_man_init()
{
	memset(man.empty_idx_aid, 1, MAX_STATION_ACCEPT);
	man.tcp_proc_remove_sock_cb = NULL;
	man.tcp_proc_new_task_cb = NULL;
	man.listen_sock = -1;

	man.regis_task_count = 0;
	for (int i = 0; i < MAX_TASK_SERVICE; ++i)
	{
		strcpy(man.regis_task_roster[i].task_info, "\0");
		man.regis_task_roster[i].process_cb = NULL;
	}
}

int roster_add_aid(const char* addr)
{
	for( uint8_t i = 0; i < MAX_STATION_ACCEPT; ++i )
	{
		if (man.empty_idx_aid[i])
		{
			printf("[MAN_ADD_AID] --- aid: %d\n", i+1);
			TieAID new_aid = { 
				.aid = i+1,
				.proc_remove_sock_cb = man.tcp_proc_remove_sock_cb
			};
			memset(new_aid.empty_idx_sock, 1, MAX_STATION_SOCKET);
			strcpy(new_aid.add_str, addr);

			man.staton_roster[i] = new_aid;
			man.empty_idx_aid[i] = 0;
			return i;
		}
	}
	return -1;
}
TieAID* roster_get_aid_struct(int aid)
{
	for( uint8_t i = 0; i < MAX_STATION_ACCEPT; ++i )
	{
		if (man.empty_idx_aid[i] == 0)
		{
			if (man.staton_roster[i].aid == aid) 
				return &man.staton_roster[i];
		}
	}
	printf("[MAN_GET_AID] -- this %d no match with any availble station aid\n", aid);
	return NULL;
}
TieAID* roster_get_aid_struct_by_addr(const char* addr)
{
	for( uint8_t i = 0; i < MAX_STATION_ACCEPT; ++i )
	{
		if (man.empty_idx_aid[i] == 0)
		{
			if (strcmp((const char*)man.staton_roster[i].add_str, addr) == 0)
				return &man.staton_roster[i];
		}
	}
	printf("[MAN_GET_AID] --- this %s no match with any availble station addr\n", addr);
	return NULL;
}
void roster_remove_aid(int aid)
{
	TieAID* avai_aid = roster_get_aid_struct(aid);
	for( uint8_t i = 0; i < MAX_STATION_SOCKET; ++i )
	{
		if (avai_aid->empty_idx_sock[i] == 0)
		{
			if (avai_aid->proc_remove_sock_cb != NULL)
				avai_aid->proc_remove_sock_cb( avai_aid->sockets[i], aid );
			
			shutdown(avai_aid->sockets[i], 0);
			close(avai_aid->sockets[i]);
			printf("[MAN_DEL_AID] --- remove sock: %d of aid: %d\n", avai_aid->sockets[i], aid);
			avai_aid->sockets[i] = -1;
			avai_aid->num_of_sock = 0;
		}
	}
	memset(avai_aid->empty_idx_sock, 1, MAX_STATION_SOCKET);
	printf("[MAN_DEL_AID] --- aid: %d, n_sk_rest: %d\n", aid, avai_aid->num_of_sock);
}

int roster_add_socket(int sock, const char* addr)
{
	printf("[MAN_ADD_SK] --- addr: %s\n", addr);
	int own_aid = -1;

	TieAID* avai_aid = roster_get_aid_struct_by_addr(addr);
	if (avai_aid == NULL) return own_aid;
	for( uint8_t i = 0; i < MAX_STATION_SOCKET; ++i )
	{
		if (avai_aid->empty_idx_sock[i])
		{
			avai_aid->sockets[i] = sock;
			avai_aid->num_of_sock += 1;
			avai_aid->empty_idx_sock[i] = 0;
			printf("[MAN_ADD_SK] --- aid: %d, addr: %s, n_sk: %d\n", avai_aid->aid, avai_aid->add_str, avai_aid->num_of_sock);
			own_aid = avai_aid->aid;
			break;
		}
	}

	return own_aid;
}
void roster_remove_socket(int sock, const int own_aid)
{
	char TAG[20]; sprintf(TAG, "[MAN_DEL_SK]");
	printf("%s --- sock: %d, own_aid: %d\n", TAG, sock, own_aid);
	TieAID* avai_aid = roster_get_aid_struct(own_aid);
	if (avai_aid == NULL)
	{
		printf("%s --- no exist %d aid in staion roster\n", TAG, own_aid);
		return;
	}

	for( uint8_t i = 0; i < MAX_STATION_SOCKET; ++i )
	{
		if (avai_aid->empty_idx_sock[i] == 0)
		{
			if (avai_aid->sockets[i] == sock)
			{
				if (avai_aid->proc_remove_sock_cb != NULL) 
					avai_aid->proc_remove_sock_cb( sock, own_aid );
				
				shutdown(avai_aid->sockets[i], 0);
				close(avai_aid->sockets[i]);
				avai_aid->sockets[i] = -1;
				avai_aid->num_of_sock -= 1;
				avai_aid->empty_idx_sock[i] = 1;
				printf("%s --- done del, sock: %d, own_aid: %d, n_sk: %d\n", TAG, sock, own_aid, avai_aid->num_of_sock);
				return;
			}
			//else printf("%s --- this %d no match with %d, own_aid: %d\n", TAG, sock, avai_aid->sockets[i], own_aid);
		}
	}
}

void *tcp_thread_socket(void *vargs)
{
	TieSocket sk = *(TieSocket*)(vargs);
	char TAG[20]; sprintf(TAG, "[TCP-SK-%d]", sk.sock);
	printf("\n%s --- aid: %d, port: %d\n", TAG, sk.own_aid, sk.sourceAddr.sin_port);

	void *rx_buffer = malloc(TCP_SOCK_BUFFER_SIZE);
	bool pass_registration = false;

	TieRecv sk_recv = {
		.sock = sk.sock,
		.aid = sk.own_aid,
		.sourceAddr = sk.sourceAddr,
		.data_cnt = 0,
		.data_new = true	//for new data
	};

	//Process registration task for client==================================/
	int len = recv(sk.sock, rx_buffer, TCP_SOCK_BUFFER_SIZE, 0);
	if (len <= 0) printf("%s: Recv failed\n", TAG);
	else
	{
		TieDataDesc data_desc = *(TieDataDesc*)rx_buffer;
		if (len-sizeof(TieDataDesc) != data_desc.data_len)
		{
			pass_registration = false;
			data_desc.data_type = DT_COUNT;
		}
		else
		{
			char* data = malloc(data_desc.data_len);
			memmove(data, rx_buffer+sizeof(TieDataDesc), data_desc.data_len);
			memset(data+data_desc.data_len, 0, 1);

			// printf("%s: Pre register desc_len=%d, s_data=%lu, data=%s\n", TAG, data_desc.data_len, len-sizeof(TieDataDesc), data);
			if (data_desc.data_type == DT_STR)
				sk.process_cb = tcp_get_task_proc_cback(data);

			if( sk.process_cb != NULL )
			{
				printf("%s: Assign task process call back: '%s'\n", TAG, data);
				tcp_sk_send_data_str(&sk_recv, "result register: success");
				pass_registration = true;
			}
			else 
			{
				// printf("%s: '%s' not match with any task call back\n", TAG, data);
				tcp_sk_send_data_str(&sk_recv, "result register: failed");
				pass_registration = false;
			}

			if (pass_registration && man.tcp_proc_new_task_cb != NULL) 
				man.tcp_proc_new_task_cb(&sk, data, &pass_registration);
			
			free(data);
		}
	}


	bool rx_buffer_empty = true;
	memset(rx_buffer, 0, TCP_SOCK_BUFFER_SIZE);

	//Process data comming ===========================================//
	while (pass_registration)
	{
		if (rx_buffer_empty)
		{
			sk_recv.recv_len = recv(sk.sock, rx_buffer, TCP_SOCK_BUFFER_SIZE, 0);
			if (sk_recv.recv_len <= 0)
			{
				printf("%s: Recv failed\n", TAG);
				break;
			}
		}

		// printf("%s: Recv size=%d\n", TAG, sk_recv.recv_len);
		// When data is new, data content receive sufficiently
		if (sk_recv.data_new)
		{
			sk_recv.data_desc = *(TieDataDesc*)rx_buffer;
			sk_recv.data_cnt = sk_recv.recv_len-sizeof(TieDataDesc);

			// Invalid data lenght, data type
			if (	sk_recv.data_desc.data_len == 0
				||	sk_recv.data_desc.data_len > 1024*1024*100
				||	sk_recv.data_desc.data_type >= DT_COUNT
			)
			{
				printf("%s: Data receive valid t=%d, l=%d\n", TAG, sk_recv.data_desc.data_type, sk_recv.data_desc.data_len);
				memset(rx_buffer, 0, TCP_SOCK_BUFFER_SIZE);
				rx_buffer_empty = true;
				continue;
			}

			// Create memory store data content
			memmove(rx_buffer, rx_buffer+sizeof(TieDataDesc), sk_recv.data_cnt);
			sk_recv.data_cont = malloc(sk_recv.data_desc.data_len);
			memmove(sk_recv.data_cont, rx_buffer, sk_recv.data_desc.data_len);
		}
		else
		{
			// Receive less than num of data needed unpack
			if ( sk_recv.data_desc.data_len >= sk_recv.data_cnt + sk_recv.recv_len )
			{
				memmove(sk_recv.data_cont+sk_recv.data_cnt, rx_buffer, sk_recv.recv_len);
				// printf("%s: Not done, data_cnt=%d\n", TAG, sk_recv.data_cnt + sk_recv.recv_len);
			}
			sk_recv.data_cnt += sk_recv.recv_len;
		}

		// printf("%s: data_len=%d, data_cnt=%d\n", TAG, sk_recv.data_desc.data_len, sk_recv.data_cnt);
		// Not receive sufficiently
		if (sk_recv.data_desc.data_len > sk_recv.data_cnt)
		{
			sk_recv.data_new = false;
			rx_buffer_empty = true;
			continue;
		}
		// Receive more than num of data needed unpack
		else if (sk_recv.data_desc.data_len < sk_recv.data_cnt)
		{
			printf("%s: Over, data_len=%d, data_cnt=%d\n", TAG, sk_recv.data_desc.data_len, sk_recv.data_cnt);
			size_t offs_data_cont = 0;
			if ( sk_recv.data_cnt > (uint32_t)sk_recv.recv_len)
			{
				offs_data_cont = sk_recv.data_cnt - sk_recv.recv_len;
			}
			size_t len_data_pad = sk_recv.data_desc.data_len - offs_data_cont;
			memmove(sk_recv.data_cont+offs_data_cont, rx_buffer, len_data_pad);

			uint32_t len_data_excess = sk_recv.data_cnt - sk_recv.data_desc.data_len;
			memmove(rx_buffer, rx_buffer+len_data_pad, len_data_excess);
			sk_recv.recv_len = len_data_excess;

			sk_recv.data_new = true;
			rx_buffer_empty = false;
		}
		else
		{
			sk_recv.data_new = true;
			rx_buffer_empty = true;
			// printf("%s: Done, data_cnt=%d\n", TAG, sk_recv.data_cnt);
		}

		if (sk.process_cb != NULL && sk_recv.data_new) 
		{
			sk.process_cb( &sk_recv );
			free(sk_recv.data_cont);
		}
	}
	
	if (sk.sock != -1) roster_remove_socket(sk.sock, sk.own_aid);
	printf("%s: Kill this sock task of aid %d\n", TAG, sk.own_aid);
	free(rx_buffer);
	return NULL;
}

void tcp_server_main()
{
	int server_fd;
	struct sockaddr_in address;
	int opt = 1;
	char ser_addr_str[128];

	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	printf("Socket created\n");

	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
		perror("setsockopt failed");
		exit(EXIT_FAILURE);
	}
	printf("Socket setup\n");
	man.listen_sock = server_fd;

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	printf("Socket binding\n");
	strcpy(ser_addr_str, inet_ntoa(address.sin_addr));

	if (listen(server_fd, 3) < 0) {
		perror("listen failed");
		exit(EXIT_FAILURE);
	}

	while (1)
	{
		printf("[TCP] - %s:%d: Socket listening\n", ser_addr_str, PORT);
		
		int new_sock;
		struct sockaddr_in src_addr;
		uint32_t src_addr_len = sizeof(src_addr);
		char src_addr_str[128];

		//Waiting for accepting socket comming 
		if ((new_sock = accept(server_fd, (struct sockaddr *)&src_addr, (socklen_t *)&src_addr_len)) < 0) {
			perror("accept failed");
			exit(EXIT_FAILURE);
		}
		strcpy(src_addr_str, inet_ntoa(src_addr.sin_addr));
		
		//Add socket to roster
		int own_aid = roster_add_socket(new_sock, src_addr_str);
		if (own_aid == -1)
		{
			own_aid = roster_add_aid(src_addr_str);
			if (own_aid == -1)
			{
				printf("Full roster of aid or err with addr: %s\n", src_addr_str);
				continue;
			}
			own_aid = roster_add_socket(new_sock, src_addr_str);
		}
		printf(	"[TCP] - Accept new socket: %d at aid %d, from %s:%d\n", 
				new_sock, own_aid, src_addr_str, src_addr.sin_port
		);
		
		//Create new thread for socket
		pthread_t tid;
		TieSocket tie_sock =  { 
			.sock = new_sock,
			.sourceAddr = src_addr,
			.process_cb = NULL,
			.own_aid = own_aid
		};
		strcpy(tie_sock.addr_str, (const char*)src_addr_str);
		pthread_create(&tid, NULL, tcp_thread_socket, (void *)&tie_sock);
		pthread_detach(tid);
	}

	printf("Done\n");
}
void tcp_server_task_delete()
{
	for( uint8_t i = 0; i < MAX_STATION_ACCEPT; ++i )
	{
		if (man.empty_idx_aid[i] == 0) roster_remove_aid(man.staton_roster[i].aid);
	}
	shutdown(man.listen_sock, 0);
	close(man.listen_sock);
	man.listen_sock = -1;
	printf("[TCP] -- Try delete server\n");
}
bool tcp_server_task_alive()
{
	return man.listen_sock != -1;
}
void tcp_set_process_remove_sock_cback(void* cb)
{
	man.tcp_proc_remove_sock_cb = cb;
}
void tcp_set_process_new_task_cback(void* cb)
{
	man.tcp_proc_new_task_cb = cb;
}
int tcp_set_task_proc_cback(const char* task_info, void* cb)
{
	if (man.regis_task_count >= MAX_TASK_SERVICE) return -1;

	uint8_t idx = man.regis_task_count;
	strcpy(man.regis_task_roster[idx].task_info, task_info);
	man.regis_task_roster[idx].process_cb = cb;
	printf("[TCP_SET_TASK_CB] --- Register for [%s] task info\n", task_info);
	return man.regis_task_count++;
}
void* tcp_get_task_proc_cback(const char* task_info)
{
	for ( int i = 0; i < MAX_TASK_SERVICE; i++ )
	{
		if (strcmp(task_info, man.regis_task_roster[i].task_info) == 0)
		{
			printf("[TCP_ASSIGN_TASK_CB] --- Assign %s\n", task_info);
			return man.regis_task_roster[i].process_cb;
		}
	}
	return NULL;
}


void tcp_sk_send_str(const TieRecv* sk, const char* str)
{
	sendto(sk->sock, str, strlen(str), 0, (struct sockaddr *)&sk->sourceAddr, sizeof(sk->sourceAddr));
}
void tcp_sk_send_data(const TieRecv* sk, TieDataType data_type, const void* data, size_t size)
{
	TieDataDesc data_desc = {
		.data_len = size,
		.data_type = data_type
	};
	size_t sizeof_desc = sizeof(TieDataDesc);
	void* data_send = malloc((sizeof_desc+size));
	memcpy(data_send, &data_desc, sizeof_desc);
	memcpy(data_send+sizeof_desc, data, size);

	sendto(sk->sock, data_send, sizeof_desc+size, 0, (struct sockaddr *)&sk->sourceAddr, sizeof(sk->sourceAddr));

	free(data_send);
}
void tcp_sk_send_data_cmd(const TieRecv* sk, TieDataCMD cmd)
{
	tcp_sk_send_data(sk, DT_CMD, (void*)(&cmd), sizeof(TieDataCMD));
}
void tcp_sk_send_data_str(const TieRecv* sk, const char* str)
{
	tcp_sk_send_data(sk, DT_STR, (void*)(str), strlen(str));
}
void tcp_sk_send_data_series(const TieRecv* sk, const void* data, size_t size)
{
	tcp_sk_send_data(sk, DT_SERIES, data, size);
}
