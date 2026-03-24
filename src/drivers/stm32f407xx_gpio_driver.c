/*
 * stm32f4xx_gpio_driver.c
 *
 *  Created on: Dec 19, 2024
 *      Author: dipch
 */
#include<stdint.h>
#include "stm32f407xx_gpio_driver.h"


#define GPIO_ADDR_TO_CODE(x)      ( (x == GPIOA)?0:\
										(x == GPIOB)?1:\
										(x == GPIOC)?2:\
										(x == GPIOD)?3:\
								        (x == GPIOE)?4:\
								        (x == GPIOF)?5:\
								        (x == GPIOG)?6:\
								        (x == GPIOH)?7: \
								        (x == GPIOI)?8:0)



/*********************************************************************
 *
 *
 * Function		- GPIO_PClkCtrl
 *
 *	Brief 		- This function enables or disable the clock for the GPIO port
 *
 * Parameter[0]	-	Base Address of the GPIO peripheral
 * Parameter[1] -	Enable or disable Macros
 * Parameter[2] -
 *
 *
 *
 *	Return		-
 *
 *	NOTE :
 *
 */



void GPIO_PClkCtrl(GPIO_RegDef_t* pGPIOx, uint8_t EnorDi)
{

	if(EnorDi == ENABLE)
	{

		if(pGPIOx == GPIOA)
		{
			GPIOA_PCLK_EN();
		}

		else if(pGPIOx == GPIOB)
		{
			GPIOB_PCLK_EN();
		}

		else if(pGPIOx == GPIOC)
		{
					GPIOC_PCLK_EN();
		}

		else if(pGPIOx == GPIOD)
		{
					GPIOD_PCLK_EN();
		}

		else if(pGPIOx == GPIOE)
		{
					GPIOE_PCLK_EN();
		}

		else if(pGPIOx == GPIOF)
		{
					GPIOF_PCLK_EN();
		}

		else if(pGPIOx == GPIOG)
		{
					GPIOG_PCLK_EN();
		}


		else if(pGPIOx == GPIOH)
		{
					GPIOH_PCLK_EN();
		}

		else if(pGPIOx == GPIOI)
		{
					GPIOI_PCLK_EN();
		}


	}

	else
	{

		if(pGPIOx == GPIOA)
				{
					GPIOA_PCLK_DI();
				}

				else if(pGPIOx == GPIOB)
				{
					GPIOB_PCLK_DI();
				}

				else if(pGPIOx == GPIOC)
				{
					GPIOC_PCLK_DI();
				}

				else if(pGPIOx == GPIOD)
				{
					GPIOD_PCLK_DI();
				}

				else if(pGPIOx == GPIOE)
				{
					GPIOE_PCLK_DI();
				}

				else if(pGPIOx == GPIOF)
				{
					GPIOF_PCLK_DI();
				}

				else if(pGPIOx == GPIOG)
				{
					GPIOG_PCLK_DI();
				}


				else if(pGPIOx == GPIOH)
				{
					GPIOH_PCLK_DI();
				}

				else if(pGPIOx == GPIOI)
				{
					GPIOI_PCLK_DI();
				}




	}
}

