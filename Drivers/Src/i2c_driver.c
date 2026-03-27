/*
 * i2c_driver.c
 *
 *  Created on: Jan 26, 2025
 *      Author: dipch
 */

#include "stm32f407xx_i2c_driver.h"

static void delay_us(uint32_t);
void I2C_Bus_Recovery(I2C_RegDef_t*,GPIO_Handle_t*,GPIO_Handle_t*);

static void  I2C_GenerateStartCondition(I2C_RegDef_t *pI2Cx);
static void I2C_ExecuteAddressPhaseWrite(I2C_RegDef_t *pI2Cx, uint8_t SlaveAddr);
static void I2C_ExecuteAddressPhaseRead(I2C_RegDef_t *pI2Cx, uint8_t SlaveAddr);
static void I2C_ClearADDRFlag(I2C_RegDef_t*);

static inline void NOP(void){
	__asm volatile("nop");
}


static void delay_us(uint32_t us){

	while(us--){
		NOP();
		NOP();
		NOP();
		NOP();
	}
}

void I2C_Bus_Recovery(I2C_RegDef_t* pI2Cx,GPIO_Handle_t* pSDA,GPIO_Handle_t* pSCL){

	/*Disable the I2C peripheral*/
	pI2Cx->I2C_CR1 &= ~(1<<I2C_CR1_PE);

	/*Configure SCL as GPIO output and OD*/
	pSCL->GPIO_PinConfig.GPIO_PinAltFunMode = 0;
	pSCL->GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	pSCL->GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;

	GPIO_Init(pSCL);

	/*Configure SDA as GPIO input*/
	pSDA->GPIO_PinConfig.GPIO_PinAltFunMode= 0;
	pSDA->GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	pSDA->GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
	GPIO_Init(pSDA);

	/*Toggle SCL 9 Times*/

	for(uint8_t i=0;i<9;i++){
		pSCL->pGPIOx->BSRR = (1 << ((pSCL->GPIO_PinConfig.GPIO_PinNumber)+16)); //SCL- Low
		delay_us(5);

		pSCL->pGPIOx->BSRR = (1 << (pSCL->GPIO_PinConfig.GPIO_PinNumber));//SCL- High
	}

	/*Generate STOP manually*/
	/*Get SDA pin in output-OD mode*/
	pSDA->GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	pSDA->GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;
	GPIO_Init(pSDA);

	pSDA->pGPIOx->BSRR = (1 << ((pSDA->GPIO_PinConfig.GPIO_PinNumber)+16)); //SDA- Low
	GPIO_Init(pSDA);
	delay_us(5);

	pSCL->pGPIOx->BSRR = (1 << (pSCL->GPIO_PinConfig.GPIO_PinNumber));//SCL- High
	GPIO_Init(pSCL);
	delay_us(5);

	pSDA->pGPIOx->BSRR = (1 << (pSDA->GPIO_PinConfig.GPIO_PinNumber));//SDA- High
	GPIO_Init(pSDA);
	delay_us(5);

	/*Enable the I2C peripheral*/
	pI2Cx->I2C_CR1 |= (1<<I2C_CR1_PE);
	/*!!!Restore the SCL and SDA pins initial state from the main call*/

}


static void I2C_GenerateStartCondition(I2C_RegDef_t *pI2Cx)
{
	pI2Cx->I2C_CR1 |= ( 1 << I2C_CR1_START);

}

void I2C_GenerateStopCondition(I2C_RegDef_t *pI2Cx)
{
	pI2Cx->I2C_CR1 |= ( 1 << I2C_CR1_STOP);
}


static void I2C_ExecuteAddressPhaseWrite(I2C_RegDef_t *pI2Cx, uint8_t SlaveAddr)
{
	//SlaveAddr = SlaveAddr << 1;
	//SlaveAddr &= ~(1); //SlaveAddr is Slave address + r/nw bit=0
	pI2Cx->I2C_DR = SlaveAddr << 1 ;
}


static void I2C_ExecuteAddressPhaseRead(I2C_RegDef_t *pI2Cx, uint8_t SlaveAddr)
{
	SlaveAddr = SlaveAddr << 1;
	SlaveAddr |= 1; //SlaveAddr is Slave address + r/nw bit=1
	pI2Cx->I2C_DR = SlaveAddr;
}


