/*
 * stm32f407xx_i2c_driver.h
 *
 *  Created on: Jan 26, 2025
 *      Author: dipch
 */

#ifndef INC_STM32F407XX_I2C_DRIVER_H_
#define INC_STM32F407XX_I2C_DRIVER_H_

#include "stm32f407xx.h"
#include "stdint.h"



//////////////////////		FLAG STATUS		///////////

#define	FLAG_SET	1
#define FLAG_RESET	0

#define I2C_FLAG_TXE   		( 1 << I2C_SR1_TXE)
#define I2C_FLAG_RXNE   	( 1 << I2C_SR1_RXNE)
#define I2C_FLAG_SB			( 1 << I2C_SR1_SB)
#define I2C_FLAG_OVR  		( 1 << I2C_SR1_OVR)
#define I2C_FLAG_AF   		( 1 << I2C_SR1_AF)
#define I2C_FLAG_ARLO 		( 1 << I2C_SR1_ARLO)
#define I2C_FLAG_BERR 		( 1 << I2C_SR1_BERR)
#define I2C_FLAG_STOPF 		( 1 << I2C_SR1_STOPF)
#define I2C_FLAG_ADD10 		( 1 << I2C_SR1_ADD10)
#define I2C_FLAG_BTF  		( 1 << I2C_SR1_BTF)
#define I2C_FLAG_ADDR 		( 1 << I2C_SR1_ADDR)
#define I2C_FLAG_TIMEOUT 	( 1 << I2C_SR1_TIMEOUT)

#define I2C_DISABLE_SR  	RESET
#define I2C_ENABLE_SR   	SET

/////////		Bit - Fields Macros for I2C_CR1 REGISTER		//////////////////

#define I2C_CR1_PE			0
#define I2C_CR1_NOSTRETCH	7
#define I2C_CR1_START		8
#define I2C_CR1_STOP		9
#define I2C_CR1_ACK			10
#define I2C_CR1_SWRST		15



/////////		Bit - Fields Macros for I2C_CR2 REGISTER		//////////////////

#define I2C_CR2_FREQ		0
#define I2C_CR2_ITERREN		8
#define I2C_CR2_ITEVTEN		9
#define I2C_CR2_ITBUFFEN	10


/////////		Bit - Fields Macros for I2C_SR1 REGISTER		//////////////////

#define I2C_SR1_SB			0
#define I2C_SR1_ADDR		1
#define I2C_SR1_BTF			2
#define I2C_SR1_ADD10		3
#define I2C_SR1_STOPF		4
#define I2C_SR1_RXNE		6
#define I2C_SR1_TXE			7
#define I2C_SR1_BERR		8
#define I2C_SR1_ARLO		9
#define I2C_SR1_AF			10
#define I2C_SR1_OVR			11
#define I2C_SR1_TIMEOUT		14


/////////		Bit - Fields Macros for I2C_SR2 REGISTER		//////////////////


#define I2C_SR2_MSL			0
#define I2C_SR2_BUSY		1
#define I2C_SR2_TRA			2
#define I2C_SR2_GENCALL		4
#define I2C_SR2_DUALF		7

/////////		Bit - Fields Macros for I2C_CCR REGISTER		//////////////////

#define I2C_CCR_CCR			0
#define I2C_CCR_DUTY		14
#define I2C_CCR_FS			15



typedef struct
{
	uint32_t I2C_SCLSpeed;
	uint8_t  I2C_DeviceAddress;
	uint8_t  I2C_AckControl;
	uint8_t  I2C_FMDutyCycle;

}I2C_Config_t;

typedef struct
{
	I2C_RegDef_t 	*pI2Cx;
	I2C_Config_t 	I2C_Config;
	uint8_t 		*pTxBuffer; /* !< To store the app. Tx buffer address > */
	uint8_t 		*pRxBuffer;	/* !< To store the app. Rx buffer address > */
	uint32_t 		TxLen;		/* !< To store Tx len > */
	uint32_t 		RxLen;		/* !< To store Tx len > */
	uint8_t 		TxRxState;	/* !< To store Communication state > */
	uint8_t 		DevAddr;	/* !< To store slave/device address > */
    uint32_t        RxSize;		/* !< To store Rx size  > */
    uint8_t         Sr;			/* !< To store repeated start value  > */
}I2C_Handle_t;



////////		I2C SCLK SPEED		/////////////////////

#define	I2C_SCL_SPEED_SM	100000				//100KHz
#define	I2C_SCL_SPEED_FM4K	400000				//400KHz
#define	I2C_SCL_SPEED_FM2K	200000				//200KHz

////////		I2C ACKControl		/////////////////////

#define I2C_ACK_ENABLE		1
#define I2C_ACK_DISABLE		0

//////// I2C DUTY CYCLE		//////////////

#define I2C_FM_DUTY_2		0
#define I2C_FM_DUTY_16_9	1




//////////////////////		PROTOTYPES OF APIs SUPPORTED BY THIS DRIVER		////////////////////////////

/*
 * Peripheral clock setup
 */
void I2C_PClkCtrl(I2C_RegDef_t* pI2Cx, uint8_t EnorDi);


/*
 * Initialization and de-Initialization of I2C
 */

void I2C_Init(I2C_Handle_t *pI2CHandle);
void I2C_DeInit(I2C_RegDef_t* pI2Cx);


/*
 * Data send and receive
 */

void I2C_MasterSendData(I2C_Handle_t *pI2CHandle,uint8_t *pTxbuffer, uint32_t Len, uint8_t SlaveAddr,uint8_t Sr);

void I2C_MasterReceiveData(I2C_Handle_t *pI2CHandle,uint8_t *pRxbuffer, uint32_t Len, uint8_t SlaveAddr,uint8_t Sr);

void I2C_MasterReceiveBurstData(I2C_Handle_t *pI2CHandle,uint8_t baseAddr,uint32_t count,uint8_t* dataBuffer,uint8_t SlaveAddr,uint8_t Sr);
/*
 *
 * IRQ Configuration and ISR handling
 */

void I2C_IRQInterruptConfig(uint8_t IRQNumber,uint8_t EnorDi);
void I2C_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);



/*
 * OTHER APIs
 */

void I2C_PeripheralControl(I2C_RegDef_t  *pI2Cx,uint8_t EnorDi);
uint8_t I2C_GetFlagStatus(I2C_RegDef_t  *pI2Cx,uint32_t FlagName);

void I2C_GenerateStopCondition(I2C_RegDef_t *);



#endif /* INC_STM32F407XX_I2C_DRIVER_H_ */
