/*
 * stm32f407xx_spi_driver.h
 *
 *  Created on: Jan 6, 2025
 *      Author: dipch
 */

#ifndef INC_STM32F407XX_SPI_DRIVER_H_
#define INC_STM32F407XX_SPI_DRIVER_H_


#include "stm32f407xx.h"
#include <stdint.h>

typedef struct {

	uint8_t SPI_DeviceMode;
	uint8_t SPI_BusConfig;
	uint8_t SPI_SclkSpeed;
	uint8_t SPI_DFF;
	uint8_t SPI_CPOL;
	uint8_t SPI_CPHA;
	uint8_t SPI_SSM;

}SPI_Config_t;

/*
 * Handle structure for SPI peripherals
 */

typedef struct{

	SPI_RegDef_t  *pSPIx;
	SPI_Config_t  SPIConfig;

}SPI_Handle_t;


/*
 *
 * SPI Device mode macros
 *
 */
#define SPI_DEVICE_MODE_MASTER		1
#define SPI_DEVICE_MODE_SLAVE		0




/*
 * SPI BUS CONFIG
 */

#define SPI_BUS_CONFIG_FD				1
#define SPI_BUS_CONFIG_HD				2
#define SPI_BUS_CONFIG_SIMPLEX_RX		3



/*
 * SPI clock speed
 */

#define SPI_SCLK_SPEED_2				0
#define SPI_SCLK_SPEED_4				1
#define SPI_SCLK_SPEED_8				2
#define SPI_SCLK_SPEED_16				3
#define SPI_SCLK_SPEED_32				4
#define SPI_SCLK_SPEED_64				5
#define SPI_SCLK_SPEED_128				6
#define SPI_SCLK_SPEED_256				7


/*
 * SPI DFF
 */

#define SPI_DFF_8BITS 					0
#define SPI_DFF_16BITS 					1



/*
 *
 * SPI CPOL
 */

#define SPI_CPOL_LOW					0
#define SPI_CPOL_HIGH					1

/*
 *
 * SPI CPHA
 */

#define SPI_CPHA_LOW					0
#define SPI_CPHA_HIGH					1

#define SPI_SSM_EN						1
#define SPI_SSM_DI						0





/*
 * Peripheral clock setup
 */
void SPI_PClkCtrl(SPI_RegDef_t* pSPIx, uint8_t EnorDi);


//		Initialization and de-Initialization of GPIO

void SPI_Init(SPI_Handle_t* pSPIHandle);
void SPI_DeInit(SPI_RegDef_t* pSPIx);


/*
 * Data send and receive
 */

	//Non- Interrupt based API

void SPI_SendData(SPI_RegDef_t* pSPIx, uint8_t *data, uint32_t Len);
void SPI_ReceiveData(SPI_RegDef_t* pSPIx, uint8_t *pRxBuffer, uint32_t Len);



/*
 *
 * IRQ Configuration and ISR handling
 */


void SPI_IRQInterruptConfig(uint8_t IRQNumber,uint8_t EnorDi);
void SPI_IRQPriorityConfig(uint8_t IRQNumber,uint8_t IRQPriority);
void SPI_IRQHandling(SPI_Handle_t* pSPIHandle);






/*
 * Other peripheral control APIs
 */

void SPI_SSIConfig(SPI_RegDef_t  *pSPIx,uint8_t EnorDi);

void SPI_Peripheral_Control(SPI_RegDef_t  *pSPIx,uint8_t EnorDi);





#endif /* INC_STM32F407XX_SPI_DRIVER_H_ */