static void I2C_ClearADDRFlag(I2C_RegDef_t *pI2Cx){
	//As per manual, to clear the ADDR bit, first: the SR1 and then the SR2 register must be read
	uint32_t dummyRead = pI2Cx->I2C_SR1;
	dummyRead = pI2Cx->I2C_SR2;
	(void)dummyRead;
}


/*static void I2C_ClearADDRFlag(I2C_Handle_t *pI2CHandle )
{
	uint32_t dummy_read;
	//check for device mode
	if(pI2CHandle->pI2Cx->I2C_SR2 & ( 1 << I2C_SR2_MSL))
	{
		//device is in master mode
		if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
		{
			if(pI2CHandle->RxSize  == 1)
			{
				//first disable the ack
				I2C_ManageAcking(pI2CHandle->pI2Cx,DISABLE);

				//clear the ADDR flag ( read SR1 , read SR2)
				dummy_read = pI2CHandle->pI2Cx->SR1;
				dummy_read = pI2CHandle->pI2Cx->SR2;
				(void)dummy_read;
			}

		}
		else
		{
			//clear the ADDR flag ( read SR1 , read SR2)
			dummy_read = pI2CHandle->pI2Cx->SR1;
			dummy_read = pI2CHandle->pI2Cx->SR2;
			(void)dummy_read;

		}

	}
	else
	{
		//device is in slave mode
		//clear the ADDR flag ( read SR1 , read SR2)
		dummy_read = pI2CHandle->pI2Cx->SR1;
		dummy_read = pI2CHandle->pI2Cx->SR2;
		(void)dummy_read;
	}


}*/






void I2C_ManageAcking(I2C_RegDef_t *pI2Cx, uint8_t EnorDi)
{
	if(EnorDi == I2C_ACK_ENABLE)
	{
		//enable the ack
		pI2Cx->I2C_CR1 |= ( 1 << I2C_CR1_ACK);
	}else
	{
		//disable the ack
		pI2Cx->I2C_CR1 &= ~( 1 << I2C_CR1_ACK);
	}
}


void I2C_PClkCtrl(I2C_RegDef_t* pI2Cx, uint8_t EnorDi){

	if(EnorDi == ENABLE){

		if(pI2Cx == I2C1){

			I2C1_PCLK_EN();
		}

		else if(pI2Cx == I2C2)
				{
			I2C2_PCLK_EN();
				}
		else if(pI2Cx == I2C3)
				{
			I2C3_PCLK_EN();
				}

	}

	else{


		if(pI2Cx == I2C1){

					I2C1_PCLK_DI();
				}

		else if(pI2Cx == I2C2){

					I2C2_PCLK_DI();
						}
		else if(pI2Cx == I2C3){

					I2C3_PCLK_DI();
						}
		}
}




//////////////////		I2C_PeripheralControl		////////

void I2C_PeripheralControl(I2C_RegDef_t *pI2Cx, uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE)
	{
		pI2Cx->I2C_CR1 |= (1 << I2C_CR1_PE);		//setting  the PE bit of CR1 register
		//pI2cBaseAddress->CR1 |= I2C_CR1_PE_Bit_Mask;
	}else
	{
		pI2Cx->I2C_CR1 &= ~(1 << 0);
	}

}

///////////////////////////////////////////////////////////////////////

///////////		IRQ Interrupt Configuration		//////////////////

void I2C_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi)
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
	}else
	{
		if(IRQNumber <= 31)
		{
			//program ICER0 register
			*NVIC_ICER0 |= ( 1 << IRQNumber );
		}else if(IRQNumber > 31 && IRQNumber < 64 )
		{
			//program ICER1 register
			*NVIC_ICER1 |= ( 1 << (IRQNumber % 32) );
		}
		else if(IRQNumber >= 6 && IRQNumber < 96 )
		{
			//program ICER2 register
			*NVIC_ICER3 |= ( 1 << (IRQNumber % 64) );
		}
	}

}


//////////////////////////////////////////////////////////
//////////////// 		Interrupt Priority Configuration		////////////////////////////////

void I2C_IRQPriorityConfig(uint8_t IRQNumber,uint32_t IRQPriority)
{
	//1. first lets find out the ipr register
	uint8_t iprx = IRQNumber / 4;
	uint8_t iprx_section  = IRQNumber %4 ;

	uint8_t shift_amount = ( 8 * iprx_section) + ( 8 - PR_BITS_IMPLEMENTED) ;

	*(  NVIC_IPR_BASEADDR + iprx ) |=  ( IRQPriority << shift_amount );

}

