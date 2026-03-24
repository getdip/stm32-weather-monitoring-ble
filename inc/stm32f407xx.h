/*
 * stm32f407xx.h
 *
 *  Created on: Dec 18, 2024
 *      Author: dipch
 */

#ifndef INC_STM32F407XX_H_
#define INC_STM32F407XX_H_

#include<stdint.h>



/*
 * MACROS FOR NVIC SET INTERRUPT SET REGISTERS
 */

#define NVIC_ISER0				((volatile uint32_t *)(0xE000E100))
#define NVIC_ISER1				((volatile uint32_t *)(0xE000E104))
#define NVIC_ISER2				((volatile uint32_t *)(0xE000E108))
#define NVIC_ISER3          	((volatile uint32_t *)(0xE000E10c))


/*
 * MACROS FOR NVIC INTERRUPT RESET REGISTERS
 */

#define NVIC_ICER0				((volatile uint32_t *)(0XE000E180))
#define NVIC_ICER1				((volatile uint32_t *)(0XE000E184))
#define NVIC_ICER2				((volatile uint32_t *)(0XE000E188))
#define NVIC_ICER3				((volatile uint32_t *)(0XE000E18C))


#define NVIC_ISPR1				((volatile uint32_t *)(0XE000E204))


/*
 * MACROS FOR NVIC INTERRUPT PRIORITY CONFIG REGISTER
 */

#define NVIC_IPR_BASEADDR				((volatile uint32_t *)(0xE000E400))

#define PR_BITS_IMPLEMENTED				4


//NVIC PRIORITY MACROS

#define NVIC_IRQ_PR_0					0
#define NVIC_IRQ_PR_1					1
#define NVIC_IRQ_PR_2					2
#define NVIC_IRQ_PR_3					3
#define NVIC_IRQ_PR_4					4
#define NVIC_IRQ_PR_5					5
#define NVIC_IRQ_PR_6					6
#define NVIC_IRQ_PR_7					7
#define NVIC_IRQ_PR_8					8
#define NVIC_IRQ_PR_9					9
#define NVIC_IRQ_PR_10					10
#define NVIC_IRQ_PR_11					11
#define NVIC_IRQ_PR_12					12
#define NVIC_IRQ_PR_13					13
#define NVIC_IRQ_PR_14					14
#define NVIC_IRQ_PR_15					15

/*SysTick Timer Macros*/

#define SYST_CSR_ADDR		((volatile uint32_t *)0xE000E010)
#define SYST_RVR_ADDR 		((volatile uint32_t *)0xE000E014)
#define SYST_CVR_ADDR 		((volatile uint32_t *)0xE000E018)
#define SYST_CALIB_ADDR 	((volatile uint32_t *)0xE000E01C)

//MACROS OF THE BASE ADDRESSES OF MEMORY REGIONS

#define FLASH_BASEADDR 			0x08000000U
#define SRAM1_BASEADDR			0x20000000U
#define SRAM2_BASEADDR			0x2001C000U
#define ROM_BASEADDR			0x1FFF0000U
#define SRAM 					SRAM1_BASEADDR


#define PERIPH_BASEADDR			0x40000000U
#define APB1PERIPH_BASEADDR		0x40000000U
#define APB2PERIPH_BASEADDR		0x40010000U
#define AHB1PERIPH_BASEADDR		0x40020000U
#define AHB2PERIPH_BASEADDR		0x50000000U

#define GPIOA_BASEADDR			(AHB1PERIPH_BASEADDR + 0x0000)
#define GPIOB_BASEADDR			(AHB1PERIPH_BASEADDR + 0x0400)
#define GPIOC_BASEADDR			(AHB1PERIPH_BASEADDR + 0x0800)
#define GPIOD_BASEADDR			(AHB1PERIPH_BASEADDR + 0x0C00)
#define GPIOE_BASEADDR			(AHB1PERIPH_BASEADDR + 0x1000)
#define GPIOF_BASEADDR			(AHB1PERIPH_BASEADDR + 0x1400)
#define GPIOG_BASEADDR			(AHB1PERIPH_BASEADDR + 0x1800)
#define GPIOH_BASEADDR			(AHB1PERIPH_BASEADDR + 0x1C00)
#define GPIOI_BASEADDR			(AHB1PERIPH_BASEADDR + 0x2000)


