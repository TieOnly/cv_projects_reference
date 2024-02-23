#pragma once
#include "tie_define.h"

#define PORT 6123

#define MAX_STATION_ACCEPT	30
#define MAX_STATION_SOCKET	30

typedef struct {
	struct sockaddr_in sourceAddr;
	int sock;
	int aid;
	const char* recv;
} TieRecv;

typedef void (*tcp_process_cback)(const TieRecv*);
typedef void (*tcp_proc_remove_sock_cback)(const int sock, const int own_aid);

typedef struct {
	char addr_str[128];
	struct sockaddr_in sourceAddr;
	tcp_process_cback process_cb;
	int sock;
	int own_aid;
	uint8_t timing;
} TieSocket;

typedef struct {
	int sockets[MAX_STATION_SOCKET];
	char add_str[128];
	int aid;
	int16_t num_of_sock;
	uint64_t empty_idx_sock;
	tcp_proc_remove_sock_cback proc_remove_sock_cb;
} TieAID;

typedef struct {
	char task_info[20];
	tcp_process_cback process_cb;
} TieRegisTask;

//add a new station
int tie_station_add_aid();
TieAID* tie_get_aid_struct(int aid);
TieAID* tie_get_aid_struct_by_addr(const char* addr);
//remove a station by aid and shutdown all its socket
void tie_station_remove_aid(int aid);

//add specific socket to socket roster of addr or aid present for this addr
int tie_station_add_socket(int sock, const char* addr);
//remove specific socket by own aid
void tie_station_remove_socket(int sock, const int own_aid);
void tie_station_remove_socket(int sock, const int own_aid);

//task for process all tcp comming
void tie_tcp_server_task(void *pvParameters);
void tie_tcp_server_task_delete();
bool tie_tcp_server_task_alive();
//set callback for process tcp recieve from all tcp
void tie_set_tcp_process_cback(void* cb);
void tie_set_tcp_process_remove_sock_cback(void* cb);
bool tie_set_tcp_task_proc_cback(const char* task_info, void* cb);
// void* tie_get_tcp_task_proc_cback(const char* task_info);
void* tie_get_tcp_task_proc_cback(const char* task_info);
// bool tie_assign_tcp_task_proc_cback(const char* task_info, void* proc);
//send back data to socket
void tie_tcp_sk_send(const TieRecv* sk, const char* data);
