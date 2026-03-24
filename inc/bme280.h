/*
 * bme280.h
 *
 *  Created on: Dec 28, 2025
 *      Author: dipch
 */

#ifndef BME280_H_
#define BME280_H_

#include "stm32f407xx.h"
typedef  int32_t  BME280_S32_t;
typedef  uint32_t BME280_U32_t;
typedef  uint16_t BME280_U16_t;

typedef struct {

	BME280_S32_t temperature;
	BME280_U32_t pressure;
	BME280_U32_t humidity;

}wData;



/*typedef struct {

	uint8_t* reg;
	uint16_t val;
}calib_regDef;*/



/*Configuration Macros*/

#define SLEEP  0
#define FORCED 1
#define NORMAL 3

#define OVRS_RATE_0 	0
#define OVRS_RATE_1 	1
#define OVRS_RATE_2 	2
#define OVRS_RATE_4 	3
#define OVRS_RATE_8 	4
#define OVRS_RATE_16 	5

#define IIR_FILTER_DISABLE	0
#define IIR_FILTER_2		1
#define IIR_FILTER_4		2
#define IIR_FILTER_8		3
#define IIR_FILTER_16		4

#define BME280_I2C_ADDRESS      0x77
#define BME280_CHIP_ID			0x60
#define BME280_VAL_RESET 		0xB6

/*I2C specific configurable items for bme280 */

#define BME280_I2C  			I2C2
#define BME280_I2C_GPIO_PORT    GPIOB
#define BME280_I2C_SDA_PIN 		GPIO_PIN_NO_11
#define BME280_I2C_SCL_PIN 		GPIO_PIN_NO_10
#define BME280_I2C_SPEED 		I2C_SCL_SPEED_SM
#define BME280_I2C_PUPD			GPIO_PIN_PU

/*Register addresses */
#define BME280_ADDR_ID			0xD0
#define BME280_ADDR_RESET 		0xE0
#define BME280_ADDR_CTRL_HUM	0xF2
#define BME280_ADDR_STATUS		0xF3
#define BME280_ADDR_CTRL_MEAS	0xF4
#define BME280_ADDR_CONFIG		0xF5

#define BME280_ADDR_PRESS_MSB	0xF7
#define BME280_ADDR_PRESS_LSB	0xF8
#define BME280_ADDR_PRESS_XLSB	0xF9

#define BME280_ADDR_TEMP_MSB	0xFA
#define BME280_ADDR_TEMP_LSB	0xFB
#define BME280_ADDR_TEMP_XLSB	0xFC

#define BME280_ADDR_HUM_MSB		0xFD
#define BME280_ADDR_HUM_LSB		0xFE

/*Calibration registers macros*/

#define idxT1 0
#define idxT2 1
#define idxT3 2
#define idxP1 3
#define idxP2 4
#define idxP3 5
#define idxP5 6
#define idxP6 7
#define idxP7 8
#define idxP8 8
#define idxP9 10
#define idxH1 11
#define idxH2 12
#define idxH3 13
#define idxH4 14
#define idxH5 15
#define idxH6 16

/*Calibration registers*/

#define CALIB_REG_BASEADDR_TMPR 			0x88
#define CALIB_REG_COUNT_PT					25
#define CALIB_REG_COUNT_H					6
#define CALIB_REG_BASEADDR_HUM 				0xE1
#define PRESSURE_DATA_BASEADDR				0xF7
#define SENSOR_DATA_REG_COUNT				7

/*BME280 Initialization states*/
#define ENABLED		0
#define NOT_ENABLED 1


/*BME280 APIs*/

uint8_t bme280_init();
void bme280_get_data(wData*);
void bme280_get_envData(wData*);
void bme280_forced_mode_trigger(void);
void bme280_calibration_reg_init();

#endif
