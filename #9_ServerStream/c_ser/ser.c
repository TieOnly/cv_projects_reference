#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#include "tcp_ser.h"
#include "tie_utils.h"

#define BUFFER_SIZE 1024

typedef struct 
{
	TieSocket sk;
	pool_t link_views;	
	bool is_stopped;
} cam_man_t;

typedef struct 
{
	TieSocket sk;
	int link_cam;
} view_man_t;


static struct man_stream_t
{
	TieSocket sock_admin;
	pool_t cam_man;
	pool_t view_man;
	pthread_mutex_t mtx;
} man_stream;

typedef enum {
	MSP_LIST_CLIENT = 0,
	MSP_SOCK_ID,
} MAN_STREAM_PACK;

void man_stream_init();
void man_stream_update_admin();
char* man_stream_wrap_pack(MAN_STREAM_PACK pack, ...);
void* tie_pool_print_cam_man(ppack_t pack, void* vargs)
{
	cam_man_t* cm = (cam_man_t*)pack;
	printf("camman: sock=%d, c_list=%lu\n", cm->sk.sock, cm->link_views.count);
	return NULL;
}

////////////////////////////////////////////////////////////////////////////////

void man_stream_init()
{
	man_stream.sock_admin.sock = -1;
	pool_create(&man_stream.cam_man, MAX_STATION_SOCKET, sizeof(cam_man_t));
	pool_create(&man_stream.view_man, MAX_STATION_SOCKET, sizeof(view_man_t));
	pthread_mutex_init(&man_stream.mtx, NULL);
}


void man_stream_add_cam(const TieSocket* sk)
{
	cam_man_t cam;
	memmove(&cam.sk, sk, sizeof(TieSocket));
	pool_create(&cam.link_views, MAX_STATION_SOCKET, sizeof(int));
	cam.is_stopped = false;

	pool_add(&man_stream.cam_man, (c_ppack_t)(&cam), POOL_PACK_FROM_VAR);
	printf("camman: add cam sk=%d, s_cam_man=%lu\n", cam.sk.sock, man_stream.cam_man.count);
}
void* _cam_change_link(ppack_t pack, void* args)
{
	SPREAD_VA_LIST( *(va_list*)args,
		sock_cam, int,
		sock_view, int
	);
	cam_man_t* cam = (cam_man_t*)pack;
	if ( pool_remove(&cam->link_views, (c_ppack_t)&sock_view) != -1 )
		printf("camman: cam= %d remove link view sk=%d, s_link=%lu\n", cam->sk.sock, sock_view, cam->link_views.count);
	if (cam->sk.sock == sock_cam)
	{
		pool_add(&cam->link_views, (c_ppack_t)(&sock_view), POOL_PACK_FROM_VAR);
		return cam;
	}
	return NULL;
}
void* _view_change_link(ppack_t pack, void* args)
{
	SPREAD_VA_LIST( *(va_list*)args,
		sock_cam, int,
		sock_view, int
	);
	view_man_t* view = (view_man_t*)pack;
	if (view->sk.sock == sock_view)
	{
		view->link_cam = sock_cam;
		return view;
	}
	return NULL;
}
void man_stream_change_link(int sock_cam, int sock_view)
{
	cam_man_t* cam = (cam_man_t*)pool_for_each(&man_stream.cam_man, _cam_change_link, sock_cam, sock_view);
	if (cam == NULL) return;
	printf("camman: cam=%d link view sk=%d, s_links=%lu\n", sock_cam, sock_view, cam->link_views.count);

	view_man_t* view = (view_man_t*)pool_for_each(&man_stream.view_man, _view_change_link, sock_cam, sock_view);
	if (view == NULL) return;
	printf("viewman: view=%d link cam sk=%d\n", sock_view, sock_cam);
}
void* _cam_remove_check(ppack_t pack, void* args)
{
	cam_man_t* cam = (cam_man_t*)pack;
	SPREAD_VA_LIST( *(va_list*)args, sock_remove, int );
	if (cam->sk.sock == sock_remove) return cam;
	return NULL;
}
void man_stream_remove_cam(int sock)
{
	cam_man_t* cam = (cam_man_t*)pool_for_each(&man_stream.cam_man, _cam_remove_check, sock);
	if (cam == NULL) return;

	for (size_t i = 0; i < cam->link_views.s_pool; ++i)
	{
		int* sock_view = (int*)pool_get_pack(&cam->link_views, i);
		if (sock_view == NULL) continue;
		
		view_man_t* view = (view_man_t*)pool_for_each(&man_stream.view_man, _view_change_link, -1, *sock_view);
		if (view == NULL) continue;
		printf("viewman: view=%d link cam sk=%d\n", *sock_view, -1);
	}

	pool_remove(&man_stream.cam_man, (c_ppack_t)cam);
	printf("camman: remove cam sk=%d, s_cam_man=%lu\n", sock, man_stream.cam_man.count);
}