#define I2C1_BASEADDR			(APB1PERIPH_BASEADDR + 0x5400)
#define I2C2_BASEADDR			(APB1PERIPH_BASEADDR + 0x5800)
#define I2C3_BASEADDR			(APB1PERIPH_BASEADDR + 0x5C00)

#define SPI2_BASEADDR			(APB1PERIPH_BASEADDR + 0X3800)
#define SPI3_BASEADDR			(APB1PERIPH_BASEADDR + 0X3C00)

#define USART2_BASEADDR			(APB1PERIPH_BASEADDR + 0x4400)
#define USART3_BASEADDR			(APB1PERIPH_BASEADDR + 0x4800)
#define UART4_BASEADDR			(APB1PERIPH_BASEADDR + 0x4C00)
#define UART5_BASEADDR			(APB1PERIPH_BASEADDR + 0x5000)

#define SPI1_BASEADDR			(APB2PERIPH_BASEADDR + 0X3000)
#define SPI4_BASEADDR			(APB2PERIPH_BASEADDR + 0X3400)

#define USART1_BASEADDR			(APB2PERIPH_BASEADDR + 0X1000)
#define USART6_BASEADDR			(APB2PERIPH_BASEADDR + 0X1400)
#define EXTI_BASEADDR			(APB2PERIPH_BASEADDR + 0X3C00)
#define SYSCFG_BASEADDR			(APB2PERIPH_BASEADDR + 0X3800)


#define	RCC_BASEADDR			(AHB1PERIPH_BASEADDR + 0x3800)

//STRUCTURE FOR GPIOs REGISTERS
typedef struct
{

	volatile uint32_t MODER;
	volatile uint32_t OTYPER;
	volatile uint32_t OSPEEDR;
	volatile uint32_t PUPDR;
	volatile uint32_t IDR;
	volatile uint32_t ODR;
	volatile uint32_t BSRR;
	volatile uint32_t LCKR;
	volatile uint32_t AFR[2];

}GPIO_RegDef_t;


#define GPIOA ((GPIO_RegDef_t*)GPIOA_BASEADDR)
#define GPIOB ((GPIO_RegDef_t*)GPIOB_BASEADDR)
#define GPIOC ((GPIO_RegDef_t*)GPIOC_BASEADDR)
#define GPIOD ((GPIO_RegDef_t*)GPIOD_BASEADDR)
#define GPIOE ((GPIO_RegDef_t*)GPIOE_BASEADDR)
#define GPIOF ((GPIO_RegDef_t*)GPIOF_BASEADDR)
#define GPIOG ((GPIO_RegDef_t*)GPIOG_BASEADDR)
#define GPIOH ((GPIO_RegDef_t*)GPIOH_BASEADDR)
#define GPIOI ((GPIO_RegDef_t*)GPIOI_BASEADDR)


// STRUCTURE FOR SYSCFG REGISTER

typedef struct{

	volatile uint32_t MEMRMP;
	volatile uint32_t PMC;
	volatile uint32_t EXTICR[4];
	uint32_t RESERVED2[2];
	volatile uint32_t CMPCR;

	}SYSCFG_RegDef_t;


#define SYSCFG ((SYSCFG_RegDef_t*)SYSCFG_BASEADDR)



// STRUCTURE FOR EXTI REGISTERS


typedef struct{

	volatile uint32_t IMR;
	volatile uint32_t EMR;
	volatile uint32_t RTSR;
	volatile uint32_t FTSR;
	volatile uint32_t SWIER;
	volatile uint32_t PR;
} EXTI_RegDef_t;


#define EXTI ((EXTI_RegDef_t*)EXTI_BASEADDR)



