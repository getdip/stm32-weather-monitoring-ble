/*
 * rtc_lcd.c
 *
 *  Created on: Dec 14, 2025
 *      Author: dipch
 */


#include <stdint.h>
#include <string.h>
#include "ds1307.h"
#include "lcd.h"
#include "bme280.h"
#include "hm10.h"
#include "format_utils.h"
#include "command_parser.h"

#define SYSTICK_TIM_CLK   16000000UL



/*Date-time data variables*/
RTC_time_t current_time;
RTC_date_t current_date;

/*Temperature-pressure-humidity data variable*/
static wData env_data;

/*SysTick Timer*/
volatile uint8_t timer_tick_enable;
volatile static uint8_t tick_flag;
volatile static uint32_t tick_count;
volatile static uint32_t cmd_first_byte_time;
volatile static uint32_t tick_count_sec;
volatile static uint32_t tick_x;
volatile static uint32_t delay;

/*LCD display flags*/
volatile uint8_t LCD_disp_clear_requested;
volatile uint8_t lcd_disp_ready;
volatile uint8_t def_disp=0;
volatile static uint8_t display_clear_fl;

/*Application state*/
volatile app_state_t app_state = STATE_IDLE;
volatile app_state_t prev_app_state = STATE_IDLE;
volatile uint32_t error_fl;

/*User notification messages*/
const char MSG_BME280_ENABLED[BME280_ENABLED_BUF_SIZE] = "Sensor BME280 Enabled\r\n";
const char MSG_BME280_NOT_ENABLED[BME280_NOT_ENABLED_BUF_SIZE] = "Sensor BME280 failed to Initialize\r\n";
const char MSG_DS1307_ENABLED[DS1307_ENABLED_BUF_SIZE]="Sensor DS1307 Enabled\r\n";
const char MSG_DS1307_NOT_ENABLED[DS1307_NOT_ENABLED_BUF_SIZE]="Sensor DS1307 failed to Initialize\r\n";
const char str1[DEFAULT_STRING1] = "Weather Station";
const char str2[DEFAULT_STRING2] = "Device is OFF";
const char MSG_OVERFLOW[OVERFLOW_MSG_BUF_SIZE]="ERR:OVERFLOW\r\n";
const char MSG_OVERRUN[OVERRUN_MSG_BUF_SIZE] = "ERR:OVERRUN\r\n";

/*Data Buffers*/
char txBuf[TX_BUF_SIZE];
char envStr[ENV_DATA_BUF_SIZE];

/*Command buffer first byte*/
volatile uint8_t cmd_first_byte;
extern uint8_t cmd_cmplt;

/*LCD display clear with delay passed as a parameter*/
void LCD_disp_clear(uint32_t delay_ms){
	lcd_disp_ready = 0;
	lcd_ins(LCD_INS_DIS_CLEAR);
	LCD_disp_clear_requested=1;
	tick_x = tick_count;
	delay = delay_ms;
}

/*SysTick TImer API*/
void init_systick_timer(uint32_t tick_hz)
{

    /* calculation of reload value */
    uint32_t count_value = (SYSTICK_TIM_CLK/tick_hz)-1;

    //Clear the value of SVR
    *SYST_RVR_ADDR &= ~(0x00FFFFFFFF);

    //load the value in to SVR
    *SYST_RVR_ADDR |= count_value;

    //do some settings
    *SYST_CSR_ADDR |= ( 1 << 1); //Enables SysTick exception request:
    *SYST_CSR_ADDR |= ( 1 << 2);  //Indicates the clock source, processor clock source

    //enable the systick
    *SYST_CSR_ADDR |= ( 1 << 0);
}

