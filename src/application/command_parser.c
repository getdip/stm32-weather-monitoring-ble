/*
 * APIs.c
 *
 *  Created on: 13-Mar-2026
 *      Author: dipch
 */
#include <stdint.h>
#include <string.h>
#include "hm10.h"
#include "ds1307.h"
#include "command_parser.h"

extern uint8_t def_disp;
extern app_state_t app_state;
extern app_state_t prev_app_state;
extern uint8_t timer_tick_enable;
extern uint8_t cmd_first_byte;

/*Date-time data variables*/
extern RTC_time_t current_time;
extern RTC_date_t current_date;

/*Command parser*/
static uint16_t cmd_parser;

extern char* day_name[7];

uint8_t rxBuf[RX_BUF_SIZE];
extern char txBuf[TX_BUF_SIZE];
static char command[CMD_BUF_SIZE];

/*Default date-time data variables initialization*/
const RTC_time_t default_time = {.hours=0,.minutes=0,.seconds=0,.time_format = TIME_FORMAT_24HRS};
const RTC_date_t default_date = {.date=23, .month=3, .year=26, .day=FRIDAY};

const char MSG_DEVICE_OFF[DEVICE_OFF_MSG_BUF_SIZE] = "DEVICE IS OFF\r\n";
const char DATA_TIME_SET_MSG[DATA_TIME_SET_MSG_BUF_SIZE]="DEVICE MUST BE OFF\r\n";


static uint8_t byte;
volatile uint8_t cmd_cmplt;

/*Reset command buffer*/
void hm10_reset_command_buffer(){
	memset(command,0,CMD_BUF_SIZE);
	cmd_parser=0;
	cmd_cmplt=0;
	cmd_first_byte=0;
}

/*Default date initialization API*/
void reset_to_def_date(){
	ds1307_set_current_date(&default_date);
}

uint8_t set_day_name(char* dname){
	for(uint8_t i=0;i<7;i++){
		if(!strcmp(dname, day_name[i])){
			current_date.day=i;
			return 1;
		}
	}
	return 0;
}

/*User set-date API*/
void rtc_set_date(char* command){

	char* date = command+8;
	uint8_t day,month,year;
	char* dname = date+9;

	if ( (date[2]!='/') || (date[5]!='/') || (date[8]!=' ') || (!set_day_name(dname)) ) {
		reset_to_def_date();
	}

	else{
		day = 	((date[0] - '0')*10) + (date[1] - '0');
		month = ((date[3] - '0')*10) + (date[4] - '0');
		year = 	((date[6] - '0')*10) + (date[7] - '0');

		current_date.date = day;
		current_date.month = month;
		current_date.year = year;

		ds1307_set_current_date(&current_date);
	}
}

/*Default time initialization API*/
void reset_to_def_time(){
	ds1307_set_current_time(&default_time);
}

/*User Set-time API*/
void rtc_set_time(char* command){
	char* time = command+8;

	if ( (time[2]!=':') || (time[5]!=':')){
			reset_to_def_time();
			return;
	}

	else if (time[8]=='\0') {
		current_time.time_format = TIME_FORMAT_24HRS;
	}

	else if(time[8]==' '){

		if (!strcmp(time+9,"AM")){
				current_time.time_format = TIME_FORMAT_12HRS_AM;
		}
		else if (!strcmp(time+9,"PM")){
				current_time.time_format = TIME_FORMAT_12HRS_PM;
		}

		else{
			reset_to_def_time();
			return;
		}
	}

	else{
		reset_to_def_time();
		return;
	}

	uint8_t hr,mn,sec;
	hr = ((time[0] - '0')*10) + (time[1] - '0');
	mn = ((time[3] - '0')*10) + (time[4] - '0');
	sec = ((time[6] - '0')*10)+ (time[7] - '0');

	current_time.hours = hr;
	current_time.minutes = mn;
	current_time.seconds =sec;

	ds1307_set_current_time(&current_time);
}


void command_handler(char* command){

	if(!strcmp(command,"ON")) {

		if(app_state == STATE_IDLE){
			prev_app_state = app_state;
			app_state = STATE_ACTIVE;
			timer_tick_enable = 1;
		}
	}

	else if(!strcmp(command,"stream")) {
		if(app_state == STATE_ACTIVE){
			prev_app_state = app_state;
			app_state = STATE_STREAMING;
		}
		else if(app_state == STATE_IDLE){
			if(hm10_usart_txstate() == USART_READY){
				hm10_send_dataIT((uint8_t*)MSG_DEVICE_OFF, DEVICE_OFF_MSG_BUF_SIZE-1);
			}
		}
	}

	else if(!strcmp(command,"stop")){
		if(app_state == STATE_STREAMING){
			prev_app_state = app_state;
			app_state = STATE_ACTIVE;
			timer_tick_enable = 1;
		}
	}

	else if(!strcmp(command,"OFF")) {
		prev_app_state = app_state;
		app_state = STATE_IDLE;
	}

	else if(!strncmp(command,"settime:",8)){
		if(app_state == STATE_IDLE){
			rtc_set_time(command);
		}
		else{
			if(hm10_usart_txstate() == USART_READY){
				hm10_send_dataIT((uint8_t*)DATA_TIME_SET_MSG, DATA_TIME_SET_MSG_BUF_SIZE-1);
			}
		}
	}

	else if(!strncmp(command,"setdate:",8)){
		if(app_state == STATE_IDLE){
			rtc_set_date(command);
		}

		else {
			if(hm10_usart_txstate() == USART_READY){
				hm10_send_dataIT((uint8_t*)DATA_TIME_SET_MSG, DATA_TIME_SET_MSG_BUF_SIZE-1);
			}
		}
	}

	/*Reset the command buffer*/
	hm10_reset_command_buffer();
}


void read_receiveBuffer(){

	byte = rxBuf[rx_read_head++];
	rx_read_head &= (RX_BUF_SIZE-1);
	if(byte == '#'){
		command[cmd_parser]='\0';
		cmd_cmplt = 1;
		cmd_first_byte=0;
		command_handler(command);
	}
	else{
		command[cmd_parser++]=byte;
	}
}

