/*
 * bme280.c
 *
 *  Created on: Dec 28, 2025
 *      Author: dipch
 */

#include<stdint.h>
#include <string.h>
#include "bme280.h"

/*Calibration static values*/

static uint16_t dig_T1;
static int16_t dig_T2;
static int16_t dig_T3;

static uint16_t dig_P1;
static int16_t dig_P2;
static int16_t dig_P3;
static int16_t dig_P4;
static int16_t dig_P5;
static int16_t dig_P6;
static int16_t dig_P7;
static int16_t dig_P8;
static int16_t dig_P9;

static uint8_t dig_H1;
static int16_t dig_H2;
static uint8_t dig_H3;
static int16_t dig_H4;
static int16_t dig_H5;
static int8_t dig_H6;

/*BOSCH API related variable*/
BME280_S32_t adc_P,adc_T,adc_H;

/*Static functions*/
static void bme280_i2c_pin_config(void);
static void bme280_i2c_config(void);
static uint8_t data_read(uint8_t);
static void data_write(uint8_t, uint8_t);
static void data_burst_read(uint8_t,uint8_t*,uint32_t);
static void calibration_variables_init();
static void sensor_data_variables_init();

/*BOSCH APIs for data compensation*/
static BME280_S32_t BME280_compensate_T_int32(int32_t);
static BME280_U32_t BME280_compensate_P_int32(int32_t);
static BME280_U32_t BME280_compensate_H_int32(int32_t);

volatile BME280_S32_t t_fine;

/*Data Burst read dump*/
static uint8_t burst_read_arr[2048];

/*Calibration register content array */
static uint16_t calibration_register_list_PT [12]={0};
static uint8_t calibration_register_list_H [8]={0};

/*Sensor data register content array */
static uint8_t data_register_list [8]={0};

/* GPIO -SDA
 * GPIO -SCL*/
GPIO_Handle_t i2c_sda, i2c_scl;

/*I2C*/
I2C_Handle_t bme280Handle;


static BME280_S32_t BME280_compensate_T_int32(BME280_S32_t adc_T)
{
	BME280_S32_t var1, var2, T;
	//var1  = ((((adc_T>>3) – ((BME280_S32_t)dig_T1<<1))) * ((BME280_S32_t)dig_T2)) >> 11;
	var1 = (((adc_T >> 3) - ((BME280_S32_t)dig_T1<<1)) * ((BME280_S32_t)dig_T2)) >> 11  ;
	var2 = (((((adc_T >> 4) - ((BME280_S32_t)dig_T1)) * ((adc_T >> 4) - ((BME280_S32_t)dig_T1))) >> 12) *  ((BME280_S32_t)dig_T3)) >> 14;

	var2 =
	t_fine = var1 + var2;
	T  = (t_fine * 5 + 128) >> 8;
	return T;
}

BME280_U32_t BME280_compensate_P_int32(BME280_S32_t adc_P)
{
	BME280_S32_t var1, var2;
	BME280_U32_t p;
    var1 = ((BME280_S32_t)t_fine >> 1) - (BME280_S32_t)64000;
    var2 = (((var1 >> 2) * (var1 >> 2)) >> 11) * (BME280_S32_t)dig_P6;
    var2 = var2 + ((var1 * (BME280_S32_t)dig_P5) << 1);
    var2 = (var2 >> 2) + ((BME280_S32_t)dig_P4 << 16);
    var1 = (((BME280_S32_t)dig_P3 * (((var1 >> 2) * (var1 >> 2)) >> 13)) >> 3) +
           (((BME280_S32_t)dig_P2 * var1) >> 1);
    var1 = (var1 >> 18);
    var1 = (((BME280_S32_t)32768 + var1) * (BME280_S32_t)dig_P1) >> 15;

    if (var1 == 0)
    {
        return 0;   // avoid exception caused by division by zero
    }

    p = (((BME280_U32_t)((BME280_S32_t)1048576 - adc_P) - (BME280_U32_t)(var2 >> 12)) * 3125U);

    if (p < 0x80000000U)
    {
        p = (p << 1) / (BME280_U32_t)var1;
    }
    else
    {
        p = (p / (BME280_U32_t)var1) * 2U;
    }

    var1 = ((BME280_S32_t)dig_P9 * (BME280_S32_t)(((p >> 3) * (p >> 3)) >> 13)) >> 12;
    var2 = ((BME280_S32_t)(p >> 2) * (BME280_S32_t)dig_P8) >> 13;

    p = (BME280_U32_t)((BME280_S32_t)p + ((var1 + var2 + (BME280_S32_t)dig_P7) >> 4));

    return p;   // pressure in Pa
}
BME280_U32_t BME280_compensate_H_int32(BME280_S32_t adc_H)
{
	BME280_S32_t v_x1_u32r;
	v_x1_u32r = (t_fine - ((BME280_S32_t)76800));

	v_x1_u32r = (((((adc_H << 14) - (((BME280_S32_t)dig_H4) << 20) - (((BME280_S32_t)dig_H5) *
	v_x1_u32r)) + ((BME280_S32_t)16384)) >> 15) * (((((((v_x1_u32r *
	((BME280_S32_t)dig_H6)) >> 10) * (((v_x1_u32r * ((BME280_S32_t)dig_H3)) >> 11) +
	((BME280_S32_t)32768))) >> 10) + ((BME280_S32_t)2097152)) * ((BME280_S32_t)dig_H2) +
	8192) >> 14));
	v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) *
	((BME280_S32_t)dig_H1)) >> 4));
	v_x1_u32r = (v_x1_u32r < 0 ? 0 : v_x1_u32r);
	v_x1_u32r = (v_x1_u32r > 419430400 ? 419430400 : v_x1_u32r);
	return (BME280_U32_t)(v_x1_u32r>>12);
}