/*ACTIVE state API*/
void hm10_uart_active(){

	if(display_clear_fl){
		display_clear_fl = 0;
		LCD_disp_clear(2);
	}

	if(lcd_disp_ready){

		lcd_disp_ready = 0;
		/*Trigger BME280 forced mode*/
		bme280_forced_mode_trigger();

		/*Get environmental data*/
		bme280_get_envData(&env_data);

		/*Populate the envStr[]*/
		envData_to_string(&env_data);

		/*Get current date and time*/
		ds1307_get_current_time(&current_time);
		ds1307_get_current_date(&current_date);

		/*Display on the LCD*/
		lcd_disp_string(date_to_string(&current_date),1,0);
		lcd_disp_string(time_to_string(&current_time),1,8);
		lcd_disp_string(envData_to_string(&env_data),2,0);
	}

	else if(tick_flag){

		tick_flag=0;
		/*Trigger BME280 forced mode*/
		bme280_forced_mode_trigger();

		/*Get environmental data*/
		bme280_get_envData(&env_data);

		/*Populate the envStr[]*/
		envData_to_string(&env_data);

		/*Get current date and time*/
		ds1307_get_current_time(&current_time);
		ds1307_get_current_date(&current_date);

		/*Display on the LCD*/
		lcd_disp_string(date_to_string(&current_date),1,0);
		lcd_disp_string(time_to_string(&current_time),1,8);
		lcd_disp_string(envData_to_string(&env_data),2,0);
	}

}

/*STREAMING state API*/
void hm10_uart_stream(){

	if(tick_flag){

		tick_flag=0;
		ds1307_get_current_time(&current_time);
		ds1307_get_current_date(&current_date);

		/*Trigger BME280 forced mode*/
		bme280_forced_mode_trigger();

		/*Get environmental data*/
		bme280_get_envData(&env_data);

		/*Populate the envStr[]*/
		envData_to_string(&env_data);

		if(hm10_usart_txstate() == USART_READY){
			envdata_to_txBuf(txBuf,envStr);
			hm10_send_dataIT((uint8_t*)txBuf, ENV_DATA_TX_BUF_SIZE);

			lcd_disp_string(date_to_string(&current_date),1,0);
			lcd_disp_string(time_to_string(&current_time),1,8);
			lcd_disp_string(envData_to_string(&env_data),2,0);
		}
	}
}

/*ERROR state API*/
void hm10_uart_error(){

	/*Receive Buffer overflow detected*/
	if(error_fl & HM10_ERR_RX_OVERFLOW){
		hm10_reset_rx_buffer();
		hm10_reset_command_buffer();
		memcpy(txBuf,MSG_OVERFLOW,sizeof(MSG_OVERFLOW)-1);
		if(hm10_usart_txstate() == USART_READY){
			hm10_send_dataIT((uint8_t*)txBuf, OVERFLOW_MSG_BUF_SIZE);
		}
		hm10_clear_errors(HM10_ERR_RX_OVERFLOW);
	}

	/*UART overrun error detected*/
	else if(error_fl & HM10_ERR_RX_ORE){
		hm10_reset_rx_buffer();
		memcpy(txBuf,MSG_OVERRUN,sizeof(MSG_OVERRUN)-1);
		if(hm10_usart_txstate() == USART_READY){
			hm10_send_dataIT((uint8_t*)txBuf, OVERRUN_MSG_BUF_SIZE);
		}
		hm10_clear_errors(HM10_ERR_RX_ORE);
	}

	app_state = STATE_ACTIVE;
}

/*IDLE state API*/
void hm10_uart_IDLE(){

	if(display_clear_fl){
		display_clear_fl = 0;
		LCD_disp_clear(10);
	}

	if(lcd_disp_ready){
		lcd_disp_ready=0;
		lcd_disp_string((char*)str1,1,0);
		lcd_disp_string((char*)str2,2,0);
	}
}


