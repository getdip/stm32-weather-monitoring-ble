/*
 * hm10.h
 *
 *  Created on: 16-Jan-2026
 *      Author: dipch
 */

#ifndef HM10_H_
#define HM10_H_
#include "stm32f407xx.h"

#define HM10_USART				USART2
#define HM10_USART_GPIO_PORT	GPIOA
#define HM10_USART_TX_PIN		GPIO_PIN_NO_2
#define HM10_USART_RX_PIN		GPIO_PIN_NO_3
#define HM10_USART_PIN_OP_TYPE	GPIO_OP_TYPE_PP
#define HM10_USART_PIN_PUPD		GPIO_PIN_PU
#define HM10_USART_PIN_SPEED	GPIO_SPEED_FAST

#define HM10_MODE				USART_TXRX
#define HM10_BAUD				USART_STD_BAUD_9600
#define HM10_STOP_BITS			USART_STOPBITS_1
#define HM10_WORD_LEN			USART_WORDLEN_8BITS
#define HM10_PARITY				USART_PARITY_DISABLE
#define HM10_HW_FLOW			USART_HW_FLOW_CTRL_NONE

#define START_STREAM			0
#define STOP_STREAM				1
#define STANDBY					2

#define RX_BUF_SIZE					128
#define TX_BUF_SIZE					128
#define CMD_BUF_SIZE				RX_BUF_SIZE
#define ENV_DATA_BUF_SIZE			17
#define ENV_DATA_TX_BUF_SIZE		18
#define OVERFLOW_MSG_BUF_SIZE		15
#define OVERRUN_MSG_BUF_SIZE		14
#define DEVICE_OFF_MSG_BUF_SIZE 	16
#define DATA_TIME_SET_MSG_BUF_SIZE	21
#define DEFAULT_STRING1				16
#define DEFAULT_STRING2				14
#define BME280_ENABLED_BUF_SIZE		24
#define BME280_NOT_ENABLED_BUF_SIZE 37
#define DS1307_ENABLED_BUF_SIZE		24
#define DS1307_NOT_ENABLED_BUF_SIZE	37


typedef enum{
	HM10_ERR_NONE = 0,
	HM10_ERR_RX_OVERFLOW = (1U << 0),
	HM10_ERR_RX_ORE = (1U << 1)
}hm10_error_t;

typedef enum{
	STATE_IDLE = 0,
	STATE_ACTIVE,
	STATE_STREAMING,
	STATE_ERROR
}app_state_t;

uint32_t hm10_get_errors(void);
void hm10_clear_errors(uint32_t errNO);

void hm10_init();
void hm10_send_data();
void hm10_recive_data();
uint8_t hm10_receive_dataIT(uint8_t*, uint8_t);
void hm10_send_dataIT(uint8_t*, uint8_t);


/*HM-10 user exposed APIs*/

void hm10_USART_pin_config();
void hm10_USART_config();
void USART_IRQHandling(USART_Handle_t*);
void hm10_receive_data(uint8_t*, uint8_t);
void hm10_reset_rx_buffer();

void usart_enable_RXIT();
void usart_enable_TXIT();
void usart_enable_TCIT();

uint8_t hm10_usart_txstate();

uint8_t command_parse_check();


/*HM-10 USART variables*/
extern volatile uint16_t rx_write_head;
extern volatile uint16_t rx_read_head;
extern  uint8_t rxBuf[RX_BUF_SIZE];

#endif /* HM10_H_ */
