/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TCP_CLIENT_H__
#define __TCP_CLIENT_H__

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "lwip/debug.h"
#include "lwip/stats.h"
#include "lwip/tcp.h"
#include "lwip/memp.h"
#include <stdio.h>
#include <string.h>
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
err_t tcp_client_connect(uint16_t _PortNumber,uint8_t *_IPAddress);

err_t tcp_Client_SendString(struct tcp_pcb *tpcb,uint8_t * str , uint16_t len);
void tcp_client_connection_close(struct tcp_pcb *tpcb);
__weak void tcp_Client_Recieve(struct tcp_pcb *tpcb,uint8_t *Buf,uint32_t _Len);
__weak void tcp_Client_NewConnection(struct tcp_pcb *newpcb);

#endif /* __TCP_CLIENT_H__ */
