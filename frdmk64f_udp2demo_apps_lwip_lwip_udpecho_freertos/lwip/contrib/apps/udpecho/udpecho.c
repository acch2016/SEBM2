#include "udpecho.h"

#include "lwip/opt.h"

#if LWIP_NETCONN

#include "lwip/api.h"
#include "lwip/sys.h"

#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"

#define N 100
#define EVENT_BIT (1<<0)
uint16_t ping[N];
uint16_t pong[N];

EventGroupHandle_t event;

//counter
static void
server_thread(void *arg)
{
	struct netconn *conn;
	struct netbuf *buf;

	uint16_t *msg;

	uint16_t len;
	uint16_t buffer[4096];

	LWIP_UNUSED_ARG(arg);
	conn = netconn_new(NETCONN_UDP);
	netconn_bind(conn, IP_ADDR_ANY, 54321);
	//LWIP_ERROR("udpecho: invalid conn", (conn != NULL), return;);
	uint8_t dacBuffer[60];

	while (1)
	{
		netconn_recv(conn, &buf);
		netbuf_data(buf, (void**)&msg, &len);

		//PRINTF("%i" ,msg[0]);




// 		for(uint8_t indice = 0;indice < len; indice++ ){
// 			dacBuffer[indice]= *msg;
// 			msg++;
// 		}

	if (netbuf_copy(buf, buffer, sizeof(buffer)) != buf->p->tot_len) {
			LWIP_DEBUGF(LWIP_DBG_ON, ("netbuf_copy failed\n"));
	} else {
			PRINTF("%i", buffer[0]);
			if (EVENT_BIT & xEventGroupGetBits(event)) {
				netbuf_copy(buf, ping, N);
				xEventGroupClearBits(event, EVENT_BIT);
			} else {
				netbuf_copy(buf, pong, N);
				xEventGroupSetBits(event, EVENT_BIT);
			}
		}

	netbuf_delete(buf);

	}


}

/*-----------------------------------------------------------------------------------*/
//static void
//client_thread(void *arg)
//{
//	ip_addr_t dst_ip;
//	struct netconn *conn;
//	struct netbuf *buf;
//
//	LWIP_UNUSED_ARG(arg);
//	conn = netconn_new(NETCONN_UDP);
//	//LWIP_ERROR("udpecho: invalid conn", (conn != NULL), return;);
//
//	char *msg = "Hello loopback!";
//	buf = netbuf_new();
//	netbuf_ref(buf,msg,10);
//
//	IP4_ADDR(&dst_ip, 127, 0, 0, 1);
//
//	while (1)
//	{
//		netconn_sendto(conn, buf, &dst_ip, 50005);
//		vTaskDelay(1000);
//	}
//}
/*-----------------------------------------------------------------------------------*/
void
udpecho_init(void)
{
	event = xEventGroupCreate();
//	sys_thread_new("client", client_thread, NULL, 300, 1);
	sys_thread_new("server", server_thread, NULL, 300, 2);

}

#endif /* LWIP_NETCONN */
