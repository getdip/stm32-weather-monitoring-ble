/*
 * format_utils.c
 *
 *  Created on: 14-Mar-2026
 *      Author: dipch
 */

#include <stdint.h>
#include <string.h>
#include "ds1307.h"
#include "hm10.h"
#include "bme280.h"
#include "format_utils.h"


/*Date-time data variables*/
extern RTC_time_t current_time;
extern RTC_date_t current_date;

char* day_name[7] = {"SUN","MON","TUE","WED","THU","FRI","SAT"};
static char timeStr[9];

extern char envStr[ENV_DATA_BUF_SIZE];
uint16_t div[4]={1000,100,10,1};
/*Date data: Integer to string conversion */
char* date_to_string(RTC_date_t* current_date){

	uint8_t dd,mm,yy,day;

	dd = current_date->date;
	mm = current_date->month;
	yy = current_date->year;
	day= current_date->day;

	static char dateStr[8];
	dateStr[2]=' ';

	if(dd<10){
		dateStr[0] = 0 + '0';
		dateStr[1] = dd + '0';
	}
	else {
		dateStr[0] = (dd/10) + '0';
		dateStr[1] = (dd %10) + '0';
	}

	for(uint8_t j=3;j<6;j++){
		dateStr[j] = day_name[day][j-3];
	}

	dateStr[6]=' ';
	dateStr[7]=' ';

	return dateStr;
}

/*Time data: Integer to string conversion API */
char* time_to_string(RTC_time_t* current_time){
	/*Format "HH:MM:SS"*/
	uint8_t hh,mm,ss;
	hh= current_time->hours;
	mm = current_time->minutes;
	ss = current_time->seconds;

	timeStr[2]=':';
	timeStr[5]=' ';

	//Hours integer to string
	if(hh<10){
		timeStr[0] = 0 + '0';
		timeStr[1] = hh + '0';
	}
	else{
		timeStr[0] = (hh/10) + '0';
		timeStr[1] = (hh %10) + '0';
	}

	//Minutes integer to string
	if(mm<10){
		timeStr[3] = 0 + '0';
		timeStr[4] = mm + '0';
	}
	else {
		timeStr[3] = (mm/10) + '0';
		timeStr[4] = (mm %10) + '0';
	}
	//Seconds integer to string
	if(ss<10){
		timeStr[6] = 0 + '0';
		timeStr[7] = ss + '0';
	}
	else {
		timeStr[6] = (ss/10) + '0';
		timeStr[7] = (ss %10) + '0';
	}

	/*AM-PM Format setting*/
	if(current_time->time_format == TIME_FORMAT_24HRS){
		timeStr[5]= ':';
		timeStr[8]='\0';
	}
	else if (current_time->time_format == TIME_FORMAT_12HRS_AM) {
		timeStr[6]='A';
		timeStr[7]='M';
		timeStr[8]='\0';
	}
	else if (current_time->time_format == TIME_FORMAT_12HRS_PM) {
		timeStr[6]='P';
		timeStr[7]='M';
		timeStr[8]='\0';
	}
	return timeStr;
}


/*Weather data: Integer to string conversion */
char* envData_to_string(wData* env_data){

	uint8_t digit;
	uint8_t fl=0;
	int32_t temperature = env_data->temperature;
	uint32_t pressure = env_data->pressure;
	uint16_t humidity = env_data->humidity;

	/*Temperature: Integer to string*/

	if(temperature<0) {
		envStr[0]='-';
		uint32_t temperature_abs = (~((uint32_t)temperature -1U));
		digit = temperature_abs/10;
		if(!digit){
			envStr[1]=digit+'0';
			envStr[2] = 'C';
			envStr[3] = ' ';
			envStr[4] = ' ';
		}

		else{
			envStr[1]=digit+'0';
			digit = temperature_abs -(10*digit);
			envStr[2]='0'+digit;
			envStr[3]='C';
			envStr[4] = ' ';
		}
	}

	else if(temperature == 0){
		envStr[0]='0';
		envStr[1] = 'C';
		envStr[2]=' ';
		envStr[3]=' ';
		envStr[4]=' ';
	}

	else{
		digit = temperature / 10;

		if(!digit){
			envStr[0]=digit+'0';
			envStr[1] = 'C';
			envStr[2]=' ';
			envStr[3]=' ';
			envStr[4]=' ';
		}

		else{
			envStr[0]=digit+'0';
			digit = temperature -(10*digit);
			envStr[1]='0'+digit;
			envStr[2]='C';
			envStr[3]=' ';
			envStr[4]=' ';
		}
	}


	/*Pressure: Integer to string*/

	for(uint8_t i =0; i<4;i++){
		digit = (pressure /div[i]);
		if(digit){
			fl=1;
			envStr[i+5] = digit +'0';
			pressure = pressure - (digit * div[i]);
		}
		else{
			if(fl) envStr[i+5]='0';
			else envStr[i+5] = ' ';
		}
	}

	envStr[9] = 'h';
	envStr[10] = 'P';
	envStr[11] = 'a';
	envStr[12] = ' ';

	/*Humidity: Integer to string*/
	fl = 0;
	for(uint8_t i =2; i<4;i++){
		digit = (humidity /div[i]);
		if(digit){
			fl=1;
			envStr[i+11] = digit +'0';
			humidity = humidity - (digit * div[i]);
		}

		else{
			if(fl) envStr[i+11]='0';
			else envStr[i+11] = ' ';
		}
	}
	envStr[15] = '%';
	envStr[16]='\0';

	return envStr;
}

/*Write weather data to txBuf[]*/
void envdata_to_txBuf(char* txBuf, char* envStr){
	for (uint8_t i =0;i<16;i++){
		txBuf[i] = envStr[i];
	}
	txBuf[16]='\r';
	txBuf[17]='\n';
	txBuf[18]='\0';
}