//STRUCTURE FOR RCC REGISTERS
typedef struct
{
	volatile uint32_t CR;
	volatile uint32_t PLLCFGR;
	volatile uint32_t CFGR;
	volatile uint32_t CIR;
	volatile uint32_t AHB1RSTR;
	volatile uint32_t AHB2RSTR;
	volatile uint32_t AHB3RSTR;
			 uint32_t RESERVED0;
	volatile uint32_t APB1RSTR;
	volatile uint32_t APB2RSTR;
			 uint32_t RESERVED1;
			 uint32_t RESERVED2;
	volatile uint32_t AHB1ENR;
	volatile uint32_t AHB2ENR;
	volatile uint32_t AHB3ENR;
			 uint32_t RESERVED3;
	volatile uint32_t APB1ENR;
	volatile uint32_t APB2ENR;
			 uint32_t RESERVED4;
			 uint32_t RESERVED5;
	volatile uint32_t AHB1LPENR;
	volatile uint32_t AHB2LPENR;
	volatile uint32_t AHB3LPENR;
	volatile uint32_t APB1LPENR;
	volatile uint32_t APB2LPENR;
			 uint32_t RESERVED6;
			 uint32_t RESERVED7;
	volatile uint32_t BDCR;
	volatile uint32_t CSR;
			 uint32_t RESERVED8;
			 uint32_t RESERVED9;
	volatile uint32_t SSCGR;
	volatile uint32_t PLLI2SCFGR;
	volatile uint32_t PLLSAICFGR;
	volatile uint32_t DCKCFGR;

}RCC_RegDef_t;



#define RCC ((RCC_RegDef_t*)RCC_BASEADDR)




/*
 * STRUCTURE FOR SPI REGISTER
 */


typedef struct{

	volatile uint32_t SPI_CR1;
	volatile uint32_t SPI_CR2;
	volatile uint32_t SPI_SR;
	volatile uint32_t SPI_DR;
	volatile uint32_t SPI_CRCPR;
	volatile uint32_t SPI_RXCRCR;
	volatile uint32_t SPI_TXCRCR;
	volatile uint32_t SPI_I2SCFGR;
	volatile uint32_t SPI_I2SPR;

}SPI_RegDef_t;


#define SPI1 ((SPI_RegDef_t*)SPI1_BASEADDR)
#define SPI2 ((SPI_RegDef_t*)SPI2_BASEADDR)
#define SPI3 ((SPI_RegDef_t*)SPI3_BASEADDR)
#define SPI4 ((SPI_RegDef_t*)SPI4_BASEADDR)


/*
 * REGISTER DEFINITION STRUCTURE FOR I2C REGISTERS
 */


typedef struct
{
	volatile uint32_t I2C_CR1;
	volatile uint32_t I2C_CR2;
	volatile uint32_t I2C_OAR1;
	volatile uint32_t I2C_OAR2;
	volatile uint32_t I2C_DR;
	volatile uint32_t I2C_SR1;
	volatile uint32_t I2C_SR2;
	volatile uint32_t I2C_CCR;
	volatile uint32_t I2C_TRISE;
	volatile uint32_t I2C_FLTR;
}I2C_RegDef_t;

#define I2C1  				((I2C_RegDef_t*)I2C1_BASEADDR)
#define I2C2  				((I2C_RegDef_t*)I2C2_BASEADDR)
#define I2C3  				((I2C_RegDef_t*)I2C3_BASEADDR)


 /* REGISTER DEFINITION STRUCTURE FOR USART REGISTERS
 */

typedef struct
{
	volatile uint32_t SR;
	volatile uint32_t DR;
	volatile uint32_t BRR;
	volatile uint32_t CR1;
	volatile uint32_t CR2;
	volatile uint32_t CR3;
	volatile uint32_t GTPR;
} USART_RegDef_t;