/*********************************************************************
 *
 *
 * Function		- GPIO_Init
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


void GPIO_Init(GPIO_Handle_t* pGPIOHandle)
{

	GPIO_PClkCtrl(pGPIOHandle->pGPIOx,ENABLE);

	//1.Configure the pin mode i.e MODE register of the GPIO

	if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_ANALOG)
	{

		pGPIOHandle->pGPIOx->MODER &= ~( 0x3 << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber)); //clearing

		pGPIOHandle->pGPIOx->MODER |= (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber ) );
	}

	else{

		//	 Implement pin interrupt mode here

		// 1. Configure the EXTI registers

		if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_FT){

			EXTI->FTSR &= (~(0x01<<(pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber))); 	// Clearing the EXTI_FTSR Register for the selected I/O Pin
			EXTI->FTSR |=(1<<pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);		// Setting the EXTI_FTSR Register for the selected I/O Pin

		}

		else if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RT){


			EXTI->RTSR &= (~(1<<pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber)); 	// Clearing the EXTI_RTSR Register for the selected I/O Pin
			EXTI->RTSR |=(1<<pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);		// Setting the EXTI_RTSR Register for the selected I/O Pin


		}


		else if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RFT){


			EXTI->RTSR |= (1<<pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber); 	// Clearing the EXTI_RFTSR Register for the selected I/O Pin
			EXTI->FTSR |= (1<<pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);		// Setting the EXTI_RFTSR Register for the selected I/O Pin

		}

		//2. Configure the SYSCFG_EXTICR

		uint8_t temp1 = (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber)/4;
		uint8_t temp2 = (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber)%4;
		uint8_t portcode = GPIO_ADDR_TO_CODE(pGPIOHandle->pGPIOx);
		SYSCFG_PCLK_EN();
		SYSCFG->EXTICR[temp1] = portcode << (4*temp2);


		//3. Configure the IMR using EXTI register


		EXTI->IMR |= (1<<pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);



	}

	//2. Configure the speed

	pGPIOHandle->pGPIOx->OSPEEDR &= ~(0x03 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);

	pGPIOHandle->pGPIOx->OSPEEDR |= (pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed)<<(2*(pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));


	//3. Configure the PuPd settings

	pGPIOHandle->pGPIOx->PUPDR &= ~(0x03 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);

	pGPIOHandle->pGPIOx->PUPDR |= (pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdControl)<<(2*(pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));

	//4. Configure the Output type

	pGPIOHandle->pGPIOx->OTYPER &= ~(0x01 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);

	pGPIOHandle->pGPIOx->OTYPER |= (pGPIOHandle->GPIO_PinConfig.GPIO_PinOPType)<<(pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);


	//5. COnfigure the alternate function


	if(pGPIOHandle ->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_ALTFN)
	{

	uint32_t temp1=0;
	uint32_t temp2=0;

	temp1 = ((pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber)/8);
	temp2 = ((pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber)%8);

	pGPIOHandle->pGPIOx->AFR[temp1] &= ~(0x0F << (4*temp2));
	pGPIOHandle->pGPIOx->AFR[temp1] |= (pGPIOHandle->GPIO_PinConfig.GPIO_PinAltFunMode)<<(4*temp2);

	}
}

/*********************************************************************
 *
 *
 * Function		-
 *
 *	Brief		-	This API de-initialises the GPIO port
 * Parameter[0]	-	GPIOs Base Address
 * Parameter[1] -
 * Parameter[2] -
 *
 *
 *
 *	Return		- Nothing
 *
 *	NOTE :
 *
 */

void GPIO_DeInit(GPIO_RegDef_t* pGPIOx)
{


	if(pGPIOx == GPIOA)
			{
				GPIOA_REG_RESET();
			}

			else if(pGPIOx == GPIOB)
			{
				GPIOB_REG_RESET();
			}

			else if(pGPIOx == GPIOC)
			{
				GPIOC_REG_RESET();
			}

			else if(pGPIOx == GPIOD)
			{
				GPIOD_REG_RESET();
			}

			else if(pGPIOx == GPIOE)
			{
				GPIOE_REG_RESET();
			}

			else if(pGPIOx == GPIOF)
			{
				GPIOF_REG_RESET();
			}

			else if(pGPIOx == GPIOG)
			{
				GPIOG_REG_RESET();
			}


			else if(pGPIOx == GPIOH)
			{
				GPIOH_REG_RESET();
			}

			else if(pGPIOx == GPIOI)
			{
				GPIOI_REG_RESET();
			}



}

