#include "pool.h"

int pool_create(pool_t* pool, size_pool s_pool, size_pack s_pack)
{
	pool->pack = malloc(s_pack*s_pool);
	memset(pool->pack, 0, s_pack*s_pool);
	pool->empty_idx = malloc(s_pool);
	memset(pool->empty_idx, 1, s_pool);
	pool->s_pool = s_pool;
	pool->s_pack = s_pack;
	pool->count = 0;
	// printf("POOL: s_pool=%lu, s_pack=%lu\n", pool->s_pool, pool->s_pack);
	return 0;
}
int pool_add(pool_t* pool, c_ppack_t pack, POOL_FLAG flag)
{
	if (pack == NULL) return -1;

	for (size_pool i = 0; i < pool->s_pool; ++i)
	{
		if (pool->empty_idx[i] != 0)
		{
			// printf("POOL: add pack idx=%lu\n", i);
			if (flag == POOL_PACK_FROM_CONST)
				memmove(pool->pack+(i*pool->s_pack), &pack, pool->s_pack);
			else if (flag == POOL_PACK_FROM_VAR)
				memmove(pool->pack+(i*pool->s_pack), pack, pool->s_pack);
			
			pool->empty_idx[i] = 0;
			pool->count += 1;
			return 0;
		}
	}
	return -1;
}
int pool_remove(pool_t* pool, c_ppack_t pack)
{
	for (size_pool i = 0; i < pool->s_pool; ++i)
	{
		if (pool->empty_idx[i] == 0)
		{
			if( 0 == memcmp(pool->pack+(i*pool->s_pack), pack, pool->s_pack) )
			{
				memset(pool->pack+(i*pool->s_pack), 0, pool->s_pack); 
				pool->empty_idx[i] = 1;
				pool->count -= 1;
				return i;
			}
		}
	}
	return -1;
}
int pool_exist_pack(pool_t* pool, c_ppack_t pack, POOL_FLAG flag)
{
	for (size_pool i = 0; i < pool->s_pool; ++i)
	{
		if (pool->empty_idx[i] == 0)
		{
			const void* p;
			if (flag == POOL_PACK_FROM_CONST)
				p = &pack;
			else if (flag == POOL_PACK_FROM_VAR)
				p = pack;

			if( 0 == memcmp(pool->pack+(i*pool->s_pack), p, pool->s_pack) )
				return 1;
		}
	}
	return 0;
}
ppack_t pool_get_pack(pool_t* pool, size_t index)
{
	if ( index >= pool->s_pool ) return NULL;
	if ( pool->empty_idx[index] == 1 ) return NULL;
	return (pool->pack+(index*pool->s_pack));
}
void* pool_for_each(pool_t* pool, pack_func callback, ...)
{
	va_list ap;
	for (size_pool i = 0; i < pool->s_pool; ++i)
	{
		if (pool->empty_idx[i] == 0)
		{
			// void* result_proc = iter_func(pool->pack+(i*pool->s_pack), vargs);
			va_start(ap, callback);
			void* result_proc = callback(pool->pack+(i*pool->s_pack), &ap);
			va_end(ap);

			if (NULL != result_proc)
				return result_proc;
			
			else continue;
		}
	}
	return NULL;
}
int pool_delete(pool_t* pool)
{
	free(pool->pack);
	free(pool->empty_idx);
	return 0;
}