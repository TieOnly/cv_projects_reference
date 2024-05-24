#pragma once
#include "tie_define.h"
#include "cJSON.h"
#include "pool.h"
#include "tcp_ser.h"

//Need to free heap string return when not using
char* tcp_create_pack(const char* mes, const char* data);

//
void* tie_pool_print_sk(ppack_t pack, void* vargs);

//
// cJSON* tie_check_json_pack(const char* data);