//////////////////////////////////////////////////////////////




//////////////		I2C INITIALISATION		///////////////////

void I2C_Init(I2C_Handle_t *pI2CHandle){

	uint32_t tempreg = 0 ;

		//enable the clock for the i2cx peripheral
	I2C_PClkCtrl(pI2CHandle->pI2Cx,ENABLE);

		//Enable peripjeral

	pI2CHandle->pI2Cx->I2C_CR1 |=(1<<I2C_CR1_PE);

		//set the ACK control bit
	pI2CHandle->pI2Cx->I2C_CR1 |=(1<<I2C_CR1_ACK);



		//configure the FREQ field of CR2
		tempreg = 0;
		tempreg |= RCC_GetPCLK1Value() /1000000U ;
		pI2CHandle->pI2Cx->I2C_CR2 =  (tempreg & 0x3F);



		//program the device own address (required when act as Slave)
		tempreg = 0;
		tempreg |= pI2CHandle->I2C_Config.I2C_DeviceAddress << 1;
		tempreg |= ( 1 << 14); 						// as per reference manual
		pI2CHandle->pI2Cx->I2C_OAR1 = tempreg;

		//CCR calculations
			uint16_t ccr_value = 0;
			tempreg = 0;
			if(pI2CHandle->I2C_Config.I2C_SCLSpeed <= I2C_SCL_SPEED_SM)
			{
				//mode is standard mode
				ccr_value = (RCC_GetPCLK1Value() / ( 2 * pI2CHandle->I2C_Config.I2C_SCLSpeed ) );
				tempreg |= (ccr_value & 0xFFF);
			}else
			{
				//mode is fast mode
				tempreg |= ( 1 << 15);
				tempreg |= (pI2CHandle->I2C_Config.I2C_FMDutyCycle << 14);
				if(pI2CHandle->I2C_Config.I2C_FMDutyCycle == I2C_FM_DUTY_2)
				{
					ccr_value = (RCC_GetPCLK1Value() / ( 3 * pI2CHandle->I2C_Config.I2C_SCLSpeed ) );
				}else
				{
					ccr_value = (RCC_GetPCLK1Value() / ( 25 * pI2CHandle->I2C_Config.I2C_SCLSpeed ) );
				}
				tempreg |= (ccr_value & 0xFFF);
			}
			pI2CHandle->pI2Cx->I2C_CCR = tempreg;




			//TRISE Configuration
			if(pI2CHandle->I2C_Config.I2C_SCLSpeed <= I2C_SCL_SPEED_SM)
			{
				//mode is standard mode

				tempreg = (RCC_GetPCLK1Value() /1000000U) + 1 ;

			}else
			{
				//mode is fast mode
				tempreg = ( (RCC_GetPCLK1Value() * 300) / 1000000000U ) + 1;

			}

			pI2CHandle->pI2Cx->I2C_TRISE = (tempreg & 0x3F);

	}


////////////////////		Master Send Data	/////////////////////////




uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx , uint32_t FlagName)
{
	if(pI2Cx->I2C_SR1 & FlagName)
	{
		return FLAG_SET;
	}


	return FLAG_RESET;
}


/////////////////////////		I2c Master data transmit		////////////////////////


void I2C_MasterSendData(I2C_Handle_t *pI2CHandle,uint8_t *pTxbuffer, uint32_t Len, uint8_t SlaveAddr,uint8_t Sr)
{
	uint32_t byteCnt = Len;
	// Wait for BUSY flag get RESET
	while(pI2CHandle->pI2Cx->I2C_SR2 & (1U << I2C_SR2_BUSY));

	// 1. Generate the START condition
	I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

	//2. confirm that start generation is completed by checking the SB flag in the SR1 -status register
	//   Note: We have to wait until SB = 1
	while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_SB));

	//3. Send the address of the slave with r/nw bit set to w(0) (total 8 bits )
	I2C_ExecuteAddressPhaseWrite(pI2CHandle->pI2Cx,SlaveAddr);


	//4. Confirm that address phase is completed by checking the ADDR flag in the SR1
	//   Note: We have to wait until ADDR = 1
	while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_ADDR));

	//5. clear the ADDR flag according to its software sequence
	//   Note: Until ADDR is cleared SCL will be stretched (pulled to LOW)
	I2C_ClearADDRFlag(pI2CHandle->pI2Cx);

	//6. send the data until len becomes 0

	while(Len > 0)
	{
		while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_TXE) ); //Wait till TXE is set
		pI2CHandle->pI2Cx->I2C_DR = *pTxbuffer;
		pTxbuffer++;
		Len--;
	}

	//7. when Len becomes zero wait for TXE=1 and BTF=1 before generating the STOP condition
	//   Note: TXE=1 , BTF=1 , means that both SR and DR are empty and next transmission should begin
	//   when BTF=1 SCL will be stretched (pulled to LOW)

	while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_TXE) );

	if(byteCnt>1)while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_BTF) );


	//8. Generate STOP condition and master need not to wait for the completion of stop condition.
	//   Note: generating STOP, automatically clears the BTF

	I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
	/*if(Sr == I2C_DISABLE_SR )
		I2C_GenerateStopCondition(pI2CHandle->pI2Cx);

		*/

}



