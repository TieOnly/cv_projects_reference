#include "../components/include/tie_driver.h"
#include "../components/include/tie_5110.h"
#include "../components/include/tie_softap.h"
#include "../components/include/tie_station.h"
#include "../components/include/tie_base64.h"
#include "../components/include/tie_stream.h"
#include "../components/include/tie_tcp_client.h"

uint64_t old = 0;
uint64_t last = 0;
uint32_t dtime = 0;

typedef struct {
	int ani_cnt;
	int ani_w;
	int ani_h;
	int ani_f_size;
	char data[528];
	uint32_t ani_dura;
	uint32_t ani_dura_timer;
	char *pData;
} TieAni;

static TieAni tie_ani = {
	.ani_dura = 100
};

void vTaskLCD(void* p)
{
	tie_ani.ani_dura_timer = 0;
	for(;;)
	{
		last = esp_timer_get_time();
		dtime = (last - old)/1000;
		old = last;

		tie_ani.ani_dura_timer += dtime;
		if( tie_ani.ani_dura_timer >= tie_ani.ani_dura )
		{
			tie_ani.ani_dura_timer -= tie_ani.ani_dura;
			tie_ani.pData = tie_stream_get();
			// tie_stream_print_distance();
		}
		clearDisplay();
		DrawNumber( dtime, 0, 0, 1 );
		drawBitmap(0, 0, (const uint8_t*)tie_ani.pData, tie_ani.ani_w, tie_ani.ani_h, 1);
		display();
		vTaskDelay(10/portTICK_RATE_MS);
	}
}

void tie_client_recv_proc(const char* recv)
{	
	if (!strcmp(recv, "none")) return tie_tcp_client_send("station_view");
	
	cJSON* root = cJSON_Parse(recv);
	if (!cJSON_GetObjectItem(root, "msg")) 
	{
		// printf("Loop view no data\n");
		cJSON_Delete(root);
		return tie_tcp_client_send("!");
	}

	char* msg = cJSON_GetObjectItem(root, "msg")->valuestring;
	if ((strcmp(msg, "conf_frame") == 0))
	{
		tie_ani.ani_dura = cJSON_GetObjectItem(root, "aniDura")->valueint;
		tie_ani.ani_w = cJSON_GetObjectItem(root, "aniW")->valueint;
		tie_ani.ani_h = cJSON_GetObjectItem(root, "aniH")->valueint;
		printf(
			"[CONF] --- : ani_dura: %dms, w: %d, h: %d\n", 
			tie_ani.ani_dura, tie_ani.ani_w, tie_ani.ani_h
		);
		cJSON_Delete(root);
		return tie_tcp_client_send("cL"); 
	}
	if ((strcmp(msg, "cLoad")) == 0)
	{
		const char *enc = cJSON_GetObjectItem(root, "data")->valuestring;
		size_t out_len = b64_decoded_size(enc);
		printf("Recv enc size: %d\n", strlen(enc));
		
		if (!b64_decode(enc, (unsigned char *)tie_ani.pData, out_len)) printf("Decode Failed\n");
		cJSON_Delete(root);
		return tie_tcp_client_send("cL");
	}

	cJSON_Delete(root);
}

void app_main()
{
	init_pcd8544();

	wifi_init_sta();
	tie_tcp_client_set_cback(tie_client_recv_proc);

	xTaskCreate(vTaskLCD, "vTaskLCD", 2048, NULL, 2, NULL);
	xTaskCreate(tie_tcp_client_task, "vTaskTCPClient", 8192, NULL, 6, NULL);
}