uint8_t bme280_init(){

	uint8_t chip_id;

	//1. init the i2c pins /*SDA : PB11 , SCL: PB10*/
	bme280_i2c_pin_config();

	//2. initialize the i2c peripheral
	bme280_i2c_config();

	if(bme280Handle.pI2Cx->I2C_SR2 & (1<<I2C_SR2_BUSY)){
		I2C_GenerateStopCondition(bme280Handle.pI2Cx);
	}

	//3. Enable the I2C peripheral
	I2C_PeripheralControl(BME280_I2C, ENABLE);

	//4. Read the chip_id
	chip_id = data_read(BME280_ADDR_ID);

	if( chip_id == BME280_CHIP_ID){

		/*Device soft reset*/
		data_write(BME280_VAL_RESET, BME280_ADDR_RESET);

		/*Configures ctrl_hum register 0xF2*/
		data_write(OVRS_RATE_4,BME280_ADDR_CTRL_HUM);

		/*IIR filter off 0xF5*/
		data_write(IIR_FILTER_DISABLE,BME280_ADDR_CONFIG);

		/*Configures ctrl_meas register 0xF4*/
		uint8_t tempreg = 0x00;
		tempreg |= (FORCED << 0);
		tempreg |= (OVRS_RATE_2 << 2);
		tempreg |= (OVRS_RATE_2 << 5);
		data_write(tempreg,BME280_ADDR_CTRL_MEAS);

		/*Reset the array - calibration_register_list[]*/
		memset(burst_read_arr,0,2048);

		//For dig_Tx , dig_Px calibration registers
		data_burst_read(CALIB_REG_BASEADDR_TMPR,burst_read_arr,CALIB_REG_COUNT_PT);

		/*Initialize Calibration register array - calibration_register_list*/
		calibration_register_list_H[0] =  burst_read_arr[25];
		for(uint32_t i=0;i<=11;i++){
			calibration_register_list_PT[i] = (uint16_t)((burst_read_arr[2*i]) | (burst_read_arr[2*i +1]<<8));
		}

		//For  dig_Hx calibration registers
		memset(burst_read_arr,0,2048);
		data_burst_read(CALIB_REG_BASEADDR_HUM,burst_read_arr,CALIB_REG_COUNT_H);

		for(uint32_t i=1;i<=7;i++){

			calibration_register_list_H[i] = (uint8_t)burst_read_arr[i-1];
			}

		if(bme280Handle.pI2Cx->I2C_SR2 & (1<<I2C_SR2_BUSY)){
				//I2C_Bus_Recovery(&bme280Handle,&i2c_sda,&i2c_scl);
				I2C_GenerateStopCondition(bme280Handle.pI2Cx);
			}

		calibration_variables_init();

		return ENABLED;

	}

	return NOT_ENABLED;
}


void bme280_forced_mode_trigger(void){

	/*Configures ctrl_hum register 0xF2*/
	data_write(OVRS_RATE_4,BME280_ADDR_CTRL_HUM);

	/*Configures ctrl_meas register 0xF4*/
	uint8_t tempreg = 0x00;
	tempreg |= (FORCED << 0);
	tempreg |= (OVRS_RATE_2 << 2);
	tempreg |= (OVRS_RATE_2 << 5);
	data_write(tempreg,BME280_ADDR_CTRL_MEAS);
}

