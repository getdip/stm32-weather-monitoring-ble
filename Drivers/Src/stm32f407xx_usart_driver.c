/*
 * stm32f407xx_usart_driver.c
 *
 *  Created on: 01-Jan-2026
 *      Author: dipch
 */
#include "stm32f407xx_usart_driver.h"
#include <stdio.h>



void USART_PClkCtrl(USART_RegDef_t *pUSARTx, uint8_t EnorDi){

	if(EnorDi == ENABLE){

			if(pUSARTx == USART1){

				USART1_PCLK_EN();
			}

			else if(pUSARTx == USART2)
					{
				USART2_PCLK_EN();
					}
			else if(pUSARTx == USART3)
					{
				USART3_PCLK_EN();
					}
			else if(pUSARTx == UART4){
				UART4_PCLK_EN();
			}

			else if(pUSARTx == UART5){
				UART5_PCLK_EN();
			}

			else if(pUSARTx == USART6){
				USART6_PCLK_EN();
			}

		}

		else{


			if(pUSARTx == USART1)
						{
				USART1_PCLK_DI();
						}
			else if(pUSARTx == USART2)
					{
				USART2_PCLK_DI();
					}
			else if(pUSARTx == USART3)
					{
				USART3_PCLK_DI();
					}
			else if(pUSARTx == UART4){
				UART4_PCLK_DI();
			}

			else if(pUSARTx == UART5){
				UART5_PCLK_DI();
			}

			else if(pUSARTx == USART6){
				USART6_PCLK_DI();
			}
		}
}


/*USART initialization*/

void USART_Init(USART_Handle_t *pUSARTHandle){

	/*Enable the peripheral*/
	USART_PClkCtrl(pUSARTHandle->pUSARTx, ENABLE);

	/*Mode configuration*/

	if(pUSARTHandle->USART_Config.USART_Mode == USART_TX_ONLY){
		pUSARTHandle->pUSARTx->CR1 |= (1<<USART_CR1_TE);
	}

	else if (pUSARTHandle->USART_Config.USART_Mode == USART_RX_ONLY){
		pUSARTHandle->pUSARTx->CR1 |= (1<<USART_CR1_RE);
	}

	else if (pUSARTHandle->USART_Config.USART_Mode == USART_TXRX){
		pUSARTHandle->pUSARTx->CR1 |= (1<<USART_CR1_TE);
		pUSARTHandle->pUSARTx->CR1 |= (1<<USART_CR1_RE);
	}

	/*Word length configuration*/
	if(pUSARTHandle->USART_Config.USART_WordLength == USART_WORDLEN_8BITS){
		pUSARTHandle->pUSARTx->CR1 &= (~(1<<USART_CR1_M));
	}
	else if(pUSARTHandle->USART_Config.USART_WordLength == USART_WORDLEN_9BITS){
		pUSARTHandle->pUSARTx->CR1 |= (1<<USART_CR1_M);
	}

	/*Parity configuration*/
	if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE){
		pUSARTHandle->pUSARTx->CR1 &= (~(1<<USART_CR1_PCE));
	}

	else if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_EN_EVEN){
		pUSARTHandle->pUSARTx->CR1 |= (1<<USART_CR1_PCE); // Parity is enabled
		pUSARTHandle->pUSARTx->CR1 &= (~(1<<USART_CR1_PS));
	}

	else if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_EN_ODD){
			pUSARTHandle->pUSARTx->CR1 |= (1<<USART_CR1_PCE); // Parity is enabled
			pUSARTHandle->pUSARTx->CR1 |= (1<<USART_CR1_PS);
	}

	/*Stop bit width configuration*/
	uint32_t temp;

	// Reset bits 12 & 13 of CR2 register
	temp = 3;
	pUSARTHandle->pUSARTx->CR2 &= (~(temp << USART_CR2_STOP));
	pUSARTHandle->pUSARTx->CR2 |= (pUSARTHandle->USART_Config.USART_NoOfStopBits << USART_CR2_STOP );


	/*Hardware flow control*/
	if(pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_NONE){

		pUSARTHandle->pUSARTx->CR3 &= (~(1<<USART_CR3_RTSE));
		pUSARTHandle->pUSARTx->CR3 &= (~(1<<USART_CR3_CTSE));
	}

	else if(pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_CTS){
		 pUSARTHandle->pUSARTx->CR3 |= (1<<USART_CR3_CTSE);
	}

	else if(pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_RTS){
		pUSARTHandle->pUSARTx->CR3 |= (1<<USART_CR3_RTSE);
	}

	else if(pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_CTS_RTS){
		pUSARTHandle->pUSARTx->CR3 |= (1<<USART_CR3_CTSE);
		pUSARTHandle->pUSARTx->CR3 |= (1<<USART_CR3_RTSE);
	}

	/*Baud rate configuration */

	USART_SetBaudRate(pUSARTHandle->pUSARTx, pUSARTHandle->USART_Config.USART_Baud);
}

