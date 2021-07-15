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
#include <avr/interrupt.h>	 //Para poder manejar interrupciones
#include "lcd.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
//PROTOTIPADO DE FUNCIONES PARA PODER UTILIZARLAS DESDE CUALQUIER "LUGAR"
//*************************************************************************
uint8_t cero_en_bit(volatile uint8_t *LUGAR, uint8_t BIT);
uint8_t uno_en_bit(volatile uint8_t *LUGAR, uint8_t BIT);
uint8_t tecladoFuncion(volatile uint8_t *DDRT, volatile uint8_t *PORTT, volatile uint8_t *PINT);
//*************************************************************************

uint8_t filas = 0, filasend = 3, columnas = 4, columnasend = 7;

volatile uint8_t teclado[4][4] = {	
{7,4,1,15},
{8,5,2,0},
{9,6,3,14},
{10,11,12,13}};

int main(void)
{
	lcd_init(LCD_DISP_ON);
	char numeroToString[10];
	lcd_puts("Voltaje:");
	lcd_gotoxy(0,1);
	ADMUX = 0b01000111;
	ADCSRA = 0B10010011;
	
		while (1)
		{
			ADCSRA |= (1<<ADSC);
			while (uno_en_bit(&ADCSRA, ADSC)){}
			uint16_t res  = ADC;
			float resDisplay = ((float)res*5)/1023;
			dtostrf(resDisplay,3,1,numeroToString);
			lcd_puts(numeroToString);
		
			lcd_gotoxy(0,1);
			
		}
}


uint8_t cero_en_bit(volatile uint8_t *LUGAR, uint8_t BIT)
{
	return (!(*LUGAR&(1<<BIT)));
}

uint8_t uno_en_bit(volatile uint8_t *LUGAR, uint8_t BIT)
{
	return (*LUGAR&(1<<BIT));
}


