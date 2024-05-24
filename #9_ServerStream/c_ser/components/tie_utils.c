#include "tie_utils.h"

char* tcp_create_pack(const char* mes, const char* data)
{
	cJSON *monitor = cJSON_CreateObject();
	if (monitor == NULL)
	{
		cJSON_Delete(monitor);
		return NULL;
	}

	if (cJSON_AddStringToObject(monitor, "mes", mes) == NULL)
	{
		cJSON_Delete(monitor);
		return NULL;
	}
	if (cJSON_AddStringToObject(monitor, "data", data) == NULL)
	{
		cJSON_Delete(monitor);
		return NULL;
	}

	char* string = NULL;
	string = cJSON_Print(monitor);

	cJSON_Delete(monitor);
	return string;
}

void* tie_pool_print_sk(ppack_t pack, void* vargs)
{
	TieSocket* p = (TieSocket*)pack;
	printf("%p-", p);
	printf("%d, ", p->sock);
	return NULL;
}

