/*
 * command_parser.h
 *
 *  Created on: 14-Mar-2026
 *      Author: dipch
 */

#ifndef COMMAND_PARSER_H_
#define COMMAND_PARSER_H_

void command_handler(char*);
void hm10_reset_command_buffer();
void reset_to_def_date();
uint8_t set_day_name(char*);
void rtc_set_date(char*);
void reset_to_def_time();
void rtc_set_time(char*);
void read_receiveBuffer();


#endif /* COMMAND_PARSER_H_ */
