/*
 * lcd.c
 *
 *  Created on: Dec 12, 2025
 *      Author: dipch
 */


#include "lcd.h"

static void mdelay(uint64_t x)
{
	for(uint64_t i=0 ; i < (x * 1000); i++);
}

static void udelay(uint64_t x)
{
	for(uint64_t i=0 ; i < (x * 1); i++);
}

static void lcd_pulse(){

	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_EN, GPIO_PIN_SET);
	udelay(10);
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_EN, GPIO_PIN_RESET);
	udelay(50);
}

static void data_write(uint8_t data){

	//printf("data = %X\n",data);
	GPIO_RegDef_t* pGPIOx = LCD_GPIO_PORT;
	uint32_t temp;
	temp = (0x0000000F << 3) & (data<<3);
	pGPIOx->ODR &= ~(0x0000000F << 3);
	pGPIOx->ODR |= temp;

	/*Generate the pulse at EN pin to latch the data on to the LCD*/
	lcd_pulse();
}

void lcd_init(){

	/*Config GPIO pins for LCD*/

	GPIO_Handle_t lcd_disp;

	lcd_disp.pGPIOx = LCD_GPIO_PORT;
	lcd_disp.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	lcd_disp.GPIO_PinConfig.GPIO_PinNumber = LCD_GPIO_RS;
	lcd_disp.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	lcd_disp.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	lcd_disp.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIO_Init(&lcd_disp);

	lcd_disp.GPIO_PinConfig.GPIO_PinNumber = LCD_GPIO_RW;
	GPIO_Init(&lcd_disp);

	lcd_disp.GPIO_PinConfig.GPIO_PinNumber = LCD_GPIO_EN;
	GPIO_Init(&lcd_disp);

	lcd_disp.GPIO_PinConfig.GPIO_PinNumber = LCD_GPIO_D4;
	GPIO_Init(&lcd_disp);

	lcd_disp.GPIO_PinConfig.GPIO_PinNumber = LCD_GPIO_D5;
	GPIO_Init(&lcd_disp);

	lcd_disp.GPIO_PinConfig.GPIO_PinNumber = LCD_GPIO_D6;
	GPIO_Init(&lcd_disp);

	lcd_disp.GPIO_PinConfig.GPIO_PinNumber = LCD_GPIO_D7;
	GPIO_Init(&lcd_disp);

	/*All GPIO pins are at reset*/
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_RS, GPIO_PIN_RESET);
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_RW, GPIO_PIN_RESET);
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_EN, GPIO_PIN_RESET);
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_D4, GPIO_PIN_RESET);
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_D5, GPIO_PIN_RESET);
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_D6, GPIO_PIN_RESET);
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_D7, GPIO_PIN_RESET);

	/*LCD power-on initialization sequence for 4-bit mode LCD <D4-D7>*/
	mdelay(40);

	/*<RS-RW-DB7-DB6-DB5-DB4>  -  <0-0-0-0-1-1>*/
	/*Reset the RS pin*/
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_RS, GPIO_PIN_RESET);

	/* Reset the RW pin*/
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_RW, GPIO_PIN_RESET);
	data_write(0x03);
	mdelay(5);
	data_write(0x03);
	udelay(120);
	data_write(0x03);
	data_write(0x02);

	/*Function set instruction <RS-RW-DB7-DB6-DB5-DB4-DB3-DB2-DB1-DB0>
	 * 					   	   <0  0  0   0   1   DL  N   F   X   X >*/
	lcd_ins(LCD_INS_4DL_2N_5X8F);

	/*Display on/off instruction <RS-RW-DB7-DB6-DB5-DB4-DB3-DB2-DB1-DB0>
	* 					     	 <0  0  0   0   0   0   1   D   C   B >*/
	lcd_ins(LCD_INS_DON_CURON);

	/*Display clear instruction <RS-RW-DB7-DB6-DB5-DB4-DB3-DB2-DB1-DB0>
	* 					    	<0  0  0   0   0   0   0   0   0   1 >*/
	lcd_ins(LCD_INS_DIS_CLEAR);
	mdelay(2);

	/*Entry mode set instruction <RS-RW-DB7-DB6-DB5-DB4-DB3-DB2-DB1-DB0>
	* 					    	 <0  0  0   0   0   0   0   1   I/D S >*/
	lcd_ins(LCD_INS_INCADD);
}

void lcd_ins(uint8_t ins){

	/*Reset the RS pin*/
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_RS, GPIO_PIN_RESET);

	/*Reset the RW pin*/
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_RW, GPIO_PIN_RESET);

	/*4 bit mode - Send the upper nibble followed by the lower nibble*/
	data_write(((ins>>4)&0x0F));
	data_write(ins & 0x0F);
}

void lcd_disp_byte(uint8_t byte,uint8_t line,uint8_t pos){

	/*Set the cursor to the correct position of the line*/
	if(line == 1){
		pos|=0x80;
		lcd_ins(pos);
	}
	else if(line ==2){
		pos|=0xC0;
		lcd_ins(pos);
	}

	/*Set the RS pin*/
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_RS, GPIO_PIN_SET);

	/*Reset the RW pin*/
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_RW, GPIO_PIN_RESET);

	/*4 bit mode - Send the upper nibble followed by the lower nibble*/
	data_write(((byte>>4)&0x0F));

	data_write(byte & 0x0F);
}

void lcd_disp_string(char* str,uint8_t line,uint8_t pos){

	uint64_t i=0;
	while(*(str+i) != '\0'){

		lcd_disp_byte((uint8_t)*(str+i),line,pos);
		pos++;
		i++;
	}
}




