#include "../components/include/tie_driver.h"
#include "../components/include/tie_5110.h"
#include "../components/include/tie_softap.h"
#include "../components/include/tie_station.h"
#include "../components/include/tie_base64.h"
#include "../components/include/tie_stream.h"
#include "../components/include/tie_button.h"

uint8_t toggle = 0;
uint64_t old = 0;
uint64_t last = 0;
uint32_t dtime = 0;

typedef struct {
	int ani_cnt;
	int ani_w;
	int ani_h;
	int ani_f_size;
	
	uint32_t ani_dura;
	uint32_t ani_dura_timer;
	char *pData;

	bool availble_conf;
	char conf_frame[100];
} TieAni;

static TieAni tie_ani = {
	.ani_dura = 100,
	.availble_conf = false
};

typedef struct {
	int own_sk;
	int own_aid;
	char buff[1024];
} TieMsg;

static TieMsg msg_video = {
	.own_sk = -1,
	.own_aid = -1
};

#define NUMS_CLIENT_VIEW 32
static struct client_view_t {
	
	int8_t own_sk[NUMS_CLIENT_VIEW];
	int8_t num_own_sk;
	int8_t sock_enable;
	int8_t idx_sock_enable;
	char buff[1024];

} client_view;
static void client_view_reset()
{
	client_view.num_own_sk = 0;
	client_view.sock_enable = -1;
	client_view.idx_sock_enable = -1;
	memset(client_view.own_sk, -1, sizeof(client_view.own_sk));
	memcpy(client_view.buff, "empty", 5);
}
static void client_view_add( const int sk )
{
	for (int i = 0; i < NUMS_CLIENT_VIEW; ++i)
	{
		if (client_view.own_sk[i] == -1)
		{
			client_view.own_sk[i] = sk;
			client_view.num_own_sk += 1;
			printf("[ADD_CLIENT_VIEW] --- add %d, num own sk %d\n", sk, client_view.num_own_sk);
			return;
		}
	}
	printf("[ADD_CLIENT_VIEW] --- faild add %d, num own sk %d\n", sk, client_view.num_own_sk);
}
static void client_view_remove( const int sk )
{
	for (int i = 0; i < NUMS_CLIENT_VIEW; ++i)
	{
		if (client_view.own_sk[i] == sk)
		{
			client_view.own_sk[i] = -1;
			client_view.num_own_sk -= 1;
			printf("[REMOVE_CLIENT_VIEW] --- remove %d, num own sk %d\n", sk, client_view.num_own_sk);
			return;
		}
	}
	printf("[REMOVE_CLIENT_VIEW] --- faild remove %d, num own sk %d\n", sk, client_view.num_own_sk);
}
static int client_view_exist_sk( const int sk )
{
	for (int i = 0; i < NUMS_CLIENT_VIEW; ++i)
	{
		if (client_view.own_sk[i] == sk) return i;
	}
	// printf("[CHECK_CLIENT_VIEW] --- not exist %d, num own sk %d\n", sk, client_view.num_own_sk);
	return -1;
}

