/*
 * rtc_lcd.h
 *
 *  Created on: 12-Mar-2026
 *      Author: dipch
 */

#ifndef FORMAT_UTILS_H_
#define FORMAT_UTILS_H_

char* time_to_string(RTC_time_t*);
char* date_to_string(RTC_date_t*);
char* envData_to_string(wData*);
void envdata_to_txBuf(char*, char*);


#endif /* FORMAT_UTILS_H_ */