#define USART1  			((USART_RegDef_t*)USART1_BASEADDR)
#define USART2  			((USART_RegDef_t*)USART2_BASEADDR)
#define USART3  			((USART_RegDef_t*)USART3_BASEADDR)
#define UART4  				((USART_RegDef_t*)UART4_BASEADDR)
#define UART5  				((USART_RegDef_t*)UART5_BASEADDR)
#define USART6  			((USART_RegDef_t*)USART6_BASEADDR)

//Macro for clock enable for GPIOs

#define	GPIOA_PCLK_EN()	(RCC -> AHB1ENR |=(1<<0))
#define	GPIOB_PCLK_EN()	(RCC -> AHB1ENR |=(1<<1))
#define	GPIOC_PCLK_EN()	(RCC -> AHB1ENR |=(1<<2))
#define	GPIOD_PCLK_EN()	(RCC -> AHB1ENR |=(1<<3))
#define	GPIOE_PCLK_EN()	(RCC -> AHB1ENR |=(1<<4))
#define	GPIOF_PCLK_EN()	(RCC -> AHB1ENR |=(1<<5))
#define	GPIOG_PCLK_EN()	(RCC -> AHB1ENR |=(1<<6))
#define	GPIOH_PCLK_EN()	(RCC -> AHB1ENR |=(1<<7))
#define	GPIOI_PCLK_EN()	(RCC -> AHB1ENR |=(1<<8))


//Macro for clock disable for GPIOs

#define GPIOA_PCLK_DI()	(RCC -> AHB1ENR &=(~(1<<0)))
#define GPIOB_PCLK_DI()	(RCC -> AHB1ENR &=(~(1<<1)))
#define GPIOC_PCLK_DI()	(RCC -> AHB1ENR &=(~(1<<2)))
#define GPIOD_PCLK_DI()	(RCC -> AHB1ENR &=(~(1<<3)))
#define GPIOE_PCLK_DI()	(RCC -> AHB1ENR &=(~(1<<4)))
#define GPIOF_PCLK_DI()	(RCC -> AHB1ENR &=(~(1<<5)))
#define GPIOG_PCLK_DI()	(RCC -> AHB1ENR &=(~(1<<6)))
#define GPIOH_PCLK_DI()	(RCC -> AHB1ENR &=(~(1<<7)))
#define GPIOI_PCLK_DI()	(RCC -> AHB1ENR &=(~(1<<8)))


// Macro for GPIOs reset

#define GPIOA_REG_RESET()	do{ (RCC -> AHB1RSTR |=((1<<0))); (RCC -> AHB1RSTR &=(~(1<<0))); } while(0)
#define GPIOB_REG_RESET()	do{ (RCC -> AHB1RSTR |=((1<<1))); (RCC -> AHB1RSTR &=(~(1<<1))); } while(0)
#define GPIOC_REG_RESET()	do{ (RCC -> AHB1RSTR |=((1<<2))); (RCC -> AHB1RSTR &=(~(1<<2))); } while(0)
#define GPIOD_REG_RESET()	do{ (RCC -> AHB1RSTR |=((1<<3))); (RCC -> AHB1RSTR &=(~(1<<3))); } while(0)
#define GPIOE_REG_RESET()	do{ (RCC -> AHB1RSTR |=((1<<4))); (RCC -> AHB1RSTR &=(~(1<<4))); } while(0)
#define GPIOF_REG_RESET()	do{ (RCC -> AHB1RSTR |=((1<<5))); (RCC -> AHB1RSTR &=(~(1<<5))); } while(0)
#define GPIOG_REG_RESET()	do{ (RCC -> AHB1RSTR |=((1<<6))); (RCC -> AHB1RSTR &=(~(1<<6))); } while(0)
#define GPIOH_REG_RESET()	do{ (RCC -> AHB1RSTR |=((1<<7))); (RCC -> AHB1RSTR &=(~(1<<7))); } while(0)
#define GPIOI_REG_RESET()	do{ (RCC -> AHB1RSTR |=((1<<8))); (RCC -> AHB1RSTR &=(~(1<<8))); } while(0)



//Macro for clock enable of I2Cs

