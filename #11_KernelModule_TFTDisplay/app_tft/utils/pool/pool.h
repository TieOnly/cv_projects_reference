#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>

#define PARAMS_COUNT_MASK(_1,_2,_3,_4,_5,_6,_7,_8,_9,N,...) 	N
#define PARAMS_COUNT(...)		PARAMS_COUNT_MASK(__VA_ARGS__, 9,8,7,6,5,4,3,2,1,0)

#define DECLARE_VAR_2(_va_list, name, type)			\
	type name = va_arg(_va_list, type);

#define DECLARE_VAR_4(_va_list, name, type, ...)	\
	type name = va_arg(_va_list, type);				\
	DECLARE_VAR_2(_va_list, __VA_ARGS__)			\

#define DECLARE_VAR_6(_va_list, name, type, ...)	\
	type name = va_arg(_va_list, type);				\
	DECLARE_VAR_4(_va_list, __VA_ARGS__)			\

#define DECLARE_VAR_8(_va_list, name, type, ...)	\
	type name = va_arg(_va_list, type);				\
	DECLARE_VAR_6(_va_list, __VA_ARGS__)			\

#define DECLARE_VARS(num_of_params, _va_list, ...)				DECLARE_VAR_##num_of_params(_va_list, __VA_ARGS__)
#define SPREAD_LIST_VARGS_MASK(num_of_params, _va_list, ...)	DECLARE_VARS(num_of_params, _va_list, __VA_ARGS__)
#define SPREAD_LIST_VARGS(fst, ...)										\
	va_list ap;															\
	va_start(ap, fst);												\
	SPREAD_LIST_VARGS_MASK(PARAMS_COUNT(__VA_ARGS__), ap, __VA_ARGS__)	\
	va_end(ap);															\

#define SPREAD_VA_LIST(args, ...)		SPREAD_LIST_VARGS_MASK(PARAMS_COUNT(__VA_ARGS__), args, __VA_ARGS__)


typedef void* ppack_t;
typedef const void* c_ppack_t;
typedef size_t size_pool;
typedef size_t size_pack;
typedef void* (pack_func)(ppack_t, void*);

typedef struct {

	ppack_t pack;
	uint8_t* empty_idx;
	size_pool s_pool;
	size_pack s_pack;
	size_t count;

} pool_t;

typedef enum {
	POOL_PACK_FROM_CONST,
	POOL_PACK_FROM_VAR,
} POOL_FLAG;

int pool_create(pool_t*, size_pool, size_pack);
int pool_add(pool_t*, c_ppack_t, POOL_FLAG);
int pool_remove(pool_t*, c_ppack_t);
int pool_exist_pack(pool_t*, c_ppack_t, POOL_FLAG);
int pool_delete(pool_t*);

ppack_t pool_get_pack(pool_t*, size_t index);

/* FOR-EACH
	pool_t* 			: ref pool we loop
	void* *iter_func 	: call back func that processing logic
							.if return NULL --> continue loop
							.else return its RETURN VALUE
	void* vargs			: variable will be passing in *iter_func()
*/
void* pool_for_each(pool_t*, pack_func callback, ...);
