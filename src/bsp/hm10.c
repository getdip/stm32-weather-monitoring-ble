/*
 * hm10.c
 *
 *  Created on: 16-Jan-2026
 *      Author: dipch
 */

#include "hm10.h"
#include <string.h>

USART_Handle_t hm10_usartHandle;

volatile uint8_t tick_flag;

static volatile uint8_t data;
volatile uint16_t rx_write_head;
volatile uint16_t rx_read_head;
static volatile uint16_t rx_write_head_next;


/*Error flag variable*/
static volatile uint32_t hm10_error_flag = HM10_ERR_NONE;

void hm10_init(){

	hm10_USART_pin_config();

	hm10_USART_config();

	USART_PeripheralControl(HM10_USART, ENABLE);
}



void hm10_USART_pin_config(){

	GPIO_Handle_t USART_GPIO;

	/*Pin description
	 * TX --> PA9
	 * RX --> PA10
	 * */

	USART_GPIO.pGPIOx = HM10_USART_GPIO_PORT;
	USART_GPIO.GPIO_PinConfig.GPIO_PinAltFunMode = 7;
	USART_GPIO.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	USART_GPIO.GPIO_PinConfig.GPIO_PinNumber = HM10_USART_TX_PIN;
	USART_GPIO.GPIO_PinConfig.GPIO_PinOPType = HM10_USART_PIN_OP_TYPE;
	USART_GPIO.GPIO_PinConfig.GPIO_PinPuPdControl = HM10_USART_PIN_PUPD;
	USART_GPIO.GPIO_PinConfig.GPIO_PinSpeed = HM10_USART_PIN_SPEED;

	GPIO_Init(&USART_GPIO);
	USART_GPIO.GPIO_PinConfig.GPIO_PinNumber = HM10_USART_RX_PIN;
	GPIO_Init(&USART_GPIO);
}



void hm10_USART_config(){

	hm10_usartHandle.pUSARTx = HM10_USART;
	hm10_usartHandle.USART_Config.USART_Mode = HM10_MODE;
	hm10_usartHandle.USART_Config.USART_Baud = HM10_BAUD;
	hm10_usartHandle.USART_Config.USART_NoOfStopBits = HM10_STOP_BITS;
	hm10_usartHandle.USART_Config.USART_ParityControl = HM10_PARITY;
	hm10_usartHandle.USART_Config.USART_WordLength = HM10_WORD_LEN;
	hm10_usartHandle.USART_Config.USART_HWFlowControl = HM10_HW_FLOW;

	USART_Init(&hm10_usartHandle);
}

void hm10_receive_data(uint8_t* rxBuf, uint8_t Len){
	USART_ReceiveData(&hm10_usartHandle, rxBuf, Len);
}


void USART2_IRQHandler(void)
{

	if(hm10_usartHandle.pUSARTx->SR & USART_FLAG_ORE){
		//Dummy read DR to clear ORE
		uint8_t dummy = hm10_usartHandle.pUSARTx->DR;
		hm10_error_flag |= HM10_ERR_RX_ORE;
	}

	if(hm10_usartHandle.pUSARTx->SR & USART_FLAG_RXNE){
		data = hm10_usartHandle.pUSARTx->DR;
		rx_write_head_next =  (rx_write_head + 1) & (RX_BUF_SIZE -1);

		if(rx_write_head_next == rx_read_head){
			hm10_error_flag |= HM10_ERR_RX_OVERFLOW;
		}
		else{
			rxBuf[rx_write_head] = data;
			rx_write_head = rx_write_head_next;
		}
	}


	if((hm10_usartHandle.pUSARTx->SR & USART_FLAG_TXE) && (hm10_usartHandle.pUSARTx->CR1 & USART_FLAG_TXEIE)){

		if(hm10_usartHandle.TxLen > 0){
			hm10_usartHandle.pUSARTx->DR = (uint8_t)(*(hm10_usartHandle.pTxBuffer));
			hm10_usartHandle.TxLen--;
			hm10_usartHandle.pTxBuffer++;
		}

		if(hm10_usartHandle.TxLen == 0){
			hm10_usartHandle.pUSARTx->CR1 &= ~USART_FLAG_TXEIE;
			hm10_usartHandle.pUSARTx->CR1 |= USART_FLAG_TCIE;
		}
	}

	if ((hm10_usartHandle.pUSARTx->SR & USART_FLAG_TC) && (hm10_usartHandle.pUSARTx->CR1 & USART_FLAG_TCIE)) {
		hm10_usartHandle.pUSARTx->CR1 &= ~USART_FLAG_TCIE;
		hm10_usartHandle.TxBusyState = USART_READY;
	}
}

uint8_t hm10_usart_txstate(){

	return hm10_usartHandle.TxBusyState;

}

uint8_t hm10_receive_dataIT(uint8_t* rxBuf, uint8_t Len){

	return USART_ReceiveDataIT(&hm10_usartHandle, rxBuf, Len);
}

void hm10_send_dataIT(uint8_t*txBuf, uint8_t Len){
	 USART_SendDataIT(&hm10_usartHandle,txBuf,Len);
}

void usart_RXNE_status(){
	if(hm10_usartHandle.pUSARTx->SR & USART_FLAG_RXNE){
		*NVIC_ISPR1 |=(1<<5);
		while(1);
	}
}

void usart_enable_RXIT(){
	hm10_usartHandle.pUSARTx->CR1 |= (1<<USART_CR1_RXNEIE);
}

void usart_enable_TXIT(){
	hm10_usartHandle.pUSARTx->CR1 |= (1<<USART_CR1_TXEIE);
}

void usart_enable_TCIT(){
	hm10_usartHandle.pUSARTx->CR1 |= (1<<USART_CR1_TCIE);
}


/*Error Handling*/

uint32_t hm10_get_errors(){
	return hm10_error_flag;
}

void hm10_clear_errors(uint32_t errNO){
	hm10_error_flag &= ~errNO;
}

void hm10_reset_rx_buffer(){
	memset(rxBuf,0,RX_BUF_SIZE);
	rx_write_head = rx_read_head;
}