/*********************************************************************
 *
 *
 * Function		- This function read the Input pin and return the pin-data(possible values 0,1)
 *
 *
 * Parameter[0]	- GPIO Base Address
 * Parameter[1] - Pin Number
 * Parameter[2] -
 *
 *
 *
 *	Return		- The data from the input pin
 *
 *	NOTE :
 *
 */

uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
   uint8_t value;

   value = (uint8_t )((pGPIOx->IDR  >> PinNumber) & 0x00000001 ) ;

   return value;
}

/*********************************************************************
 *
 *
 * Function		- This function read the Input port and returns the port-data (0xFF < possible values < 0X00)
 *
 *
 * Parameter[0]	- Base Address of GPIOx
 * Parameter[1] -
 * Parameter[2] -
 *
 *
 *
 *	Return		- The data from the input port
 *
 *	NOTE :
 *
 */

uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t* pGPIOx)
{

	uint16_t temp = (uint16_t)(pGPIOx->IDR);

	return temp;

}

/*********************************************************************
 *
 *
 * Function		-
 *
 *
 * Parameter[0]	-
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

void GPIO_WriteToOutputPin(GPIO_RegDef_t* pGPIOx,uint8_t PinNumber,uint8_t Value)
{


	if(Value == GPIO_PIN_SET){


	(pGPIOx->ODR)|= (1<<PinNumber);


	}

	else if(Value == GPIO_PIN_RESET)
	{

	(pGPIOx->ODR)&= (~(1<<PinNumber));

	}



}

/*********************************************************************
 *
 *
 * Function		-
 *
 *
 * Parameter[0]	-
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
void GPIO_WriteToOutputPort(GPIO_RegDef_t* pGPIOx,uint16_t Value)
{


	pGPIOx->ODR = Value;

}



/*********************************************************************
 *
 *
 * Function		-
 *
 *
 * Parameter[0]	-
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
void GPIO_ToggleOutputPin(GPIO_RegDef_t* pGPIOx,uint8_t PinNumber)
{

	(pGPIOx->ODR) ^= (1<<PinNumber);


}


/*********************************************************************
 *
 *
 * Function		-
 *
 *
 * Parameter[0]	-
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
void GPIO_IRQInterruptConfig(uint8_t IRQNumber,uint8_t EnorDi)
{

	if (EnorDi == ENABLE)
	{

		if(IRQNumber <=31 )
		{
			//Config the ISER0 reg

			*NVIC_ISER0 |= (1<<IRQNumber);

		}
		else if(IRQNumber > 31 && IRQNumber <= 63)
		{
			//Config the ISER1 reg

			*NVIC_ISER1 |= (1<< (IRQNumber % 32));
		}

		else if(IRQNumber > 64 && IRQNumber <= 95)
		{

			//Config the ISER2 reg
			*NVIC_ISER2 |= (1<< (IRQNumber % 64));
		}



	}

	else{

		if(IRQNumber <=31 )
				{

				*NVIC_ICER0 |= (1<<IRQNumber);

				}
		else if(IRQNumber > 31 && IRQNumber <= 63)
				{

			*NVIC_ICER1 |= (1<<IRQNumber);

				}

		else if(IRQNumber > 64 && IRQNumber <= 95)
				{

			*NVIC_ICER2 |= (1<<IRQNumber);

				}


	}


}


void GPIO_IRQPriorityConfig(uint8_t IRQNumber, uint8_t IRQPriority)
{


	uint8_t iprx = IRQNumber/4 ;
	uint8_t iprx_section = IRQNumber%4;
	uint8_t shift_ammount = (8*iprx_section) + (8 - PR_BITS_IMPLEMENTED);
	*(NVIC_IPR_BASEADDR + iprx) |=(IRQPriority << shift_ammount );

}

/*********************************************************************
 *
 *
 * Function		-
 *
 *
 * Parameter[0]	-
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
void GPIO_IRQHandling(uint8_t PinNumber)
{

	if(EXTI->PR & (0x1<<PinNumber))
	{

		EXTI->PR |= (0x1<<PinNumber);
	}


}