void tie_tcp_src_cb(const TieRecv* sk)
{
	cJSON* root = cJSON_Parse(sk->recv);
	if (!cJSON_GetObjectItem(root, "msg")) 
	{
		cJSON_Delete(root);
		return tie_tcp_sk_send(sk, "none");
	}
	char* msg = cJSON_GetObjectItem(root, "msg")->valuestring;

	if ((strcmp(msg, "station_src") == 0))
	{
		if( msg_video.own_sk == -1 )
		{
			printf("[SK-%d] --- : Add station source\n", sk->sock);
			msg_video.own_sk = sk->sock;
			msg_video.own_aid = sk->aid;
			cJSON_Delete(root);
			return tie_tcp_sk_send(sk, "conf_frame");
		}
		else
		{
			cJSON_Delete(root);
			return tie_tcp_sk_send(sk, "not_access");
		}
	}
	if ((strcmp(msg, "conf_frame") == 0))
	{
		tie_ani.ani_dura = cJSON_GetObjectItem(root, "aniDura")->valueint;
		tie_ani.ani_w = cJSON_GetObjectItem(root, "aniW")->valueint;
		tie_ani.ani_h = cJSON_GetObjectItem(root, "aniH")->valueint;
		tie_ani.availble_conf = true;
		memcpy(tie_ani.conf_frame, sk->recv, strlen(sk->recv));
		printf(
			"[SK-%d] --- : ani_dura: %dms, w: %d, h: %d\n", 
			sk->sock, tie_ani.ani_dura, tie_ani.ani_w, tie_ani.ani_h
		);

		memcpy(client_view.buff, sk->recv, strlen(sk->recv));
		cJSON_Delete(root);
		return tie_tcp_sk_send(sk, "cL"); 
	}

	if (msg_video.own_sk == sk->sock)
	{
		memcpy(client_view.buff, sk->recv, strlen(sk->recv));
		cJSON_Delete(root);
		return tie_tcp_sk_send(sk, "cL"); 
	}

	cJSON_Delete(root);
	msg_video.own_sk = -1;
	msg_video.own_aid = -1;
	return tie_tcp_sk_send(sk, "none");
}
void tie_tcp_view_cb(const TieRecv* sk)
{
	if ((strcmp(sk->recv, "station_view")) == 0)
	{
		printf("[SK-%d] --- : Add station view\n", sk->sock);
		client_view_add(sk->sock);

		if (tie_ani.availble_conf)
			return tie_tcp_sk_send(sk, tie_ani.conf_frame);
		else
			return tie_tcp_sk_send(sk, client_view.buff);
	}
	if (client_view.sock_enable != -1 && client_view.sock_enable != sk->sock)
	{
		return tie_tcp_sk_send(sk, "Admin denied");
	}
	if (client_view_exist_sk(sk->sock) != -1)
	{
		// printf("[SK-%d] --- : Loop view\n", sk->sock);
		return tie_tcp_sk_send(sk, client_view.buff); 
	}

	return tie_tcp_sk_send(sk, "none");
}
void tie_tcp_temp_cb(const TieRecv* sk)
{
	printf("[SK-%d] --- : recv temp\n", sk->sock);
	return tie_tcp_sk_send(sk, "none");
}
void tie_tcp_remove_sock_cb( const int sock, const int own_aid )
{
	printf("[SK-%d-%d] --- : process remove socket call back\n", sock, own_aid);
	if (msg_video.own_sk == sock && msg_video.own_aid == own_aid)
	{
		msg_video.own_sk = -1;
		msg_video.own_aid = -1;
		memset(client_view.buff, 0, 1024);
		memcpy(client_view.buff, "empty", 5);
	}
	if (client_view_exist_sk(sock) != -1) client_view_remove(sock);
}

static TieButton btn;
static int idx = -1;
void BTN_GPIO12_Press_CBack(uint32_t pin_num)
{
	TD_gpio_out_toggle( GPIO_NUM_2 );
	while ( ++idx < NUMS_CLIENT_VIEW )
	{
		if (client_view.own_sk[idx] != -1)
		{
			client_view.sock_enable = client_view.own_sk[idx];
			// printf("\n[ADMIN_SELECT_CLIENT_VIEW] --- only accept %d socket\n", client_view.sock_enable);
			return;
		}
	}
	// printf("\n[ADMIN_SELECT_CLIENT_VIEW] --- accept all socket\n");
	idx = -1;
	client_view.sock_enable = -1;
}

void Task_Log_Socket(void *p)
{
	while(1)
	{
		printf("[ADMIN] --- accecpt socket %d, idx %d\n", (int)client_view.sock_enable, idx);
		vTaskDelay(2000/portTICK_PERIOD_MS);
	}
}

void app_main()
{
	TDriver_Init();
	TD_gpio_out_level(GPIO_NUM_2, 0);
	TieBTN_Init(&btn, GPIO_NUM_0, GPIO_PULLUP_ONLY, BTN_GPIO12_Press_CBack);
	TieBTN_Install_INTR_Service(&btn, 1);

	wifi_init_softap();
	
	client_view_reset();
	tie_set_tcp_process_cback(tie_tcp_temp_cb);
	tie_set_tcp_process_remove_sock_cback(tie_tcp_remove_sock_cb);
	tie_set_tcp_task_proc_cback("task_src", tie_tcp_src_cb);
	tie_set_tcp_task_proc_cback("task_view", tie_tcp_view_cb);

	xTaskCreate(Task_Log_Socket, "vTaskLog", 1024, NULL, 4, NULL);
	xTaskCreate(tie_tcp_server_task, "vTaskTCP", 1024*11, NULL, 5, NULL);
}