int main (void)
{
	/*HM-10 Initialization*/
	hm10_init();

	/*Enable UART RX interrupt*/
	usart_enable_RXIT();

	/*Configure SysTick TImer for 1 sec tick*/
	init_systick_timer(1000);

	/*Enable UART peripheral interrupt to NVIC*/
	USART_IRQInterruptConfig(IRQ_NO_USART2,ENABLE);

	/*Initialize LCD*/
	lcd_init();


	/*Initialize DS1307 RTC module
	 * Initialization fails if CH bit is 1*/

	if(!ds1307_init()) {
		while(!(hm10_usart_txstate() == USART_READY));
		hm10_send_dataIT((uint8_t*)MSG_DS1307_ENABLED, DS1307_ENABLED_BUF_SIZE-1);
	}
	else{
		while(!(hm10_usart_txstate() == USART_READY));
		hm10_send_dataIT((uint8_t*)MSG_DS1307_NOT_ENABLED, DS1307_NOT_ENABLED_BUF_SIZE-1);
	}


	/*Initialize BME280 sensor module*/
	if(bme280_init() == ENABLED){
		while(!(hm10_usart_txstate() == USART_READY));
		hm10_send_dataIT((uint8_t*)MSG_BME280_ENABLED, BME280_ENABLED_BUF_SIZE-1);

	}
	else{
		while(!(hm10_usart_txstate() == USART_READY));
		hm10_send_dataIT((uint8_t*)MSG_BME280_NOT_ENABLED, BME280_NOT_ENABLED_BUF_SIZE-1);
	}


	/*Date-Time initialization of DS1307 RTC*/
	//ds1307_data_time_initialization(&current_time,&current_date);

	/*Temperature-Pressure-Humidity acquisition*/
	bme280_get_envData(&env_data);

	/*Date and Time acquisition */
	ds1307_get_current_time(&current_time);
	ds1307_get_current_date(&current_date);

	/*Clearing out LCD display
	 * Default screen display*/

	LCD_disp_clear(200);
	while(!lcd_disp_ready){}

	lcd_disp_ready=0;
	lcd_disp_string((char*)str1,1,0);
	lcd_disp_string((char*)str2,2,0);

	while(1){

		/*For each received byte in "rxBuf" the command buffer "command" is build based on '#' delimiter
		 * Once delimiter character is found coammand_handler is invoked*/

		while(rx_read_head !=rx_write_head){
			error_fl = hm10_get_errors();
			if(error_fl!=HM10_ERR_NONE){
				app_state = STATE_ERROR;
				hm10_uart_error();
			}

			else{
				if(!cmd_first_byte){
					cmd_first_byte_time = tick_count;
					cmd_first_byte=1;
				}
				read_receiveBuffer();
			}
		}

		/*Timeout for command buffer*/
		if(cmd_first_byte){
			if(((tick_count - cmd_first_byte_time)>1000) && (!cmd_cmplt)){
				hm10_reset_command_buffer();
			}
		}

		/*Application State Handling -
		 * Application states: STATE_IDLE, STATE_ACTIVE, STATE_STREAMING, STATE_ERROR */

		if(timer_tick_enable){

			switch (app_state){
				case STATE_STREAMING:
					hm10_uart_stream();
					break;
				case STATE_ACTIVE:
					if ((prev_app_state == STATE_IDLE)||(prev_app_state == STATE_ERROR)){
						prev_app_state = STATE_ACTIVE;
						display_clear_fl = 1;
					}
					hm10_uart_active();
					break;
				case STATE_ERROR:
					hm10_uart_error();
					break;
				case STATE_IDLE:
					if (((prev_app_state == STATE_ACTIVE)||(prev_app_state == STATE_STREAMING)||(prev_app_state == STATE_ERROR)) && (!display_clear_fl)){
						prev_app_state = STATE_IDLE;
						display_clear_fl = 1;
					}
					hm10_uart_IDLE();
					break;
				default:
					app_state = STATE_IDLE;
					break;
			}
		}
	}
}

/*SysTick timer handler*/
void SysTick_Handler(void){
	tick_count++;

	if(++tick_count_sec >= 1000){
		tick_count_sec=0;
		if(timer_tick_enable){
				tick_flag=1;
			}
	}

	if((LCD_disp_clear_requested) && ((tick_count - tick_x)>=delay)){
		LCD_disp_clear_requested=0;
		lcd_disp_ready=1;
	}
}