void man_stream_add_view(const TieSocket* sk)
{
	view_man_t view;
	memmove(&view.sk, sk, sizeof(TieSocket));
	view.link_cam = -1;

	pool_add(&man_stream.view_man, (c_ppack_t)(&view), POOL_PACK_FROM_VAR);
	printf("VIEWMAN: add view sk=%d, s_view_man=%lu\n", view.sk.sock, man_stream.view_man.count);
}
void* _view_remove_check(ppack_t pack, void* args)
{
	view_man_t* view = (view_man_t*)pack;
	SPREAD_VA_LIST( *(va_list*)args, sock_remove, int );
	if (view->sk.sock == sock_remove) return view;
	return NULL;
}
void man_stream_remove_view(int sock)
{
	view_man_t* view = (view_man_t*)pool_for_each(&man_stream.view_man, _view_remove_check, sock);
	if (view == NULL) return;
	man_stream_change_link(-1, sock);
	pool_remove(&man_stream.view_man, (c_ppack_t)view);
	printf("VIEWMAN: remove view sk=%d, s_view_man=%lu\n", sock, man_stream.view_man.count);
}

int man_stream_remove_sock(int sock)
{
	if (sock == man_stream.sock_admin.sock)
	{
		man_stream_update_admin();
		printf("[MAN-STREAM] --- : remove admin sk=%d\n", sock);
		man_stream.sock_admin.sock = -1;
		return 0;
	}

	man_stream_remove_cam(sock);
	man_stream_remove_view(sock);
	man_stream_update_admin();
	return 0;
}
void man_stream_update_admin()
{
	if (man_stream.sock_admin.sock == -1) return;
	
	char *wrap_pack = man_stream_wrap_pack(MSP_LIST_CLIENT);
	if ( wrap_pack != NULL )
	{
		tcp_sk_send_data_str((TieRecv*)&man_stream.sock_admin, (const char*)wrap_pack);
		free(wrap_pack);
	}
}