static void bme280_i2c_pin_config(){

	i2c_sda.pGPIOx = BME280_I2C_GPIO_PORT;
	i2c_sda.GPIO_PinConfig.GPIO_PinAltFunMode = 4;
	i2c_sda.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	i2c_sda.GPIO_PinConfig.GPIO_PinNumber = BME280_I2C_SDA_PIN;
	i2c_sda.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;
	i2c_sda.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
	i2c_sda.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	GPIO_Init(&i2c_sda);

	i2c_scl.pGPIOx = BME280_I2C_GPIO_PORT;
	i2c_scl.GPIO_PinConfig.GPIO_PinAltFunMode = 4;
	i2c_scl.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	i2c_scl.GPIO_PinConfig.GPIO_PinNumber = BME280_I2C_SCL_PIN;
	i2c_scl.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;
	i2c_scl.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
	i2c_scl.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	GPIO_Init(&i2c_scl);
}


static void bme280_i2c_config(){

	bme280Handle.pI2Cx = BME280_I2C;
	bme280Handle.I2C_Config.I2C_AckControl=I2C_ACK_ENABLE;
	bme280Handle.I2C_Config.I2C_SCLSpeed = BME280_I2C_SPEED;
	I2C_Init(&bme280Handle);
}


static uint8_t data_read( uint8_t reg_addr){

	uint8_t data;

	I2C_MasterSendData(&bme280Handle,&reg_addr, 1, BME280_I2C_ADDRESS,0);
	I2C_MasterReceiveData(&bme280Handle,&data, 1, BME280_I2C_ADDRESS,0);

	return data;
}


static void data_write(uint8_t val,  uint8_t addr){

	uint8_t tx[2];
	tx[0] =addr;
	tx[1] = val;
	I2C_MasterSendData(&bme280Handle, tx, 2, BME280_I2C_ADDRESS, 0);
}

static void data_burst_read(uint8_t baseAddr,uint8_t* dataBuffer,uint32_t count){

	I2C_MasterReceiveBurstData(&bme280Handle,baseAddr,count,dataBuffer,BME280_I2C_ADDRESS,0);
}

void bme280_get_envData(wData* data){
	uint8_t temp;
	//wait until bit3 of STATUS register (0xF3) is reset. - Conversion Finished
	do{
		temp=data_read(BME280_ADDR_STATUS);
	}while(temp& (0x01<<3));

	data_burst_read(PRESSURE_DATA_BASEADDR, burst_read_arr, SENSOR_DATA_REG_COUNT);

	for(uint8_t i=0;i<8;i++){
		data_register_list[i] = burst_read_arr[i];
	}

	sensor_data_variables_init();
	data->temperature = BME280_compensate_T_int32((int32_t)adc_T) / 100;
	data->pressure = BME280_compensate_P_int32((int32_t)adc_P) / 100;
	data->humidity = BME280_compensate_H_int32((int32_t)adc_H) /1024 ;
}

static void calibration_variables_init()
{
	dig_T1 = (uint16_t)calibration_register_list_PT[0];
	dig_T2 = (int16_t)calibration_register_list_PT[1];
	dig_T3 = (int16_t)calibration_register_list_PT[2];

	dig_P1 = (uint16_t)calibration_register_list_PT[3];
	dig_P2 = (int16_t)calibration_register_list_PT[4];
	dig_P3 = (int16_t)calibration_register_list_PT[5];
	dig_P4 = (int16_t)calibration_register_list_PT[6];
	dig_P5 = (int16_t)calibration_register_list_PT[7];
	dig_P6 = (int16_t)calibration_register_list_PT[8];
	dig_P7 = (int16_t)calibration_register_list_PT[9];
	dig_P8 = (int16_t)calibration_register_list_PT[10];
	dig_P9 = (int16_t)calibration_register_list_PT[11];

	dig_H1 = (uint8_t)calibration_register_list_H[0];
	dig_H2 = (int16_t)((calibration_register_list_H[2]<<8) | (calibration_register_list_H[1]));
	dig_H3 = (uint8_t)(calibration_register_list_H[3]);
	dig_H4 = (int16_t)(((int16_t)calibration_register_list_H[4] << 4) |
	                   (calibration_register_list_H[5] & 0x0F));
	dig_H5 = (int16_t)(((int16_t)calibration_register_list_H[6] << 4) |
	                   ((calibration_register_list_H[5] & 0xF0) >> 4));
	dig_H6 = (int8_t)calibration_register_list_H[7];

}

static void sensor_data_variables_init(){
	adc_P =( ((uint32_t)data_register_list[0] << 12) | ((uint32_t)data_register_list[1] << 4) | ((uint32_t)data_register_list[2] >> 4));
	adc_T =( ((uint32_t)data_register_list[3] << 12) | ((uint32_t)data_register_list[4] << 4) | ((uint32_t)data_register_list[5] >> 4));
	adc_H = ( ((uint16_t)data_register_list[6] << 8)  | ((uint16_t)data_register_list[7])) ;

}