////////////		I2C Master data Receive		//////////////////////////

void I2C_MasterReceiveData(I2C_Handle_t *pI2CHandle,uint8_t *pRxbuffer, uint32_t Len, uint8_t SlaveAddr,uint8_t Sr){

	// 1. Generate the START condition
		I2C_GenerateStartCondition(pI2CHandle->pI2Cx);


		//2. confirm that start generation is completed by checking the SB flag in the SR1
		//   Note: Until SB is cleared SCL will be stretched (pulled to LOW)
		while( !  I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_SB)   );


		//3. Send the address of the slave with r/~w bit set to w(0) (total 8 bits )
		I2C_ExecuteAddressPhaseRead(pI2CHandle->pI2Cx,SlaveAddr);

		//4. Confirm that address phase is completed by checking the ADDR flag in the SR1
		while( !  I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_ADDR));


		if(Len == 1){


				// Make the ACK bit 0 since master does not want more than 1 byte: ACKING is disabled

				I2C_ManageAcking(pI2CHandle->pI2Cx,I2C_ACK_DISABLE);

				//5. clear the ADDR flag according to its software sequence
						//   Note: Until ADDR is cleared SCL will be stretched (pulled to LOW)
				I2C_ClearADDRFlag(pI2CHandle->pI2Cx);


				//Wait until RXne flag is set
				while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_RXNE) );

				// Stop Condition is generated first
				I2C_GenerateStopCondition(pI2CHandle->pI2Cx);

				// NOw the only (1) Data byte will be read from the Data register I2C_DR

				*pRxbuffer = pI2CHandle->pI2Cx->I2C_DR;

			}

		else if(Len == 2){

			while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_BTF) ); // Means both DR and RXSR contains valif data

			//Disable Acking
			I2C_ManageAcking(pI2CHandle->pI2Cx,I2C_ACK_DISABLE);

			I2C_GenerateStopCondition(pI2CHandle->pI2Cx);

			//read the data from data register in to buffer
			*pRxbuffer = pI2CHandle->pI2Cx->I2C_DR;
			//increment the buffer address
			pRxbuffer++;

			//read the data from data register in to buffer
			*pRxbuffer = pI2CHandle->pI2Cx->I2C_DR;
			//increment the buffer address


		}


			else if (Len>=3){

				//clear the ADDR flag
						I2C_ClearADDRFlag(pI2CHandle->pI2Cx);

						//read the data until Len becomes zero
						for ( uint32_t i = Len ; i >= 2 ; i--)
						{

							if(i == 2) //if last 2 bytes are remaining
							{

								//wait for BTF to set
								while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_BTF) );

								//Disable Acking
								I2C_ManageAcking(pI2CHandle->pI2Cx,I2C_ACK_DISABLE);


								//generate STOP condition
								I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
								//read the data from data register in to buffer
								*pRxbuffer = pI2CHandle->pI2Cx->I2C_DR;
								//increment the buffer address
								pRxbuffer++;

								//read the data from data register in to buffer
								*pRxbuffer = pI2CHandle->pI2Cx->I2C_DR;
								//increment the buffer address

							}

							else{

								//wait until RXNE becomes 1
								while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_RXNE) );

								//read the data from data register in to buffer
								*pRxbuffer = pI2CHandle->pI2Cx->I2C_DR;

								//increment the buffer address
								pRxbuffer++;
							}

						}

			}



		//re-enable ACKing
			if(pI2CHandle->I2C_Config.I2C_AckControl == I2C_ACK_ENABLE)
			{
				I2C_ManageAcking(pI2CHandle->pI2Cx,I2C_ACK_ENABLE);
			}

}

