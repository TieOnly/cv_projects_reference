#ifndef TIE_DEFINE
#define TIE_DEFINE

#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <pthread.h>

#define bitRead(reg, bit) 				(((reg) >> (bit)) & 0x01)
#define bitSet(reg, bit) 				((reg) |= (1UL << (bit)))
#define bitClear(reg, bit) 				((reg) &= ~(1UL << (bit)))
#define bitToggle(reg, bit) 			((reg) ^= (1UL << (bit)))
#define bitWrite(reg, bit, val)			((val) ? bitSet(reg, bit) : bitClear(reg, bit))

// #include <stdarg.h>

// #define PARAMS_COUNT_MASK(_1,_2,_3,_4,_5,_6,_7,_8,_9,N,...) 	N
// #define PARAMS_COUNT(...)		PARAMS_COUNT_MASK(__VA_ARGS__, 9,8,7,6,5,4,3,2,1,0)

// #define DECLARE_VAR_2(_va_list, name, type)			\
// 	type name = va_arg(_va_list, type);

// #define DECLARE_VAR_4(_va_list, name, type, ...)	\
// 	type name = va_arg(_va_list, type);				\
// 	DECLARE_VAR_2(_va_list, __VA_ARGS__)			\

// #define DECLARE_VAR_6(_va_list, name, type, ...)	\
// 	type name = va_arg(_va_list, type);				\
// 	DECLARE_VAR_4(_va_list, __VA_ARGS__)			\

// #define DECLARE_VAR_8(_va_list, name, type, ...)	\
// 	type name = va_arg(_va_list, type);				\
// 	DECLARE_VAR_6(_va_list, __VA_ARGS__)			\

// #define DECLARE_VARS(num_of_params, _va_list, ...)				DECLARE_VAR_##num_of_params(_va_list, __VA_ARGS__)
// #define SPREAD_LIST_VARGS_MASK(num_of_params, _va_list, ...)	DECLARE_VARS(num_of_params, _va_list, __VA_ARGS__)
// #define SPREAD_LIST_VARGS(...)											\
// 	va_list ap;															\
// 	va_start(ap, p);													\
// 	SPREAD_LIST_VARGS_MASK(PARAMS_COUNT(__VA_ARGS__), ap, __VA_ARGS__)	\
// 	va_end(ap);															\

#endif