void* _cam_update_view(ppack_t pack, void* args)
{
	view_man_t* view = (view_man_t*)pack;
	SPREAD_VA_LIST( *(va_list*)args,
		sock_cam, int,
		data, const void*,
		size, size_t
	);

	// printf("[MAN-STREAM] --- : update view sk=%d has link cam=%d, from cam=%d\n", view->sk.sock, view->link_cam, sock_cam);
	if ( view->link_cam == sock_cam )
	{
		TieRecv* sk = (TieRecv*)(&view->sk);
		tcp_sk_send_data_series(sk, data, size);
	}
	return NULL;
}
void man_stream_cam_update_view(int sock_cam, const void* data, size_t size)
{
	if (man_stream.sock_admin.sock != -1)
		tcp_sk_send_data_series((TieRecv*)&man_stream.sock_admin, data, size);

	pool_for_each(&man_stream.view_man, _cam_update_view, sock_cam, data, size);
}
cam_man_t* man_stream_get_cam_by_sock(int sock_cam)
{
	for ( size_t i = 0; i < man_stream.cam_man.s_pool; ++i )
	{
		cam_man_t* cam = (cam_man_t*)pool_get_pack(&man_stream.cam_man, i);
		if (cam == NULL) continue;

		if (cam->sk.sock == sock_cam) return cam;
	}
	return NULL;
}
view_man_t* man_stream_get_view_by_sock(int sock_view)
{
	for ( size_t i = 0; i < man_stream.view_man.s_pool; ++i )
	{
		view_man_t* view = (view_man_t*)pool_get_pack(&man_stream.view_man, i);
		if (view == NULL) continue;

		if (view->sk.sock == sock_view) return view;
	}
	return NULL;
}
char* man_stream_wrap_pack(MAN_STREAM_PACK pack, ...)
{
	switch (pack)
	{
	case MSP_LIST_CLIENT:
	{
		char list_cam_sk[100] = {0};
		char list_view_sk[100] = {0};

		for ( size_t i = 0; i < man_stream.cam_man.s_pool; ++i )
		{
			cam_man_t* cam = (cam_man_t*)pool_get_pack(&man_stream.cam_man, i);
			if (cam == NULL) continue;

			char sk_cam[10];
			sprintf(sk_cam, "%d", cam->sk.sock);
			if (list_cam_sk[0] != '\0') strcat(list_cam_sk, ",");
			strcat(list_cam_sk, sk_cam);
		}
		for ( size_t i = 0; i < man_stream.view_man.s_pool; ++i )
		{
			view_man_t* view = (view_man_t*)pool_get_pack(&man_stream.view_man, i);
			if (view == NULL) continue;

			char sk_view[20];
			sprintf(sk_view, "[%d,%d]", view->sk.sock, view->link_cam);
			if (list_view_sk[0] != '\0') strcat(list_view_sk, ",");
			strcat(list_view_sk, sk_view);
		}

		char data[1000] = {0};
		sprintf(data, "{\"cam\":[%s], \"view\":[%s]}", list_cam_sk, list_view_sk);
		char* wrap_pack = tcp_create_pack("list_client", data);
		// printf("[MAN-STREAM] --- wrap pack: %s\n", wrap_pack);
		return wrap_pack;
	} break;
	
	case MSP_SOCK_ID:
	{
		SPREAD_LIST_VARGS(pack, sock, int);
		char sock_str[10] = {0}; sprintf(sock_str, "%d", sock);
		char* wrap_pack = tcp_create_pack("sock_id", sock_str);
		// printf("[MAN-STREAM] --- wrap pack: %s\n", wrap_pack);
		return wrap_pack;
	} break;

	default: break;
	}

	return NULL;
}



void tie_tcp_new_task_proc_cb(const TieSocket* sk, const char* task, bool* pass)
{
	printf("[NEW-TASK-PROC] --- : sock=%d, task=%s\n", sk->sock, task);
	pthread_mutex_lock(&man_stream.mtx);

	// Send sock id for new task
	char* wrap_pack = man_stream_wrap_pack(MSP_SOCK_ID, sk->sock);
	if ( wrap_pack != NULL )
	{
		tcp_sk_send_data_str((TieRecv*)sk, wrap_pack);
		free(wrap_pack);
	}

	// Update management streaming
	if		(strcmp(task, "task-admin") == 0)
	{
		if (man_stream.sock_admin.sock == -1)
		{
			man_stream.sock_admin.sock = sk->sock;
			tcp_sk_send_data_cmd((TieRecv*)sk, CMD_AD_ACCEPT);
			man_stream_update_admin();
		}
		else
		{
			*pass = false;
			tcp_sk_send_data_cmd((TieRecv*)sk, CMD_AD_REJECT);
		}
	}
	else if (strcmp(task, "task-cam") == 0)
	{
		man_stream_add_cam(sk);
		man_stream_update_admin();
	}
	else if (strcmp(task, "task-view") == 0)
	{
		man_stream_add_view(sk);
		man_stream_update_admin();
	}
	else *pass = false;

	pthread_mutex_unlock(&man_stream.mtx);
}

