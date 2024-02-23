#pragma once
#include "tie_tcp_server.h"

typedef void (*tcp_client_proc_cback_t)(const char*);

void tie_tcp_client_task(void *p);
void tie_tcp_client_set_cback(void *p);
void tie_tcp_client_send(const char* data);