/*USART peripheral control configuration*/

void USART_PeripheralControl(USART_RegDef_t* pUSARTx,uint8_t EnorDi){

	if(EnorDi == ENABLE){
		pUSARTx->CR1 |= (1<<13);
	}
	else if(EnorDi == DISABLE){
		pUSARTx->CR1 &= (~(1<<13));
	}
}


/*USART get flag status*/

uint8_t USART_GetFlagStatus(USART_RegDef_t* pUSARTx , uint32_t FlagName)
{
	if(pUSARTx->SR & FlagName)
	{
		return FLAG_SET;
	}

	return FLAG_RESET;
}



/*USART IRQ configuration*/

void USART_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi)
{

	if(EnorDi == ENABLE)
	{
		if(IRQNumber <= 31)
		{
			//program ISER0 register
			*NVIC_ISER0 |= ( 1 << IRQNumber );

		}else if(IRQNumber > 31 && IRQNumber < 64 ) //32 to 63
		{
			//program ISER1 register
			*NVIC_ISER1 |= ( 1 << (IRQNumber % 32) );
		}
		else if(IRQNumber >= 64 && IRQNumber < 96 )
		{
			//program ISER2 register //64 to 95
			*NVIC_ISER3 |= ( 1 << (IRQNumber % 64) );
		}
	}

}

/*Interrupt priority configuration*/

void USART_IRQPriorityConfig(uint8_t IRQNumber,uint32_t IRQPriority)
{
	//1. first lets find out the ipr register
	uint8_t iprx = IRQNumber / 4;
	uint8_t iprx_section  = IRQNumber %4 ;

	uint8_t shift_amount = ( 8 * iprx_section) + ( 8 - PR_BITS_IMPLEMENTED) ;

	*(  NVIC_IPR_BASEADDR + iprx ) |=  ( IRQPriority << shift_amount );

}

void USART_SendData(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer, uint32_t Len)
{

	uint16_t *pdata;

   //Loop over until "Len" number of bytes are transferred
	for(uint32_t i = 0 ; i < Len; i++)
	{
		//Implement the code to wait until TXE flag is set in the SR
		while(! USART_GetFlagStatus(pUSARTHandle->pUSARTx,USART_FLAG_TXE));

		//Check the USART_WordLength item for 9BIT or 8BIT in a frame
		if(pUSARTHandle->USART_Config.USART_WordLength == USART_WORDLEN_9BITS)
		{

			//check for USART_ParityControl
			if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE)
			{
				//No parity is used in this transfer , so 9bits of user data will be sent
				//Implement the code to increment pTxBuffer twice
				//if 9BIT load the DR with 2bytes masking  the bits other than first 9 bits
				pdata = (uint16_t*) pTxBuffer;
				pUSARTHandle->pUSARTx->DR = (*pdata & (uint16_t)0x01FF);

				pTxBuffer++;
			}
			else
			{
				//Parity bit is used in this transfer . so 8bits of user data will be sent
				//The 9th bit(the bit after MSB of data) will be replaced by parity bit by the hardware
				pUSARTHandle->pUSARTx->DR = (*pTxBuffer  & (uint8_t)0xFF);
				pTxBuffer++;
			}
		}
		else
		{	//8 Bit data transfer
			//This is 8bit data transfer
			pUSARTHandle->pUSARTx->DR = (*pTxBuffer  & (uint8_t)0xFF);

			//Implement the code to increment the buffer address
			pTxBuffer++;
		}
	}

	//Implement the code to wait till TC flag is set in the SR
	while( ! USART_GetFlagStatus(pUSARTHandle->pUSARTx,USART_FLAG_TC));
}

