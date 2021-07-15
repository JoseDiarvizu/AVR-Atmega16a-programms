/*
 * Titulo
 *
 * Created: Fecha
 * Author : Nombre
 */ 

#define  F_CPU 1000000UL
#include <avr/io.h>				//Librería general del Microcontrolador
#include <stdint.h>				//Para poder declarar varialbes especiales
#include <util/delay.h>			//Para poder hacer delays
#include "lcd.h"
//#include <avr/interrupt.h>	 //Para poder manejar interrupciones

//PROTOTIPADO DE FUNCIONES PARA PODER UTILIZARLAS DESDE CUALQUIER "LUGAR"
//*************************************************************************
uint8_t cero_en_bit(volatile uint8_t *LUGAR, uint8_t BIT);
uint8_t uno_en_bit(volatile uint8_t *LUGAR, uint8_t BIT);
//*************************************************************************


int main(void)
{
	lcd_init(LCD_DISP_ON);
	lcd_gotoxy(7,0);
	
	
	lcd_puts("QWERQWERQWERQWE");
	_delay_ms(2000);
	

	
	lcd_command(LCD_MOVE_DISP_RIGHT);
	_delay_ms(2000);
	lcd_command(LCD_MOVE_DISP_LEFT);
    while (1) 
    {
		
    }
	
}