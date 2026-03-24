/*
 * stm32f407xx_usart_driver.h
 *
 *  Created on: 31-Dec-2025
 *      Author: dipch
 */

#ifndef INC_STM32F407XX_USART_DRIVER_H_
#define INC_STM32F407XX_USART_DRIVER_H_
#include "stm32f407xx.h"

/*
 * Bit position definitions USART_CR1
 */
#define USART_CR1_SBK					0
#define USART_CR1_RWU 					1
#define USART_CR1_RE  					2
#define USART_CR1_TE 					3
#define USART_CR1_IDLEIE 				4
#define USART_CR1_RXNEIE  				5
#define USART_CR1_TCIE					6
#define USART_CR1_TXEIE					7
#define USART_CR1_PEIE 					8
#define USART_CR1_PS 					9
#define USART_CR1_PCE 					10
#define USART_CR1_WAKE  				11
#define USART_CR1_M 					12
#define USART_CR1_UE 					13
#define USART_CR1_OVER8  				15

/*
 * Bit position definitions USART_CR2
 */
#define USART_CR2_ADD   				0
#define USART_CR2_LBDL   				5
#define USART_CR2_LBDIE  				6
#define USART_CR2_LBCL   				8
#define USART_CR2_CPHA   				9
#define USART_CR2_CPOL   				10
#define USART_CR2_STOP   				12
#define USART_CR2_LINEN   				14


/*
 * Bit position definitions USART_CR3
 */
#define USART_CR3_EIE   				0
#define USART_CR3_IREN   				1
#define USART_CR3_IRLP  				2
#define USART_CR3_HDSEL   				3
#define USART_CR3_NACK   				4
#define USART_CR3_SCEN   				5
#define USART_CR3_DMAR  				6
#define USART_CR3_DMAT   				7
#define USART_CR3_RTSE   				8
#define USART_CR3_CTSE   				9
#define USART_CR3_CTSIE   				10
#define USART_CR3_ONEBIT   				11




/*
 *@USART_Mode
 *Possible options for USART_Mode
 */

#define USART_TX_ONLY	 0
#define USART_RX_ONLY 	 1
#define USART_TXRX  	 2

/*
 *@USART_Baud
 *Possible options for USART_Baud
 */
#define USART_STD_BAUD_1200					1200
#define USART_STD_BAUD_2400					2400
#define USART_STD_BAUD_9600					9600
#define USART_STD_BAUD_19200 				19200
#define USART_STD_BAUD_38400 				38400
#define USART_STD_BAUD_57600 				57600
#define USART_STD_BAUD_115200 				115200
#define USART_STD_BAUD_230400 				230400
#define USART_STD_BAUD_460800 				460800
#define USART_STD_BAUD_921600 				921600
#define USART_STD_BAUD_2M 					2000000
#define USART_STD_BAUD_3M 					3000000

/*
 *@USART_ParityControl
 *Possible options for USART_ParityControl
 */
#define USART_PARITY_EN_ODD   2
#define USART_PARITY_EN_EVEN  1
#define USART_PARITY_DISABLE  0

/*
 *@USART_WordLength
 *Possible options for USART_WordLength
 */
#define USART_WORDLEN_8BITS  0
#define USART_WORDLEN_9BITS  1


/*
 *@USART_NoOfStopBits
 *Possible options for USART_NoOfStopBits
 */
#define USART_STOPBITS_1     0
#define USART_STOPBITS_0_5   1
#define USART_STOPBITS_2     2
#define USART_STOPBITS_1_5   3

/*
 *@USART_HWFlowControl
 *Possible options for USART_HWFlowControl
 */
#define USART_HW_FLOW_CTRL_NONE    	0
#define USART_HW_FLOW_CTRL_CTS    	1
#define USART_HW_FLOW_CTRL_RTS    	2
#define USART_HW_FLOW_CTRL_CTS_RTS	3

/*
 * USART flags
 */

#define	FLAG_SET	1
#define FLAG_RESET	0

#define USART_SR_TXE        	7
#define USART_SR_RXNE        	5
#define USART_SR_TC        		6
#define USART_SR_ORE			3

#define USART_FLAG_TXE 			( 1 << USART_SR_TXE)
#define USART_FLAG_RXNE 		( 1 << USART_SR_RXNE)
#define USART_FLAG_TC 			( 1 << USART_SR_TC)
#define USART_FLAG_ORE 			( 1 << USART_SR_ORE)


#define USART_FLAG_RXNEIE		(1 << USART_CR1_RXNEIE)
#define USART_FLAG_TXEIE		(1 << USART_CR1_TXEIE)
#define USART_FLAG_TCIE			(1 << USART_CR1_TCIE)
/*
 * Application states
 */
#define USART_BUSY_IN_RX 1
#define USART_BUSY_IN_TX 2
#define USART_READY 0

#define USART_EVENT_RX_CMPLT 0



typedef struct
{
	uint8_t USART_Mode;
	uint32_t USART_Baud;
	uint8_t USART_NoOfStopBits;
	uint8_t USART_WordLength;
	uint8_t USART_ParityControl;
	uint8_t USART_HWFlowControl;
}USART_Config_t;



typedef struct
{
	USART_RegDef_t *pUSARTx;
	USART_Config_t   USART_Config;
	uint8_t *pTxBuffer;
	uint8_t *pRxBuffer;
	volatile uint32_t TxLen;
	volatile uint32_t RxLen;
	uint8_t TxBusyState;
	uint8_t RxBusyState;
}USART_Handle_t;



/*USART APIs*/

/*
 * Peripheral clock setup
 */
void USART_PClkCtrl(USART_RegDef_t *pUSARTx, uint8_t EnorDi);

/*
 * Initialization and de-Initialization of I2C
 */

void USART_Init(USART_Handle_t *pUSARTHandle);
void USART_DeInit(USART_RegDef_t* pUSARTx);

/*
 * Data send and receive
 */

void USART_SendData(USART_Handle_t *pUSARTHandle,uint8_t *pTxbuffer, uint32_t Len);
void USART_ReceiveData(USART_Handle_t *pUSARTHandle,uint8_t *pRxbuffer, uint32_t Len);
void USART_SendDataIT(USART_Handle_t *pUSARTHandle,uint8_t *pTxbuffer, uint32_t Len);
uint8_t USART_ReceiveDataIT(USART_Handle_t *pUSARTHandle,uint8_t *pRxbuffer, uint32_t Len);

/*
 * IRQ Configuration and ISR handling
 */

void USART_IRQInterruptConfig(uint8_t IRQNumber,uint8_t EnorDi);
void USART_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);
void USART_IRQHandling(USART_Handle_t *pHandle);

/*
 * OTHER APIs
 */

void USART_PeripheralControl(USART_RegDef_t* pUSARTx,uint8_t EnorDi);
uint8_t USART_GetFlagStatus(USART_RegDef_t* pUSARTx,uint32_t FlagName);
void USART_SetBaudRate(USART_RegDef_t *pUSARTx, uint32_t BaudRate);

void USART_ApplicationEventCallback(USART_Handle_t*,uint8_t);
void usart_RXNE_status();

#endif /* INC_STM32F407XX_USART_DRIVER_H_ */