void USART_ReceiveData(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Len){

	uint16_t *pdata;

	   //Loop over until "Len" number of bytes are Received OR terminating character '\n' or '\r' is received
		for(uint32_t i = 0 ; i < Len; i++)
		{
			//Implement the code to wait until RxnE flag is set in the SR
			while(! USART_GetFlagStatus(pUSARTHandle->pUSARTx,USART_FLAG_RXNE));

			//Check the USART_WordLength item for 9BIT or 8BIT in a frame
			if(pUSARTHandle->USART_Config.USART_WordLength == USART_WORDLEN_9BITS)
			{

				//check for USART_ParityControl
				if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE)
				{
					//No parity is used in this transfer , so 9bits of user data will be received
					//if 9BIT load the DR with 2bytes masking  the bits other than first 9 bits
					pdata = (uint16_t*) pRxBuffer;
				   *pdata = ((pUSARTHandle->pUSARTx->DR) & (uint16_t)0x01FF) ;

				   //pRxBuffer++;
				}
				else
				{
					//Parity bit is used in this transfer . so only 8bits of user data(from LSB) will be considered
					//The 9th bit(the bit after MSB of data) will be replaced by parity bit by the hardware
					*pRxBuffer = ((pUSARTHandle->pUSARTx->DR)  & (uint8_t)0xFF);
					//pRxBuffer++;
				}
			}
			else
			{	//8 Bit data reception
				//This is 8bit data reception

				//With parity control disabled - all 8bitsof data must be considered
				if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE){

					*pRxBuffer = ((pUSARTHandle->pUSARTx->DR)  & (uint8_t)0xFF);

				}
				else{
					// With parity enabled - only 7bits of data(fromLSB) must be considered
					*pRxBuffer = ((pUSARTHandle->pUSARTx->DR)  & (uint8_t)0x7F);
					//Implement the code to increment the buffer address
					//pRxBuffer++;

				}
			}

			if ((*pRxBuffer == '#') || (*pRxBuffer == '\n')){

				printf("Termination detected\n");
				*pRxBuffer = '\0';
				break;
			}

			else pRxBuffer++; //Increment the buffer address
		}

		printf("Recived msg: %s\n",pRxBuffer);
}


void USART_SendDataIT(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer, uint32_t Len){

	//uint8_t temp_tx_state = pUSARTHandle->TxBusyState;

	if(pUSARTHandle->TxBusyState != USART_BUSY_IN_TX){
		pUSARTHandle->TxLen = Len;
		pUSARTHandle->pTxBuffer = pTxBuffer;
		pUSARTHandle->TxBusyState = USART_BUSY_IN_TX;

		pUSARTHandle->pUSARTx->CR1 |= (1<<USART_CR1_TXEIE);
	}
	//return temp_tx_state;
}

uint8_t USART_ReceiveDataIT(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Len){

	uint8_t temp_rx_state = pUSARTHandle->RxBusyState;
	//volatile uint32_t temp;

	if(pUSARTHandle->RxBusyState != USART_BUSY_IN_RX){
		pUSARTHandle->RxLen = Len;
		pUSARTHandle->pRxBuffer = pRxBuffer;
		pUSARTHandle->RxBusyState = USART_BUSY_IN_RX;
		//temp = pUSARTHandle->pUSARTx->SR;
		//temp = pUSARTHandle->pUSARTx->DR;
		//(void)temp;
		//(void)pUSARTHandle->pUSARTx->DR;
		//pUSARTHandle->pUSARTx->CR1 |= (1<<USART_CR1_RXNEIE);
		//rxcmplt = FLAG_RESET;
	}

	return temp_rx_state;
}

void USART_SetBaudRate(USART_RegDef_t *pUSARTx, uint32_t BaudRate){

	uint32_t pCLK;

	uint32_t div_M, div_F;

	uint32_t dummy = 0;

	if((pUSARTx == USART1) | (pUSARTx == USART6)){
		pCLK = RCC_GetPCLK2Value();
	}

	else{
			pCLK = RCC_GetPCLK1Value();
	}

	if((pUSARTx->CR1) & (1<<USART_CR1_OVER8)){
		//Over-sampling by 8
		dummy = ((pCLK*25) /(2*BaudRate)) ;

		div_M = (dummy / 100);

		div_F = (dummy - (div_M * 100));
		div_F = ((div_F * 8) + 50)/100;
	}

	else if (!((pUSARTx->CR1) & (1<<USART_CR1_OVER8))){
		//Over-sampling by 16
		dummy = ((pCLK*25) /(4*BaudRate));

		div_M = (dummy / 100);

		div_F = (dummy - (div_M * 100));
		div_F = ((div_F * 16) + 50)/100;
	}

	pUSARTx->BRR &= 0xFFFF0000;

	pUSARTx->BRR |= ((div_M << 4) & 0x0000FFF0);
	pUSARTx->BRR |= (div_F & 0x0000000F);
}


