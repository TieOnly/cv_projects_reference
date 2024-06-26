#pragma once
#include "tie_define.h"

#define PORT 6543

#define MAX_STATION_ACCEPT	30
#define MAX_STATION_SOCKET	30
#define MAX_TASK_SERVICE	10

#define TCP_SOCK_BUFFER_SIZE	1024*1024

typedef struct {
	uint32_t data_len;
	uint32_t data_type;
} TieDataDesc;

typedef enum {
	DT_STR = 0, DT_SERIES, DT_CMD, DT_COUNT
} TieDataType;

typedef enum {
	CMD_NONE = 0,
	
	CMD_CTRL_UP,
	CMD_CTRL_DN,
	CMD_CTRL_RT,
	CMD_CTRL_LT,
	CMD_CTRL_ENT

} TieDataCMD;

typedef struct {
	struct sockaddr_in sourceAddr;
	int sock;
	int aid;

	const void* recv;
	int recv_len;
	TieDataDesc data_desc;
	void* data_cont;
	uint32_t data_cnt;
	bool data_new;
} TieRecv;

typedef void (*tcp_process_cback)(const TieRecv*);

typedef struct {
	struct sockaddr_in sourceAddr;
	int sock;
	int own_aid;
	char addr_str[128];
	tcp_process_cback process_cb;
	uint8_t timing;
} TieSocket;

typedef void (*tcp_proc_new_task_cback)(const TieSocket*, const char* task, bool* pass_registration);

typedef void (*tcp_proc_remove_sock_cback)(const int sock, const int own_aid);

typedef struct {
	int sockets[MAX_STATION_SOCKET];
	char add_str[128];
	int aid;
	int16_t num_of_sock;
	bool empty_idx_sock[MAX_STATION_SOCKET];
	tcp_proc_remove_sock_cback proc_remove_sock_cb;
} TieAID;

typedef struct {
	char task_info[20];
	tcp_process_cback process_cb;
} TieRegisTask;


void tcp_man_init();

//add a new station
int roster_add_aid(const char* addr);
TieAID* roster_get_aid_struct(int aid);
TieAID* roster_get_aid_struct_by_addr(const char* addr);
//remove a station by aid and shutdown all its socket
void roster_remove_aid(int aid);

//add specific socket to socket roster of addr or aid present for this addr
int roster_add_socket(int sock, const char* addr);
//remove specific socket by own aid
void roster_remove_socket(int sock, const int own_aid);

//task for process all tcp comming
void tcp_server_main();
void tcp_server_task_delete();
bool tcp_server_task_alive();

//set callback for process tcp recieve from all tcp
void tcp_set_process_remove_sock_cback(void* cb);
void tcp_set_process_new_task_cback(void* cb);
int tcp_set_task_proc_cback(const char* task_info, void* cb);
void* tcp_get_task_proc_cback(const char* task_info);

//send back data to socket
void tcp_sk_send_str(const TieRecv* sk, const char* str);
void tcp_sk_send_data(const TieRecv* sk, TieDataType data_type, const void* data, size_t size);
void tcp_sk_send_data_cmd(const TieRecv* sk, TieDataCMD cmd);
void tcp_sk_send_data_str(const TieRecv* sk, const char* str);
void tcp_sk_send_data_series(const TieRecv* sk, const void* data, size_t size);
// void tcp_sk_get_from_recv(const TieRecv* sk, TieDataDesc* data_desc, const void* data);
