/*
 * stm32f407xx_spi_driver.c
 *
 *  Created on: Jan 6, 2025
 *      Author: dipch
 */

#include "stm32f407xx_spi_driver.h"

void SPI_PClkCtrl(SPI_RegDef_t* pSPIx, uint8_t EnorDi)
{

	if(EnorDi == ENABLE)
	{

		if(pSPIx == SPI1)
		{
			SPI1_PCLK_EN();
		}

		else if(pSPIx == SPI2)
		{
			SPI2_PCLK_EN();
		}

		else if(pSPIx == SPI3)
		{
			SPI3_PCLK_EN();
		}

		else if(pSPIx == SPI4)
		{
			SPI4_PCLK_EN();
		}



	}

	else
	{

		if(pSPIx == SPI1)
				{
					SPI1_PCLK_DI();
				}

				else if(pSPIx == SPI2)
				{
					SPI2_PCLK_DI();
				}

				else if(pSPIx == SPI3)
				{
					SPI3_PCLK_DI();
				}

				else if(pSPIx == SPI4)
				{
					SPI4_PCLK_DI();
				}



	}



}

	/*********************************************************************
	 *
	 *
	 * Function		- SPI_Init
	 *
	 * Brief		- Function initializes the GPIO Mode register(Input, Output, AltFN, Analog)
	 *
	 * Parameter[0]	- GPIO handle details
	 * Parameter[1] -
	 * Parameter[2] -
	 *
	 *
	 *
	 *	Return		-
	 *
	 *	NOTE :
	 *
	 */



void SPI_Init(SPI_Handle_t* pSPIHandle)
{

	uint32_t tempreg = 0;

	// Configure the Device mode (Master OR slave)

	if (pSPIHandle->SPIConfig.SPI_DeviceMode == SPI_DEVICE_MODE_MASTER){

		tempreg |=(1<<2);

	}


// Configure the Bus Configuration modes(FD,HD,RxOnly)

	if (pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_FD){

		//BIDIMODE should be cleared
		tempreg &= (~(1<<15));

	}

	else if (pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_HD){

			//BIDIMODE should be set
		tempreg |= (1<<15);

	}

	else if (pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_SIMPLEX_RX){

				//BIDIMODE should be cleared
		tempreg &= (~(1<<15));
				// RXONLY should be set
		tempreg |= (1<<10);


		}


// Configure the SClk i.e the BAUD rate bits of SPI_CR1


	tempreg |= (pSPIHandle->SPIConfig.SPI_SclkSpeed<<3);

//Configure DFF

	tempreg |= (pSPIHandle->SPIConfig.SPI_DFF<<11);

//Configure CPOL

	tempreg |= (pSPIHandle->SPIConfig.SPI_CPOL<<1);

//Configure CPHA

	tempreg |= (pSPIHandle->SPIConfig.SPI_CPHA<<0);

// Configure SSM

	tempreg |= (pSPIHandle->SPIConfig.SPI_SSM<<9);


	//Assign the value to SPI_CR1

	pSPIHandle->pSPIx->SPI_CR1 = tempreg;


}



void SPI_Peripheral_Control(SPI_RegDef_t  *pSPIx,uint8_t EnorDi ){

	if(EnorDi==ENABLE){
		pSPIx->SPI_CR1|=(1<<6);

	}

	else if(EnorDi==DISABLE) {

		pSPIx->SPI_CR1 &=(~(1<<6));

	}

}


/*********************************************************************
 *
 *
 * Function		- SPI_DeInit
 *
 * Brief		- Function initializes the GPIO Mode register(Input, Output, AltFN, Analog)
 *
 * Parameter[0]	- GPIO handle details
 * Parameter[1] -
 * Parameter[2] -
 *
 *
 *
 *	Return		-
 *
 *	NOTE :
 *
 */



//void SPI_DeInit(SPI_Handle_t* pSPIHandle){

//}



/*********************************************************************
 *
 *
 * Function		- SPI_SendData
 *
 * Brief		- Function initializes the GPIO Mode register(Input, Output, AltFN, Analog)
 *
 * Parameter[0]	- GPIO handle details
 * Parameter[1] -
 * Parameter[2] -
 *
 *
 *
 *	Return		-
 *
 *	NOTE :		!!!POLLING METHOD IMPLEMENTATION OF THE SENDDATA. THIS HAS PONTENTIAL TO BLOCK THE CODE
 *
 *					****INTERRUPT BASED IMPLEMENTATION IS RECOMMENDED*****
 *
 */


void SPI_SendData(SPI_RegDef_t* pSPIx, uint8_t *data, uint32_t Len){

while(Len!=0){

	while(((pSPIx->SPI_SR >>1)&(1<<0)) == RESET);			//wait until TXe bit flag is set

	// check the DFF

	if(((pSPIx->SPI_CR1>>11)&(1<<0))== SET){


			//16bit DFF
		pSPIx->SPI_DR = *((uint16_t*)data);
		Len --;
		Len --;

		(uint16_t*)data++;								//data pointer is incremented to point the next 16 bits


	}


	else if(((pSPIx->SPI_CR1>>11)&(1<<0))== RESET){

		//8bit DFF

		pSPIx->SPI_DR = *((uint8_t*)data);
		Len --;
		(uint8_t*)data++;							 //data pointer is incremented to point the next 8 bits

		}





}

}


void SPI_SSIConfig(SPI_RegDef_t  *pSPIx,uint8_t EnorDi){

	if(EnorDi == ENABLE){

		pSPIx->SPI_CR1 |=(1<<8);

	}

	else{

		pSPIx->SPI_CR1 &=(~(1<<8));
	}


}