void USART_IRQHandling(USART_Handle_t* pUSARTHandle){
	//printf("blocking...\n");
	uint8_t fl1, fl2;

	/*Interrupt due to TXE flag is set*/
	fl1 = USART_GetFlagStatus(pUSARTHandle->pUSARTx,USART_FLAG_TXE);
	fl2 = USART_GetFlagStatus(pUSARTHandle->pUSARTx, USART_FLAG_TXEIE);

	if(fl1 & fl2){

		if(pUSARTHandle->TxBusyState == USART_BUSY_IN_TX ){

			if(pUSARTHandle->TxLen >0){

				if(pUSARTHandle->USART_Config.USART_WordLength == USART_WORDLEN_9BITS){

					if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE){
						uint16_t* dummy;
						dummy = (uint16_t*)pUSARTHandle->pTxBuffer;
						pUSARTHandle->pUSARTx->DR = *dummy & (uint16_t)0x01FF;
						pUSARTHandle->pTxBuffer++;
					}

					else{//9bit data with parity enabled
						pUSARTHandle->pUSARTx->DR = ((*pUSARTHandle->pTxBuffer) & (uint8_t)0xFF);
						pUSARTHandle->pTxBuffer++;
					}
				}
				else{ //8bit data transmission without parity bit
					if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE){
						pUSARTHandle->pUSARTx->DR = ((*pUSARTHandle->pTxBuffer) & (uint8_t)0xFF);
						pUSARTHandle->pTxBuffer++;
					}

					else{//8bit data transmission with parity bit enabled
						uint8_t mdata;
						mdata = *pUSARTHandle->pTxBuffer;
						pUSARTHandle->pUSARTx->DR = mdata & 0x7F;
						pUSARTHandle->pTxBuffer++;
					}
				}
			pUSARTHandle->TxLen = pUSARTHandle->TxLen - 1;
			}
		}
	}
	/********************************************************************************************/

	/*Interrupt due to RXNE flag is set*/
		fl1 = USART_GetFlagStatus(pUSARTHandle->pUSARTx,USART_FLAG_RXNE);
		fl2 = USART_GetFlagStatus(pUSARTHandle->pUSARTx, USART_FLAG_RXNEIE);


		if(fl1 & fl2){

			if(pUSARTHandle->RxBusyState == USART_BUSY_IN_RX ){

				if(pUSARTHandle->RxLen >0){

					if(pUSARTHandle->USART_Config.USART_WordLength == USART_WORDLEN_9BITS){
						if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE){

							uint16_t* dummy;
							dummy = (uint16_t*)pUSARTHandle->pRxBuffer;
							*dummy = (pUSARTHandle->pUSARTx->DR) & (uint16_t)0x01FF;
						}

						else{//9bit data with parity enabled
							*pUSARTHandle->pRxBuffer = ((pUSARTHandle->pUSARTx->DR) & (uint8_t)0xFF);
							//pUSARTHandle->pRxBuffer++;
						}
					}
					else{ //8bit data
						if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE){
							*pUSARTHandle->pRxBuffer = ((pUSARTHandle->pUSARTx->DR) & (uint8_t)0xFF);
							//pUSARTHandle->pRxBuffer++;

						}

						else{
							//uint8_t* mdata;
							*pUSARTHandle->pRxBuffer = pUSARTHandle->pUSARTx->DR & 0x7F;
						}
					}

					if ((*pUSARTHandle->pRxBuffer == '#') || (*pUSARTHandle->pRxBuffer == '\n')){

						//printf("Termination detected\n");
						*pUSARTHandle->pRxBuffer = '\0';
						pUSARTHandle->RxBusyState = USART_READY;
					}

					pUSARTHandle->RxLen = pUSARTHandle->RxLen - 1;
					pUSARTHandle->pRxBuffer++;
				}

				else{

					pUSARTHandle->pUSARTx->CR1 &= ~(1 << USART_CR1_RXNEIE);
					pUSARTHandle->RxBusyState = USART_READY;
					//pUSARTHandle->pRxBuffer = NULL;
					//A global flag will be set which means all bytes have been received
					USART_ApplicationEventCallback(pUSARTHandle,USART_EVENT_RX_CMPLT);
				}
			}

		}

/**********************************************************************************************************/

	/*Interrupt due to Transmission Complete (TC) */

	fl1 = USART_GetFlagStatus(pUSARTHandle->pUSARTx,USART_FLAG_TC);
	fl2 = USART_GetFlagStatus(pUSARTHandle->pUSARTx, USART_FLAG_TCIE);

	if (fl1 & fl2){

		if(pUSARTHandle->TxBusyState != USART_BUSY_IN_TX){

			if(!pUSARTHandle->TxLen){

				pUSARTHandle->pUSARTx->SR &= ~(1 << USART_SR_TC);

				pUSARTHandle->TxBusyState = USART_READY;

				pUSARTHandle->pTxBuffer = NULL;

			}

		}

	}
}


/*__weak void USART_ApplicationEventCallback(USART_Handle_t *pUSARTHandle,uint8_t event)
{

}*/