void tie_tcp_admin_cb(const TieRecv* sk)
{
	printf("[USER-AT-%d] --- : recv admin\n", sk->sock);
	TieDataType data_type = sk->data_desc.data_type;

	if (data_type == DT_CMD)
	{
		pthread_mutex_lock(&man_stream.mtx);

		TieDataCMD cmd = *(TieDataCMD*)sk->data_cont;
		switch (cmd)
		{
		case CMD_AD_GET_LIST_CLIENT:
		{
			printf("[USER-AT-%d] --- CMD admin get list client\n", sk->sock);
			char *wrap_pack = man_stream_wrap_pack(MSP_LIST_CLIENT);
			if ( wrap_pack != NULL )
			{
				tcp_sk_send_data_str(sk, (const char*)wrap_pack);
				free(wrap_pack);
			}
		} break;

		default:
		{
			printf("[USER-AT-%d] --- cmd recv %d\n", sk->sock, cmd);
			tcp_sk_send_data_cmd(sk, CMD_NONE);
		} break;
		}

		pthread_mutex_unlock(&man_stream.mtx);
		return;
	}

	if (data_type == DT_STR)
	{
		printf("[USER-AT-%d] --- : JSON receive: %s\n", sk->sock, (const char*)sk->data_cont);
		cJSON* json = cJSON_Parse((const char*)sk->data_cont);
		if ( json == NULL )
		{
			printf("[USER-AT-%d] --- : JSON failed\n", sk->sock);
			cJSON_Delete(json);
			return;
		}
		const cJSON* mes = cJSON_GetObjectItemCaseSensitive(json, "mes");
		if (cJSON_IsString(mes) == 0 ) return;

		pthread_mutex_lock(&man_stream.mtx);

		//Change view link
		if (0 == strcmp(mes->valuestring, "change_view_link"))
		{
			const cJSON* data = cJSON_GetObjectItemCaseSensitive(json, "data");
			if (cJSON_IsString(data) == 0 )
			{
				pthread_mutex_unlock(&man_stream.mtx);
				return;
			}

			int sock_cam, sock_view = -1;
			sscanf(data->valuestring, "%d-%d", &sock_cam, &sock_view);
			printf("[USER-AT-%d] --- : JSON change view link cam=%d, view=%d\n", sk->sock, sock_cam, sock_view);

			man_stream_change_link(sock_cam, sock_view);
			man_stream_update_admin();
		}

		//Delete view
		if (0 == strcmp(mes->valuestring, "del_view"))
		{
			const cJSON* data = cJSON_GetObjectItemCaseSensitive(json, "data");
			if (cJSON_IsNumber(data) == 0 )
			{
				pthread_mutex_unlock(&man_stream.mtx);
				return;
			}

			int sock_view = data->valueint;
			int own_aid = -1;
			printf("[USER-AT-%d] --- : Admin want del view=%d\n", sk->sock, sock_view);
			for ( size_t i = 0; i < man_stream.view_man.s_pool; ++i )
			{
				view_man_t* view = (view_man_t*)pool_get_pack(&man_stream.view_man, i);
				if (view == NULL) continue;

				printf("[USER-AT-%d] --- : man has view=%d\n", sk->sock, sock_view);
				if (view->sk.sock == sock_view)
				{
					printf("[USER-AT-%d] --- : Ser del view=%d\n", sk->sock, view->sk.sock);
					own_aid = view->sk.own_aid;
					break;
				}
			}
			if (own_aid != -1)
			{
				man_stream_remove_view(sock_view);
				man_stream_update_admin();

				pthread_mutex_unlock(&man_stream.mtx);
				roster_remove_socket(sock_view, own_aid);
				pthread_mutex_lock(&man_stream.mtx);
			}
		}

		//Delete cam
		if (0 == strcmp(mes->valuestring, "del_cam"))
		{
			const cJSON* data = cJSON_GetObjectItemCaseSensitive(json, "data");
			if (cJSON_IsNumber(data) == 0 )
			{
				pthread_mutex_unlock(&man_stream.mtx);
				return;
			}

			int sock_cam = data->valueint;
			int own_aid = -1;
			printf("[USER-AT-%d] --- : Admin want del cam=%d\n", sk->sock, sock_cam);
			for ( size_t i = 0; i < man_stream.cam_man.s_pool; ++i )
			{
				cam_man_t* cam = (cam_man_t*)pool_get_pack(&man_stream.cam_man, i);
				if (cam == NULL) continue;

				printf("[USER-AT-%d] --- : man has cam=%d\n", sk->sock, sock_cam);
				if (cam->sk.sock == sock_cam)
				{
					printf("[USER-AT-%d] --- : Ser del cam=%d\n", sk->sock, cam->sk.sock);
					own_aid = cam->sk.own_aid;
					break;
				}
			}
			if (own_aid != -1)
			{
				man_stream_remove_cam(sock_cam);
				man_stream_update_admin();

				pthread_mutex_unlock(&man_stream.mtx);
				roster_remove_socket(sock_cam, own_aid);
				pthread_mutex_lock(&man_stream.mtx);
			}
		}

		//Stop/Pause cam
		if (0 == strcmp(mes->valuestring, "transmit_cam"))
		{
			const cJSON* data = cJSON_GetObjectItemCaseSensitive(json, "data");
			if (cJSON_IsString(data) == 0 )
			{
				pthread_mutex_unlock(&man_stream.mtx);
				return;
			}

			int sock_cam, is_stopped = -1;
			sscanf(data->valuestring, "%d-%d", &sock_cam, &is_stopped);
			printf("[USER-AT-%d] --- : JSON transmit cam=%d, is_stopped=%d\n", sk->sock, sock_cam, is_stopped);

			cam_man_t* cam = man_stream_get_cam_by_sock(sock_cam);
			if ( cam != NULL )
			{
				if (is_stopped != 0)
					tcp_sk_send_data_cmd((TieRecv*)&cam->sk, CMD_CAM_SER_STOP);
				else
					tcp_sk_send_data_cmd((TieRecv*)&cam->sk, CMD_CAM_SER_PLAY);
			}
		}

		pthread_mutex_unlock(&man_stream.mtx);
		return;
	}

	tcp_sk_send_data_cmd(sk, CMD_NONE);
}
void tie_tcp_cam_cb(const TieRecv* sk)
{	
	// printf("[USER-AT-%d] --- : recv cam\n", sk->sock);
	// printf("[USER-AT-%d] --- : d_s=%d, d_t=%d\n", sk->sock, sk->data_desc.data_len, sk->data_desc.data_type);

	TieDataType data_type = sk->data_desc.data_type;

	//CMD Process
	if (data_type == DT_CMD)
	{
		pthread_mutex_lock(&man_stream.mtx);

		TieDataCMD cmd = *(TieDataCMD*)sk->data_cont;
		switch (cmd)
		{
		
		default:
		{
			printf("[USER-AT-%d] --- cmd recv %d\n", sk->sock, cmd);
			tcp_sk_send_data_cmd(sk, CMD_NONE);
		} break;
		}

		pthread_mutex_unlock(&man_stream.mtx);
		return;
	}

	//SERIES Process
	if (data_type == DT_SERIES)
	{
		pthread_mutex_lock(&man_stream.mtx);

		man_stream_cam_update_view(sk->sock, sk->data_cont, sk->data_desc.data_len);
		
		pthread_mutex_unlock(&man_stream.mtx);
		return;
	}

	tcp_sk_send_data_cmd(sk, CMD_NONE);
}
void tie_tcp_view_cb(const TieRecv* sk)
{
	printf("[USER-AT-%d] --- : recv view\n", sk->sock);
	printf("[USER-AT-%d] --- : d_s=%d, d_t=%d\n", sk->sock, sk->data_desc.data_len, sk->data_desc.data_type);

	TieDataType data_type = sk->data_desc.data_type;

	//CMD Process
	if (data_type == DT_CMD)
	{
		TieDataCMD cmd = *(TieDataCMD*)sk->data_cont;
		switch (cmd)
		{
		default:
		{
			printf("[USER-AT-%d] --- cmd recv %d\n", sk->sock, cmd);
			tcp_sk_send_data_cmd(sk, CMD_NONE);
		} break;
		}
		return;
	}

	//SERIES Process
	if (data_type == DT_SERIES)
	{
		printf("[USER-AT-%d] --- series size=%d\n", sk->sock, sk->data_desc.data_len);
		tcp_sk_send_data_series(sk, sk->data_cont, sk->data_desc.data_len);
		return;
	}

	tcp_sk_send_data_cmd(sk, CMD_NONE);
}
void tie_tcp_remove_sock_cb( const int sock, const int own_aid )
{
	printf("[USER-AT-%d-%d] --- : process remove socket call back\n", sock, own_aid);
	pthread_mutex_lock(&man_stream.mtx);

	man_stream_remove_sock(sock);

	pthread_mutex_unlock(&man_stream.mtx);
}

int main()
{
	man_stream_init();
	tcp_man_init();

	tcp_set_process_remove_sock_cback(tie_tcp_remove_sock_cb);
	tcp_set_process_new_task_cback(tie_tcp_new_task_proc_cb);
	tcp_set_task_proc_cback("task-admin", tie_tcp_admin_cb);
	tcp_set_task_proc_cback("task-cam", tie_tcp_cam_cb);
	tcp_set_task_proc_cback("task-view", tie_tcp_view_cb);

	tcp_server_main();
	return 0;
}