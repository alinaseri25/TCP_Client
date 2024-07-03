/* Includes ------------------------------------------------------------------*/
#include "tcp_client.h"

#if LWIP_TCP
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

struct tcp_pcb *client_pcb;


/* Private function prototypes -----------------------------------------------*/
static err_t tcp_client_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);
static err_t tcp_client_connected(void *arg, struct tcp_pcb *tpcb, err_t err);
static void tcp_server_error(void *arg, err_t err);
/* Private functions ---------------------------------------------------------*/
err_t tcp_client_connect(uint16_t _PortNumber,uint8_t *_IPAddress)
{
	err_t ret_err;
  ip_addr_t DestIPaddr;
  
  /* create new tcp pcb */
  client_pcb = tcp_new();
  
  if (client_pcb != NULL)
  {
    IP4_ADDR( &DestIPaddr, _IPAddress[0], _IPAddress[1], _IPAddress[2], _IPAddress[3] );
    
    /* connect to destination address/port */
    ret_err = tcp_connect(client_pcb,&DestIPaddr,_PortNumber,tcp_client_connected);
  }
	else
	{
		memp_free(MEMP_TCP_PCB, client_pcb);
		ret_err = ERR_MEM;
	}
	return ret_err;
}

static err_t tcp_client_connected(void *arg, struct tcp_pcb *tpcb, err_t err)
{
  if (err == ERR_OK)   
  {
		
		tcp_arg(tpcb, NULL);

		/* initialize LwIP tcp_recv callback function */ 
		tcp_recv(tpcb, tcp_client_recv);
		
		tcp_err(tpcb, tcp_server_error);
		
		tcp_Client_NewConnection(tpcb);
		
		return ERR_OK;
  }
  else
  {
    /* close connection */
    tcp_client_connection_close(tpcb);
  }
  return err;
}

static err_t tcp_client_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{
  err_t ret_err;
  
  /* if we receive an empty tcp frame from server => close connection */
  if (p == NULL)
  {
		/* we're done sending, close connection */
		tcp_client_connection_close(tpcb);
    ret_err = ERR_OK;
  }   
  /* else : a non empty frame was received from server but for some reason err != ERR_OK */
  else if(err != ERR_OK)
  {
    /* free received pbuf*/
    if (p != NULL)
    {
      pbuf_free(p);
    }
    ret_err = err;
  }
  else
  {
		tcp_Client_Recieve(tpcb,p->payload,p->len);
         
    /* Acknowledge data reception */
    tcp_recved(tpcb, p->tot_len);  
    
    pbuf_free(p);
    ret_err = ERR_OK;
  }
  return ret_err;
}

void tcp_server_error(void *arg, err_t err)
{
  LWIP_UNUSED_ARG(err);
}

void tcp_client_connection_close(struct tcp_pcb *tpcb)
{
  /* remove callbacks */
	tcp_arg(tpcb, NULL);
  tcp_recv(tpcb, NULL);
	tcp_err(tpcb, NULL);
  /* close tcp connection */
  tcp_close(tpcb);
  
}

err_t tcp_Client_SendString(struct tcp_pcb *tpcb,uint8_t * str , uint16_t len)
{
	err_t ret_err;
  ret_err = tcp_write(tpcb,str,len,1);
	return ret_err;
}

__weak void tcp_Client_Recieve(struct tcp_pcb *tpcb,uint8_t *Buf,uint32_t _Len)
{
	
}

__weak void tcp_Client_NewConnection(struct tcp_pcb *newpcb)
{
	
}

#endif /* LWIP_TCP */

