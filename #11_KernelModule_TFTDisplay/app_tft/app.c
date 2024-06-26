#include "tie_define.h"
#include "tie_st7735.h"
#include "utils.h"
#include "tetris.h"
#include "tcp_ser.h"

void tcp_task_ctrl_cb(const TieRecv* sk)
{
	TieDataType data_type = sk->data_desc.data_type;

	if (data_type == DT_CMD)
	{
		TieDataCMD cmd = *(TieDataCMD*)sk->data_cont;
		signal_mtx_lock();
		switch (cmd)
		{
		case CMD_CTRL_UP: signal_ctrl_set(S_CTRL_UP); break;
		case CMD_CTRL_DN: signal_ctrl_set(S_CTRL_DN); break;
		case CMD_CTRL_RT: signal_ctrl_set(S_CTRL_RT); break;
		case CMD_CTRL_LT: signal_ctrl_set(S_CTRL_LT); break;
		case CMD_CTRL_ENT: signal_ctrl_set(S_CTRL_ENT); break;
		
		default:
		{
			printf("[USER-AT-%d] --- cmd recv %d\n", sk->sock, cmd);
			tcp_sk_send_data_cmd(sk, CMD_NONE);
		} break;
		}
		signal_mtx_unlock();
		return;
	}

	tcp_sk_send_data_cmd(sk, CMD_NONE);
}

void* thread_tcp_ser( void* agrs )
{
	tcp_man_init();
	tcp_set_task_proc_cback("task-ctrl", tcp_task_ctrl_cb);
	tcp_server_main();
	return agrs;
}

int main()
{
	tft_init();
	tft_clear(0xf1f1);

	pthread_t t_tcp_ser;
	pthread_create(&t_tcp_ser, NULL, thread_tcp_ser, NULL);
	pthread_detach(t_tcp_ser);

	assets_load();
	tet_init();

	uint32_t dtime = 0;
	while (1)
	{
		dtime = tick_dtime();
		tft_clear(COLOR_BLACK);
		
		signal_mtx_lock();
		tet_update(dtime);
		signal_ctrl_clear();
		signal_mtx_unlock();
		tet_draw();

		tft_display();
		usleep(1000*100);
	}

	return 0;
}