////////////		I2C Master data Burst Read Data 	//////////////////////////

void I2C_MasterReceiveBurstData(I2C_Handle_t *pI2CHandle,uint8_t baseAddr,uint32_t addrCount,uint8_t* dataBuffer,uint8_t SlaveAddr,uint8_t Sr)
{
	volatile uint32_t loop_count=0;

			// Wait for BUSY flag get RESET
			while(pI2CHandle->pI2Cx->I2C_SR2 & (1U << I2C_SR2_BUSY));

			// 1. Generate the START condition
			I2C_GenerateStartCondition(pI2CHandle->pI2Cx);


			//2. confirm that start generation is completed by checking the SB flag in the SR1
			//   Note: Until SB is cleared SCL will be stretched (pulled to LOW)
			while( !  I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_SB)   );


			//3. Send the address of the slave with r/nw bit set to w(0) (total 8 bits )
			I2C_ExecuteAddressPhaseWrite(pI2CHandle->pI2Cx,SlaveAddr);


			//4. Confirm that address phase is completed by checking the ADDR flag in the SR1
				//   Note: We have to wait until ADDR = 1
			while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_ADDR));

			//5. clear the ADDR flag according to its software sequence
			//   Note: Until ADDR is cleared SCL will be stretched (pulled to LOW)
			I2C_ClearADDRFlag(pI2CHandle->pI2Cx);

			/*Send register address to be read from to the DR register*/

			while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_TXE) ); //Wait till TXE is set

			pI2CHandle->pI2Cx->I2C_DR = baseAddr;

			// 6. Generate the repeated- START condition
			I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

			//7. confirm that start generation is completed by checking the SB flag in the SR1
			//   Note: Until SB is cleared SCL will be stretched (pulled to LOW)
			while( !  I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_SB)   );


			//8. Send the address of the slave with r/~w bit set to w(0) (total 8 bits )
			I2C_ExecuteAddressPhaseRead(pI2CHandle->pI2Cx,SlaveAddr);

			//9. Confirm that address phase is completed by checking the ADDR flag in the SR1
			while( !  I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_ADDR));

			//5. clear the ADDR flag according to its software sequence
			//   Note: Until ADDR is cleared SCL will be stretched (pulled to LOW)
			I2C_ClearADDRFlag(pI2CHandle->pI2Cx);


			if(addrCount == 1){

				while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_BTF) ); // Means both DR and RXSR contains valif data

				// Make the ACK bit 0 since master does not want more than 1 byte: ACKING is disabled
				I2C_ManageAcking(pI2CHandle->pI2Cx,I2C_ACK_DISABLE);

				//Wait until RXne flag is set
				//while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_RXNE) );

				// Stop Condition is generated first
				I2C_GenerateStopCondition(pI2CHandle->pI2Cx);

				// NOw the only (1) Data byte will be read from the Data register I2C_DR

				dataBuffer[loop_count] = pI2CHandle->pI2Cx->I2C_DR;
				loop_count++;
				dataBuffer[loop_count] = pI2CHandle->pI2Cx->I2C_DR;
			}

			else if (addrCount > 1){

					while(loop_count<addrCount-1){

						if((addrCount-loop_count) == 2){


								while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_BTF) ); // Means both DR and RXSR contains valif data
								// Make the ACK bit 0 since master does not want more than 1 byte: ACKING is disabled
								I2C_ManageAcking(pI2CHandle->pI2Cx,I2C_ACK_DISABLE);

								dataBuffer[loop_count] = pI2CHandle->pI2Cx->I2C_DR;

								loop_count++;

								// Stop Condition is generated first
								I2C_GenerateStopCondition(pI2CHandle->pI2Cx);

								dataBuffer[loop_count] = pI2CHandle->pI2Cx->I2C_DR;

								loop_count++;

								while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_RXNE) );

								dataBuffer[loop_count] = pI2CHandle->pI2Cx->I2C_DR;

							}

						else{

							//Wait until RXne flag is set
							while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_RXNE) );

							dataBuffer[loop_count] = pI2CHandle->pI2Cx->I2C_DR;

							loop_count++;
						}

					}
			}

			//re-enable ACKing
			if(pI2CHandle->I2C_Config.I2C_AckControl == I2C_ACK_ENABLE)
			{
				I2C_ManageAcking(pI2CHandle->pI2Cx,I2C_ACK_ENABLE);
			}

}