#define	I2C1_PCLK_EN()	(RCC -> APB1ENR |=(1<<21))
#define	I2C2_PCLK_EN()	(RCC -> APB1ENR |=(1<<22))
#define	I2C3_PCLK_EN()	(RCC -> APB1ENR |=(1<<23))


//Macro for clock enable of SPIs

#define	SPI1_PCLK_EN()	(RCC -> APB2ENR |=(1<<12))
#define	SPI2_PCLK_EN()	(RCC -> APB1ENR |=(1<<14))
#define	SPI3_PCLK_EN()	(RCC -> APB1ENR |=(1<<15))
#define	SPI4_PCLK_EN()	(RCC -> APB2ENR |=(1<<13))


//Macro for clock enable of UsARTs
#define	USART1_PCLK_EN()	(RCC -> APB2ENR |=(1<<4))
#define	USART2_PCLK_EN()	(RCC -> APB1ENR |=(1<<17))
#define	USART3_PCLK_EN()	(RCC -> APB1ENR |=(1<<18))
#define	UART4_PCLK_EN()		(RCC -> APB1ENR |=(1<<19))
#define	UART5_PCLK_EN()		(RCC -> APB1ENR |=(1<<20))
#define	USART6_PCLK_EN()	(RCC -> APB1ENR |=(1<<5))


//Macro for clock enable for SYSCFG peripheral

#define	SYSCFG_PCLK_EN()	(RCC -> APB1ENR |=(1<<14))



//clock disable macro for SPIx


#define SPI1_PCLK_DI()		(RCC -> APB2ENR &=(~(1<<12)))
#define SPI2_PCLK_DI()		(RCC -> APB2ENR &=(~(1<<14)))
#define SPI3_PCLK_DI()		(RCC -> APB2ENR &=(~(1<<15)))
#define SPI4_PCLK_DI()		(RCC -> APB2ENR &=(~(1<<13)))

//clock disable macro for I2cx

#define	I2C1_PCLK_DI()	(RCC -> APB1ENR &=(~(1<<21)))
#define	I2C2_PCLK_DI()	(RCC -> APB1ENR &=(~(1<<22)))
#define	I2C3_PCLK_DI()	(RCC -> APB1ENR &=(~(1<<23)))


//clock disable macro for USARTx
#define	USART1_PCLK_DI()	(RCC -> APB2ENR &=(~(1<<4)))
#define	USART2_PCLK_DI()	(RCC -> APB1ENR &=(~(1<<17)))
#define	USART3_PCLK_DI()	(RCC -> APB1ENR &=(~(1<<18)))
#define	UART4_PCLK_DI()		(RCC -> APB1ENR &=(~(1<<19)))
#define	UART5_PCLK_DI()		(RCC -> APB1ENR &=(~(1<<20)))
#define	USART6_PCLK_DI()	(RCC -> APB1ENR &=(~(1<<5)))



//clock disable macro for SYSCFGx

#define	SYSCFG_PCLK_DI()	(RCC -> APB1ENR &=(~(1<<14)))

//Generic Macros

#define ENABLE 					1
#define DISABLE					0
#define SET						ENABLE
#define RESET					DISABLE

#define GPIO_PIN_SET			SET
#define GPIO_PIN_RESET			RESET


/*
 * IRQ numbers STM32F407XX
 *
 */

#define IRQ_NO_EXTI0		6
#define IRQ_NO_EXTI1		7
#define IRQ_NO_EXTI2		8
#define IRQ_NO_EXTI3		9
#define IRQ_NO_EXTI4		10
#define IRQ_NO_EXTI5_9		23
#define IRQ_NO_EXTI10_15	40
#define IRQ_NO_USART1	    37
#define IRQ_NO_USART2	    38



#include "stm32f407xx_gpio_driver.h"
#include "stm32f407xx_spi_driver.h"
#include "stm32f407xx_i2c_driver.h"
#include "stm32f407xx_usart_driver.h"
#include "stm32f407xx_rcc_driver.h"


#endif /* INC_STM32F407XX_H_ */



